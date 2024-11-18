#include "plane.hpp"
#include "passengers.hpp"
#include <iostream>

PlaneSegment::PlaneSegment(size_t max_passengers, double max_total_weight)
        : max_passengers_(max_passengers),
          max_total_weight_(max_total_weight),
          current_hand_luggage_weight_(0.0),
          current_luggage_weight_(0.0) {
}

PlaneSegment::~PlaneSegment() {
    for (Unit *unit: units_) {
        delete unit;
    }
}

bool PlaneSegment::can_add_passenger(const Passenger &passenger) const {
    if (units_.size() >= max_passengers_) {
        return false;
    }
    double new_hand_luggage_weight = current_hand_luggage_weight_ + passenger.get_total_hand_luggage_weight();
    double new_luggage_weight = current_luggage_weight_ + passenger.get_total_luggage_weight();
    double new_total_weight = new_hand_luggage_weight + new_luggage_weight;
    if (new_total_weight > max_total_weight_) {
        return false;
    }
    return true;
}

bool PlaneSegment::add_passenger(Passenger *passenger) {
    if (!can_add_passenger(*passenger)) {
        return false;
    }
    units_.push_back(passenger);
    current_hand_luggage_weight_ += passenger->get_total_hand_luggage_weight();
    current_luggage_weight_ += passenger->get_total_luggage_weight();
    return true;
}

size_t PlaneSegment::get_unit_count() const {
    return units_.size();
}

double PlaneSegment::get_current_hand_luggage_weight() const {
    return current_hand_luggage_weight_;
}

double PlaneSegment::get_current_luggage_weight() const {
    return current_luggage_weight_;
}

double PlaneSegment::get_max_total_weight() const {
    return max_total_weight_;
}

FirstClassSegment::FirstClassSegment(double max_total_weight)
        : PlaneSegment(4, max_total_weight) {
}

std::string FirstClassSegment::get_segment_type() const {
    return "FIRST_CLASS_SEGMENT";
}

BusinessClassSegment::BusinessClassSegment(double max_total_weight)
        : PlaneSegment(10, max_total_weight) {
}

std::string BusinessClassSegment::get_segment_type() const {
    return "BUSINESS_CLASS_SEGMENT";
}

EconomyClassSegment::EconomyClassSegment(double max_total_weight)
        : PlaneSegment(200, max_total_weight) {
}

bool EconomyClassSegment::add_passenger(Passenger *passenger) {
    if (units_.size() >= max_passengers_) {
        return false;
    }
    double new_hand_luggage_weight = current_hand_luggage_weight_ + passenger->get_total_hand_luggage_weight();
    double new_luggage_weight = current_luggage_weight_ + passenger->get_total_luggage_weight();
    double new_total_weight = new_hand_luggage_weight + new_luggage_weight;

    if (new_total_weight <= max_total_weight_) {
        units_.push_back(passenger);
        current_hand_luggage_weight_ = new_hand_luggage_weight;
        current_luggage_weight_ = new_luggage_weight;
        return true;
    } else {
        units_.push_back(passenger);
        current_hand_luggage_weight_ += passenger->get_total_hand_luggage_weight();
        std::cout << "!!PASSENGER’S LUGGAGE REMOVED FROM FLIGHT, ID = "
                  << passenger->get_id() << "!!" << std::endl;
        return true;
    }
}

std::string EconomyClassSegment::get_segment_type() const {
    return "ECONOMY_CLASS_SEGMENT";
}

Plane::Plane()
        : first_class_segment_(nullptr),
          business_class_segment_(nullptr),
          economy_class_segment_(nullptr),
          economy_baggage_capacity_(0.0),
          economy_current_baggage_weight_(0.0),
          pilot_count_(0),
          max_pilot_count_(2),
          flight_attendant_count_(0),
          max_flight_attendant_count_(6) {
}

Plane::~Plane() {
    delete first_class_segment_;
    delete business_class_segment_;
    delete economy_class_segment_;

    for (CrewMember *crew: crew_members_) {
        delete crew;
    }
}

void Plane::set_first_class_segment(FirstClassSegment *segment) {
    first_class_segment_ = segment;
}

void Plane::set_business_class_segment(BusinessClassSegment *segment) {
    business_class_segment_ = segment;
}

void Plane::set_economy_class_segment(EconomyClassSegment *segment) {
    economy_class_segment_ = segment;
    economy_baggage_capacity_ = segment->get_max_total_weight();
    economy_current_baggage_weight_ = 0.0;
}

bool Plane::add_unit(Unit *unit) {
    if (CrewMember *crew_member = dynamic_cast<CrewMember *>(unit)) {
        if (crew_member->get_type() == CrewMemberType::PILOT) {
            if (pilot_count_ >= max_pilot_count_) {
                std::cout << "!!CANT REGISTER PILOT PASSENGER, ID = " << unit->get_id() << "!!" << std::endl;
                delete crew_member;
                return false;
            }
            crew_members_.push_back(crew_member);
            pilot_count_++;
            return true;
        } else if (crew_member->get_type() == CrewMemberType::FLIGHT_ATTENDANT) {
            if (flight_attendant_count_ >= max_flight_attendant_count_) {
                std::cout << "!!CANT REGISTER FLIGHT ATTENDANT PASSENGER, ID = " << unit->get_id() << "!!" << std::endl;
                delete crew_member;
                return false;
            }
            crew_members_.push_back(crew_member);
            flight_attendant_count_++;
            return true;
        }
    } else if (Passenger *passenger = dynamic_cast<Passenger *>(unit)) {
        PassengerClassType class_type = passenger->get_class_type();
        PlaneSegment *segment = nullptr;
        if (class_type == PassengerClassType::FIRST_CLASS) {
            segment = first_class_segment_;
        } else if (class_type == PassengerClassType::BUSINESS) {
            segment = business_class_segment_;
        } else if (class_type == PassengerClassType::ECONOMY) {
            segment = economy_class_segment_;
        }

        if (!segment) {
            std::cout << "!!CANT REGISTER " << passenger->get_class_type_string()
                      << " PASSENGER, ID = " << passenger->get_id() << "!!" << std::endl;
            delete passenger;
            return false;
        }

        if (class_type == PassengerClassType::ECONOMY) {
            if (!segment->add_passenger(passenger)) {
                std::cout << "!!CANT REGISTER ECONOMY PASSENGER, ID = " << passenger->get_id() << "!!" << std::endl;
                delete passenger;
                return false;
            }
            return true;
        } else {
            double new_hand_luggage_weight = segment->current_hand_luggage_weight_
                                             + passenger->get_total_hand_luggage_weight();
            double new_luggage_weight = segment->current_luggage_weight_
                                        + passenger->get_total_luggage_weight();
            double new_total_weight = new_hand_luggage_weight + new_luggage_weight;

            if (segment->units_.size() >= segment->max_passengers_) {
                std::cout << "!!CANT REGISTER " << passenger->get_class_type_string()
                          << " PASSENGER, ID = " << passenger->get_id() << "!!" << std::endl;
                delete passenger;
                return false;
            }

            if (new_total_weight <= segment->max_total_weight_) {
                segment->add_passenger(passenger);
                return true;
            } else {
                double needed_baggage_weight = passenger->get_total_luggage_weight();
                if (economy_current_baggage_weight_ + needed_baggage_weight <= economy_baggage_capacity_) {
                    economy_current_baggage_weight_ += needed_baggage_weight;
                    segment->units_.push_back(passenger);
                    segment->current_hand_luggage_weight_ += passenger->get_total_hand_luggage_weight();
                    return true;
                } else {
                    std::cout << "!!PASSENGER’S LUGGAGE REMOVED FROM FLIGHT, ID = "
                              << passenger->get_id() << "!!" << std::endl;

                    segment->units_.push_back(passenger);
                    segment->current_hand_luggage_weight_ += passenger->get_total_hand_luggage_weight();
                    return true;
                }
            }
        }
    }

    std::cout << "!!CANT REGISTER PASSENGER, ID = " << unit->get_id() << "!!" << std::endl;
    delete unit;
    return false;
}

double Plane::get_total_luggage_weight() const {
    double total = 0.0;

    if (first_class_segment_) {
        total += first_class_segment_->get_current_luggage_weight();
    }
    if (business_class_segment_) {
        total += business_class_segment_->get_current_luggage_weight();
    }
    if (economy_class_segment_) {
        total += economy_current_baggage_weight_;
    }
    return total;
}

double Plane::get_total_hand_luggage_weight() const {
    double total = 0.0;
    if (first_class_segment_) {
        total += first_class_segment_->get_current_hand_luggage_weight();
    }
    if (business_class_segment_) {
        total += business_class_segment_->get_current_hand_luggage_weight();
    }
    if (economy_class_segment_) {
        total += economy_class_segment_->get_current_hand_luggage_weight();
    }
    return total;
}
