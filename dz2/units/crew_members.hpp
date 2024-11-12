#ifndef CREW_MEMBERS_HPP
#define CREW_MEMBERS_HPP

#include "unit.hpp"
#include <string>

enum class CrewMemberType {
    PILOT,
    FLIGHT_ATTENDANT
};

class CrewMember : public Unit {
protected:
    CrewMemberType type_;

public:
    CrewMember(CrewMemberType type);

    virtual ~CrewMember() = default;

    CrewMemberType get_type() const;

    virtual std::string get_type_string() const = 0;

    double get_total_hand_luggage_weight() const override;

    double get_total_luggage_weight() const override;
};

// Specific crew member classes
class Pilot : public CrewMember {
public:
    Pilot();

    std::string get_type_string() const override;
};

class FlightAttendant : public CrewMember {
public:
    FlightAttendant();

    std::string get_type_string() const override;
};

#endif // CREW_MEMBERS_HPP