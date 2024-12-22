#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>
#include <boost/fusion/include/define_struct.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/value_at.hpp>
#include <boost/fusion/include/extension.hpp>
#include <nlohmann/json.hpp>

// --------------------------------------------------------
// 1) Определим структуру, чтобы на этапе компиляции
//    проверять, что тип T допустим для сериализации.
//    Допустимые типы:
//        - int
//        - float
//        - std::string
//        - std::vector<T>, где T сам допустимый
//        - FusionT (вложенная boost::fusion структура)
// --------------------------------------------------------

// Предварительные заготовки
template <typename T, typename Enable = void>
struct is_serializable_type : std::false_type {};

// Базовые типы
template <>
struct is_serializable_type<int> : std::true_type {};

template <>
struct is_serializable_type<float> : std::true_type {};

template <>
struct is_serializable_type<std::string> : std::true_type {};

// Для std::vector<T> нужно рекурсивно смотреть тип T
template <typename T>
struct is_serializable_type<std::vector<T>>
    : std::conditional_t< is_serializable_type<T>::value, std::true_type, std::false_type >
{};

// Проверка, является ли T Fusion-последовательностью (структурой)
namespace detail
{
    // Будем проверять, что все члены структуры T также сериализуемые
    template <typename FusionT, std::size_t Index, std::size_t Size>
    struct check_all_members_serializable
    {
        static constexpr bool value()
        {
            using MemberT = typename boost::fusion::result_of::value_at_c<FusionT, Index>::type;
            // Если MemberT не сериализуемый — false
            if constexpr (!is_serializable_type<MemberT>::value)
            {
                return false;
            }
            else
            {
                // Проверяем следующий
                return check_all_members_serializable<FusionT, Index + 1, Size>::value();
            }
        }
    };

    // Специализация для выхода из рекурсии
    template <typename FusionT, std::size_t Size>
    struct check_all_members_serializable<FusionT, Size, Size>
    {
        static constexpr bool value() { return true; }
    };
}

// Специализация для boost::fusion структур (если T — fusion sequence)
template <typename T>
struct is_serializable_type<
    T,
    std::enable_if_t<boost::fusion::traits::is_sequence<T>::value>
>
{
    static constexpr bool value = detail::check_all_members_serializable<T, 0, boost::fusion::result_of::size<T>::value>::value();
};

// --------------------------------------------------------
// 2) Вспомогательные функции:
//    - получение имени члена структуры по индексу
//    - заполнение nlohmann::json из поля
//    - чтение поля из nlohmann::json
// --------------------------------------------------------

// Имя поля хранится внутри Boost.Fusion через extension::struct_member_name<T, N>
template <typename FusionT, int N>
std::string GetFieldName()
{
    return boost::fusion::extension::struct_member_name<FusionT, N>::call();
}

// Универсальная "запись поля" в JSON
// (перегрузки для int, float, std::string, std::vector, fusion-struct)
template <typename T>
void WriteToJson(nlohmann::json& j, const std::string& key, const T& value);

// Специализация для int, float, std::string
template <>
void WriteToJson<int>(nlohmann::json& j, const std::string& key, const int& value)
{
    j[key] = value;
}

template <>
void WriteToJson<float>(nlohmann::json& j, const std::string& key, const float& value)
{
    j[key] = value;
}

template <>
void WriteToJson<std::string>(nlohmann::json& j, const std::string& key, const std::string& value)
{
    j[key] = value;
}

// Специализация для std::vector<T>
template <typename T>
void WriteToJson(nlohmann::json& j, const std::string& key, const std::vector<T>& vec)
{
    nlohmann::json arr = nlohmann::json::array();
    for (auto const& elem : vec)
    {
        // Рекурсивно пишем отдельный элемент
        nlohmann::json element;
        WriteToJson<T>(element, "_", elem);  
        // т.к. element имеет временный ключ "_",
        // нужно вытащить значение:
        arr.push_back(element["_"]);
    }
    j[key] = arr;
}

// Специализация для вложенной fusion-структуры
template <typename FusionT>
void WriteToJson(nlohmann::json& j, const std::string& key, const FusionT& value,
                 std::enable_if_t<boost::fusion::traits::is_sequence<FusionT>::value>* = 0);

// Реализация
template <typename FusionT>
void WriteToJson(nlohmann::json& j, const std::string& key, const FusionT& value,
                 std::enable_if_t<boost::fusion::traits::is_sequence<FusionT>::value>*)
{
    // Сериализуем через общий Serialize
    j[key] = nlohmann::json::parse( Serialize(value) );
}

// Аналогично "чтение из JSON"
// (перегрузки для int, float, std::string, std::vector<T>, fusion-struct)
template <typename T>
void ReadFromJson(const nlohmann::json& j, const std::string& key, T& value);

template <>
void ReadFromJson<int>(const nlohmann::json& j, const std::string& key, int& value)
{
    if (!j.contains(key)) {
        throw std::runtime_error("Missing key '" + key + "'");
    }
    if (!j[key].is_number_integer()) {
        throw std::runtime_error("Key '" + key + "' is not integer");
    }
    value = j[key].get<int>();
}

template <>
void ReadFromJson<float>(const nlohmann::json& j, const std::string& key, float& value)
{
    if (!j.contains(key)) {
        throw std::runtime_error("Missing key '" + key + "'");
    }
    if (!j[key].is_number_float() && !j[key].is_number_integer()) {
        throw std::runtime_error("Key '" + key + "' is not float");
    }
    value = j[key].get<float>();
}

template <>
void ReadFromJson<std::string>(const nlohmann::json& j, const std::string& key, std::string& value)
{
    if (!j.contains(key)) {
        throw std::runtime_error("Missing key '" + key + "'");
    }
    if (!j[key].is_string()) {
        throw std::runtime_error("Key '" + key + "' is not string");
    }
    value = j[key].get<std::string>();
}

// Для std::vector<T>
template <typename T>
void ReadFromJson(const nlohmann::json& j, const std::string& key, std::vector<T>& vec)
{
    if (!j.contains(key)) {
        throw std::runtime_error("Missing key '" + key + "'");
    }
    if (!j[key].is_array()) {
        throw std::runtime_error("Key '" + key + "' is not array");
    }
    vec.clear();
    for (auto const& element : j[key])
    {
        T tmp{};
        // Рекурсивно читаем элемент
        // Чтобы использовать ReadFromJson, "обманем" его ключом "_"
        nlohmann::json tempJson;
        tempJson["_"] = element;
        ReadFromJson<T>(tempJson, "_", tmp);
        vec.push_back(tmp);
    }
}

// Для вложенной fusion-структуры
template <typename FusionT>
void ReadFromJson(const nlohmann::json& j, const std::string& key, FusionT& value,
                  std::enable_if_t<boost::fusion::traits::is_sequence<FusionT>::value>* = 0)
{
    if (!j.contains(key)) {
        throw std::runtime_error("Missing key '" + key + "'");
    }
    if (!j[key].is_object()) {
        throw std::runtime_error("Key '" + key + "' is not object");
    }
    // Десериализуем через общий Deserialize
    // Превращаем j[key] в строку и вызываем Deserialize<FusionT>
    auto str = j[key].dump();
    value = Deserialize<FusionT>(str);
}

// --------------------------------------------------------
// 3) Serialize/Deserialize
// --------------------------------------------------------

// Обход полей FusionT и сериализация в JSON
template <typename FusionT>
std::string Serialize(const FusionT& obj)
{
    // compile-time проверка всех полей
    static_assert(is_serializable_type<FusionT>::value,
                  "FusionT contains non-serializable fields!");

    nlohmann::json j;
    // Соберём названия всех полей, чтобы потом свериться (при желании)
    // и чтобы последовательно их заполнить
    constexpr auto size = boost::fusion::result_of::size<FusionT>::value;

    // Функтор, который обходит каждое поле
    auto visitor = [&](auto& field, auto index)
    {
        constexpr int i = decltype(index)::value;
        // Имя поля
        std::string key = GetFieldName<FusionT, i>();
        // Запись поля field в j[key]
        WriteToJson<std::decay_t<decltype(field)>>(j, key, field);
    };

    boost::fusion::for_each(obj, visitor);

    return j.dump(); // Вернём строку JSON
}

// Обратная операция
template <typename FusionT>
FusionT Deserialize(std::string_view sv)
{
    // compile-time проверка
    static_assert(is_serializable_type<FusionT>::value,
                  "FusionT contains non-serializable fields!");

    nlohmann::json j = nlohmann::json::parse(sv.begin(), sv.end());

    // Проверим, что j — именно объект
    if (!j.is_object()) {
        throw std::runtime_error("Top-level JSON is not object");
    }

    // Проверка, что нет "лишних" ключей
    // 1) Собираем все ключи в структуре
    constexpr auto size = boost::fusion::result_of::size<FusionT>::value;
    std::vector<std::string> structKeys;
    structKeys.reserve(size);

    boost::fusion::for_each(
        FusionT{}, // пустой объект лишь для вызова GetFieldName
        [&](auto& field, auto index)
        {
            constexpr int i = decltype(index)::value;
            structKeys.push_back(GetFieldName<FusionT, i>());
        }
    );

    // 2) Проверяем, что в j нет полей, которых нет в structKeys
    for (auto it = j.begin(); it != j.end(); ++it)
    {
        const auto& key = it.key();
        if (std::find(structKeys.begin(), structKeys.end(), key) == structKeys.end())
        {
            throw std::runtime_error("Unknown field in JSON: '" + key + "'");
        }
    }

    FusionT result;
    // Функтор, который будет читать поле в result
    auto reader = [&](auto& field, auto index)
    {
        constexpr int i = decltype(index)::value;
        std::string key = GetFieldName<FusionT, i>();
        // Считываем
        ReadFromJson<std::decay_t<decltype(field)>>(j, key, field);
    };

    boost::fusion::for_each(result, reader);

    return result;
}

// --------------------------------------------------------
// 4) Пример использования + тесты
// --------------------------------------------------------

// По условию, могут быть вот такие структуры:
BOOST_FUSION_DEFINE_STRUCT(
    (pkg),
    S1,
    (int, r0)
)

BOOST_FUSION_DEFINE_STRUCT(
    (pkg),
    S2,
    (float, val)
)

BOOST_FUSION_DEFINE_STRUCT(
    (pkg),
    S3,
    (int, r1)
    (std::string, some_str)
    (std::vector<int>, vals)
    (S2, s2_val)
    (std::vector<S1>, s1_vals)
)

int main()
{
    // Заполним тестовую структуру
    pkg::S3 s3;
    s3.r1 = 123;
    s3.some_str = "abcde";
    s3.vals = {1, 2, 3};
    s3.s2_val.val = 1.22f;
    s3.s1_vals = { pkg::S1{1}, pkg::S1{2} };

    // Сериализуем
    std::string jsonStr = Serialize(s3);
    std::cout << "Serialized:\n" << jsonStr << "\n\n";

    // Десериализуем обратно
    auto s3_copy = Deserialize<pkg::S3>(jsonStr);

    // Проверим, что поля совпали
    std::cout << "Deserialized:\n"
              << "r1 = " << s3_copy.r1 << "\n"
              << "some_str = " << s3_copy.some_str << "\n"
              << "vals = ";
    for (auto v : s3_copy.vals) std::cout << v << " ";
    std::cout << "\ns2_val.val = " << s3_copy.s2_val.val << "\n"
              << "s1_vals = ";
    for (auto& s1 : s3_copy.s1_vals) std::cout << s1.r0 << " ";
    std::cout << "\n";

    // Попробуем "сломать" десериализацию: добавить лишнее поле
    try
    {
        std::string badJson = R"({
            "r1": 123,
            "some_str": "abcde",
            "vals": [1,2,3],
            "s2_val": { "val": 1.22 },
            "s1_vals": [{"r0":1}, {"r0":2}],
            "extra_field": "boom"
        })";
        auto broken = Deserialize<pkg::S3>(badJson);
        (void)broken; // Чтобы не ругался компилятор
    }
    catch (const std::exception& ex)
    {
        std::cout << "\nCaught expected error on unknown field: " << ex.what() << "\n";
    }

    // Всё ок
    return 0;
}
