#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>
#include <ranges>

// --------------------------------------------------------
// 1) Define your structs within the pkg namespace
// --------------------------------------------------------

namespace pkg {
    struct S1 {
        int r0;
    };

    struct S2 {
        float val;
    };

    struct S3 {
        int r1;
        std::string some_str;
        std::vector<int> vals;
        S2 s2_val;
        std::vector<S1> s1_vals;
    };
}

// --------------------------------------------------------
// 2) Adapt the structs for Boost.Fusion
// --------------------------------------------------------

BOOST_FUSION_ADAPT_STRUCT(
    pkg::S1,
    r0
)

BOOST_FUSION_ADAPT_STRUCT(
    pkg::S2,
    val
)

BOOST_FUSION_ADAPT_STRUCT(
    pkg::S3,
    r1,
    some_str,
    vals,
    s2_val,
    s1_vals
)

// --------------------------------------------------------
// 3) Define concepts for serializable types
// --------------------------------------------------------

template <typename T>
concept Serializable = requires(T t) {
    { std::to_string(t) } -> std::same_as<std::string>;
} || std::same_as<T, int> || std::same_as<T, float> || std::same_as<T, std::string> ||
    std::is_same_v<T, std::vector<int>> || std::is_same_v<T, std::vector<float>> ||
    std::is_same_v<T, std::vector<std::string>>;

// For nested structs (FusionT)
template <typename T>
concept FusionStruct = boost::fusion::traits::is_sequence<T>::value;

// --------------------------------------------------------
// 4) Serialize / Deserialize Templates Implementation
// --------------------------------------------------------

// Forward declarations for Serialize and Deserialize
template <typename T>
nlohmann::json Serialize(const T& obj);

template <typename T>
T Deserialize(const nlohmann::json& j);

// --------------------------------------------------------
// 5) Helper functions for JSON serialization/deserialization
// --------------------------------------------------------

// Base template declarations
template <typename T>
void WriteToJson(nlohmann::json& j, const std::string& key, const T& value);

template <typename T>
void ReadFromJson(const nlohmann::json& j, const std::string& key, T& value);

// --------------------------------------------------------
// 6) Serialize / Deserialize Implementations
// --------------------------------------------------------

// Serialize for Serializable types
template <typename T>
requires Serializable<T>
nlohmann::json Serialize(const T& obj) {
    nlohmann::json j;
    WriteToJson(j, "value", obj);
    return j;
}

// Deserialize for Serializable types
template <typename T>
requires Serializable<T>
T Deserialize(const nlohmann::json& j) {
    T obj;
    ReadFromJson(j, "value", obj);
    return obj;
}

// Helper function to serialize using compile-time index sequence
template <typename FusionT, std::size_t... Is>
nlohmann::json SerializeImpl(const FusionT& obj, std::index_sequence<Is...>) {
    nlohmann::json j;
    // Fold expression to serialize each member
    (void)std::initializer_list<int>{(
        WriteToJson(j, boost::fusion::extension::struct_member_name<FusionT, Is>::call(),
                    boost::fusion::at_c<Is>(obj)),
        0
    )...};
    return j;
}

template <FusionStruct FusionT>
nlohmann::json Serialize(const FusionT& obj) {
    constexpr std::size_t size = boost::fusion::result_of::size<FusionT>::type::value;
    return SerializeImpl(obj, std::make_index_sequence<size>{});
}

// Helper function to deserialize using compile-time index sequence
template <typename FusionT, std::size_t... Is>
FusionT DeserializeImpl(const nlohmann::json& j, std::index_sequence<Is...>) {
    FusionT obj;
    // Fold expression to deserialize each member
    (void)std::initializer_list<int>{(
        ReadFromJson(j, boost::fusion::extension::struct_member_name<FusionT, Is>::call(),
                    boost::fusion::at_c<Is>(obj)),
        0
    )...};
    return obj;
}

template <FusionStruct FusionT>
FusionT Deserialize(const nlohmann::json& j) {
    constexpr std::size_t size = boost::fusion::result_of::size<FusionT>::type::value;
    return DeserializeImpl<FusionT>(j, std::make_index_sequence<size>{});
}

// --------------------------------------------------------
// 7) Specializations for WriteToJson
// --------------------------------------------------------

// Primitive and directly serializable types
template <>
void WriteToJson<int>(nlohmann::json& j, const std::string& key, const int& value) {
    j[key] = value;
}

template <>
void WriteToJson<float>(nlohmann::json& j, const std::string& key, const float& value) {
    j[key] = value;
}

template <>
void WriteToJson<std::string>(nlohmann::json& j, const std::string& key, const std::string& value) {
    j[key] = value;
}

template <>
void WriteToJson<std::vector<int>>(nlohmann::json& j, const std::string& key, const std::vector<int>& vec) {
    j[key] = vec;
}

template <>
void WriteToJson<std::vector<float>>(nlohmann::json& j, const std::string& key, const std::vector<float>& vec) {
    j[key] = vec;
}

template <>
void WriteToJson<std::vector<std::string>>(nlohmann::json& j, const std::string& key, const std::vector<std::string>& vec) {
    j[key] = vec;
}

// Serialize FusionStruct types
template <FusionStruct FusionT>
void WriteToJson(nlohmann::json& j, const std::string& key, const FusionT& value) {
    j[key] = Serialize(value);
}

// Serialize containers of FusionStruct types
template <typename T>
requires FusionStruct<T>
void WriteToJson(nlohmann::json& j, const std::string& key, const std::vector<T>& vec) {
    nlohmann::json array = nlohmann::json::array();
    for (const auto& item : vec) {
        array.push_back(Serialize(item));
    }
    j[key] = array;
}

// --------------------------------------------------------
// 8) Specializations for ReadFromJson
// --------------------------------------------------------

// Primitive and directly deserializable types
template <>
void ReadFromJson<int>(const nlohmann::json& j, const std::string& key, int& value) {
    if (j.contains(key) && j[key].is_number_integer()) {
        value = j[key].get<int>();
    } else {
        throw std::runtime_error("Invalid type or missing field: " + key);
    }
}

template <>
void ReadFromJson<float>(const nlohmann::json& j, const std::string& key, float& value) {
    if (j.contains(key) && (j[key].is_number_float() || j[key].is_number_integer())) {
        value = j[key].get<float>();
    } else {
        throw std::runtime_error("Invalid type or missing field: " + key);
    }
}

template <>
void ReadFromJson<std::string>(const nlohmann::json& j, const std::string& key, std::string& value) {
    if (j.contains(key) && j[key].is_string()) {
        value = j[key].get<std::string>();
    } else {
        throw std::runtime_error("Invalid type or missing field: " + key);
    }
}

template <>
void ReadFromJson<std::vector<int>>(const nlohmann::json& j, const std::string& key, std::vector<int>& vec) {
    if (j.contains(key) && j[key].is_array()) {
        vec = j[key].get<std::vector<int>>();
    } else {
        throw std::runtime_error("Invalid type or missing field: " + key);
    }
}

template <>
void ReadFromJson<std::vector<float>>(const nlohmann::json& j, const std::string& key, std::vector<float>& vec) {
    if (j.contains(key) && j[key].is_array()) {
        vec = j[key].get<std::vector<float>>();
    } else {
        throw std::runtime_error("Invalid type or missing field: " + key);
    }
}

template <>
void ReadFromJson<std::vector<std::string>>(const nlohmann::json& j, const std::string& key, std::vector<std::string>& vec) {
    if (j.contains(key) && j[key].is_array()) {
        vec = j[key].get<std::vector<std::string>>();
    } else {
        throw std::runtime_error("Invalid type or missing field: " + key);
    }
}

// Deserialize FusionStruct types
template <FusionStruct FusionT>
void ReadFromJson(const nlohmann::json& j, const std::string& key, FusionT& value) {
    if (j.contains(key) && j[key].is_object()) {
        value = Deserialize<FusionT>(j[key]);
    } else {
        throw std::runtime_error("Invalid type or missing field: " + key);
    }
}

// Deserialize containers of FusionStruct types
template <typename T>
requires FusionStruct<T>
void ReadFromJson(const nlohmann::json& j, const std::string& key, std::vector<T>& vec) {
    if (j.contains(key) && j[key].is_array()) {
        for (const auto& item : j[key]) {
            vec.emplace_back(Deserialize<T>(item));
        }
    } else {
        throw std::runtime_error("Invalid type or missing field: " + key);
    }
}

// --------------------------------------------------------
// 9) Example Usage and Tests
// --------------------------------------------------------

int main() {
    try {
        // Populate the test structure
        pkg::S3 s3;
        s3.r1 = 123;
        s3.some_str = "abcde";
        s3.vals = {1, 2, 3};
        s3.s2_val.val = 1.22f;
        s3.s1_vals = { pkg::S1{1}, pkg::S1{2} };

        // Serialize
        nlohmann::json jsonObj = Serialize(s3);
        std::cout << "Serialized:\n" << jsonObj.dump(4) << "\n\n";

        // Deserialize back
        auto s3_copy = Deserialize<pkg::S3>(jsonObj);

        // Verify that the fields match
        std::cout << "Deserialized:\n"
                  << "r1 = " << s3_copy.r1 << "\n"
                  << "some_str = " << s3_copy.some_str << "\n"
                  << "vals = ";
        for (auto v : s3_copy.vals) std::cout << v << " ";
        std::cout << "\ns2_val.val = " << s3_copy.s2_val.val << "\n"
                  << "s1_vals = ";
        for (auto v : s3_copy.s1_vals) std::cout << v.r0 << " ";
        std::cout << std::endl;
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
    return 0;
}

