#include "crew_members.hpp"

CrewMember::CrewMember(CrewMemberType type) : type_(type) {
}

CrewMemberType CrewMember::get_type() const {
    return type_;
}

double CrewMember::get_total_hand_luggage_weight() const {
    return 0.0;
}

double CrewMember::get_total_luggage_weight() const {
    return 0.0;
}

// Pilot

Pilot::Pilot() : CrewMember(CrewMemberType::PILOT) {
}

std::string Pilot::get_type_string() const {
    return "PILOT";
}

// FlightAttendant

FlightAttendant::FlightAttendant() : CrewMember(CrewMemberType::FLIGHT_ATTENDANT) {
}

std::string FlightAttendant::get_type_string() const {
    return "FLIGHT ATTENDANT";
}