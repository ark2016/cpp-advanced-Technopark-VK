#ifndef PLANE_HPP
#define PLANE_HPP

#include "crew_members.hpp"
#include "passengers.hpp"
#include "unit.hpp"
#include <vector>
#include <string>

class PlaneSegment {
protected:
    friend class Plane;

    size_t max_passengers_;
    double max_total_weight_;
    double current_hand_luggage_weight_;
    double current_luggage_weight_;
    std::vector<Unit *> units_; // Passengers in this segment

public:
    PlaneSegment(size_t max_passengers, double max_total_weight);

    virtual ~PlaneSegment();

    virtual bool can_add_passenger(const Passenger &passenger) const;

    virtual bool add_passenger(Passenger *passenger);

    size_t get_unit_count() const;

    double get_current_hand_luggage_weight() const;

    double get_current_luggage_weight() const;

    double get_max_total_weight() const;

    virtual std::string get_segment_type() const = 0;
};

class FirstClassSegment : public PlaneSegment {
public:
    FirstClassSegment(double max_total_weight);

    std::string get_segment_type() const override;
};

class BusinessClassSegment : public PlaneSegment {
public:
    BusinessClassSegment(double max_total_weight);

    std::string get_segment_type() const override;
};

class EconomyClassSegment : public PlaneSegment {
public:
    EconomyClassSegment(double max_total_weight);

    bool add_passenger(Passenger *passenger) override;

    std::string get_segment_type() const override;
};

class Plane {
private:
    FirstClassSegment *first_class_segment_;
    BusinessClassSegment *business_class_segment_;
    EconomyClassSegment *economy_class_segment_;
    std::vector<CrewMember *> crew_members_;

    // Baggage capacities
    double economy_baggage_capacity_;
    double economy_current_baggage_weight_;

    // Crew capacities
    size_t pilot_count_;
    size_t max_pilot_count_;
    size_t flight_attendant_count_;
    size_t max_flight_attendant_count_;

public:
    Plane();

    ~Plane();

    void set_first_class_segment(FirstClassSegment *segment);

    void set_business_class_segment(BusinessClassSegment *segment);

    void set_economy_class_segment(EconomyClassSegment *segment);

    bool add_unit(Unit *unit);

    double get_total_luggage_weight() const;

    double get_total_hand_luggage_weight() const;

    // For the output
    void print_segment_load() const;
};

#endif // PLANE_HPP