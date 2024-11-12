#ifndef PASSENGERS_HPP
#define PASSENGERS_HPP

#include "unit.hpp"
#include <string>
#include <vector>

enum class PassengerClassType {
    ECONOMY,
    BUSINESS,
    FIRST_CLASS
};

class Passenger : public Unit {
protected:
    PassengerClassType class_type_;
    std::vector<double> hand_luggage_weights_;
    std::vector<double> luggage_weights_;

public:
    Passenger(PassengerClassType class_type,
              const std::vector<double> &hand_luggage_weights,
              const std::vector<double> &luggage_weights);

    virtual ~Passenger() = default;

    PassengerClassType get_class_type() const;

    virtual std::string get_class_type_string() const = 0;

    double get_total_hand_luggage_weight() const override;

    double get_total_luggage_weight() const override;

    const std::vector<double> &get_hand_luggage_weights() const;

    const std::vector<double> &get_luggage_weights() const;
};

// Specific passenger classes

class EconomyPassenger : public Passenger {
public:
    EconomyPassenger(const std::vector<double> &hand_luggage_weights,
                     const std::vector<double> &luggage_weights);

    std::string get_class_type_string() const override;
};

class BusinessPassenger : public Passenger {
public:
    BusinessPassenger(const std::vector<double> &hand_luggage_weights,
                      const std::vector<double> &luggage_weights);

    std::string get_class_type_string() const override;
};

class FirstClassPassenger : public Passenger {
public:
    FirstClassPassenger(const std::vector<double> &hand_luggage_weights,
                        const std::vector<double> &luggage_weights);

    std::string get_class_type_string() const override;
};

#endif // PASSENGERS_HPP