#include "passengers.hpp"

Passenger::Passenger(PassengerClassType class_type,
                     const std::vector<double> &hand_luggage_weights,
                     const std::vector<double> &luggage_weights)
        : class_type_(class_type),
          hand_luggage_weights_(hand_luggage_weights),
          luggage_weights_(luggage_weights) {
}

PassengerClassType Passenger::get_class_type() const {
    return class_type_;
}

double Passenger::get_total_hand_luggage_weight() const {
    double sum = 0.0;
    for (double w: hand_luggage_weights_) {
        sum += w;
    }
    return sum;
}

double Passenger::get_total_luggage_weight() const {
    double sum = 0.0;
    for (double w: luggage_weights_) {
        sum += w;
    }
    return sum;
}

const std::vector<double> &Passenger::get_hand_luggage_weights() const {
    return hand_luggage_weights_;
}

const std::vector<double> &Passenger::get_luggage_weights() const {
    return luggage_weights_;
}

// Economy Passenger

EconomyPassenger::EconomyPassenger(const std::vector<double> &hand_luggage_weights,
                                   const std::vector<double> &luggage_weights)
        : Passenger(PassengerClassType::ECONOMY, hand_luggage_weights, luggage_weights) {
}

std::string EconomyPassenger::get_class_type_string() const {
    return "ECONOMY";
}

// Business Passenger

BusinessPassenger::BusinessPassenger(const std::vector<double> &hand_luggage_weights,
                                     const std::vector<double> &luggage_weights)
        : Passenger(PassengerClassType::BUSINESS, hand_luggage_weights, luggage_weights) {
}

std::string BusinessPassenger::get_class_type_string() const {
    return "BUSINESS";
}

// First Class Passenger

FirstClassPassenger::FirstClassPassenger(const std::vector<double> &hand_luggage_weights,
                                         const std::vector<double> &luggage_weights)
        : Passenger(PassengerClassType::FIRST_CLASS, hand_luggage_weights, luggage_weights) {
}

std::string FirstClassPassenger::get_class_type_string() const {
    return "FIRST CLASS";
}