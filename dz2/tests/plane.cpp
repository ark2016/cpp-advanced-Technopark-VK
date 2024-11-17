#include "../units/plane.hpp"
#include "../units/passengers.hpp"
#include "../units/crew_members.hpp"

#include <cassert>
#include <iostream>

int main() {
    Plane plane;
    // Set up segments
    plane.set_first_class_segment(new FirstClassSegment(1000.0));
    plane.set_business_class_segment(new BusinessClassSegment(2000.0));
    plane.set_economy_class_segment(new EconomyClassSegment(3000.0));

    // Add crew
    assert(plane.add_unit(new Pilot()));
    assert(plane.add_unit(new Pilot()));
    assert(!plane.add_unit(new Pilot())); // Should print error and return false

    for (int i = 0; i < 6; ++i) {
        assert(plane.add_unit(new FlightAttendant()));
    }
    assert(!plane.add_unit(new FlightAttendant())); // Should print error and return false

    // Add passengers
    assert(plane.add_unit(new EconomyPassenger({5.0}, {20.0})));
    assert(plane.add_unit(new BusinessPassenger({10.0, 10.0}, {30.0, 10.0})));
    assert(plane.add_unit(new FirstClassPassenger({30.0, 30.0}, {40.0, 20.0})));

    // Test edge cases for passengers
    assert(plane.add_unit(new EconomyPassenger({}, {}))); // Empty luggage should be added
    assert(plane.add_unit(new BusinessPassenger({}, {}))); // Empty luggage should be added
    assert(plane.add_unit(new FirstClassPassenger({}, {}))); // Empty luggage should be added

    // Test adding passengers beyond capacity
    for (int i = 0; i < 197; ++i) {
        assert(plane.add_unit(new EconomyPassenger({5.0}, {20.0})));
    }

    // Print weights
    double total_luggage_weight = plane.get_total_luggage_weight();
    double total_hand_luggage_weight = plane.get_total_hand_luggage_weight();

    // Check that methods return expected values
    assert(total_luggage_weight > 0.0);
    assert(total_hand_luggage_weight > 0.0);

    // Test get_max_total_weight for each segment
    assert(plane.get_first_class_segment()->get_max_total_weight() == 1000.0);
    assert(plane.get_business_class_segment()->get_max_total_weight() == 2000.0);
    assert(plane.get_economy_class_segment()->get_max_total_weight() == 3000.0);

    // Test get_segment_type for each segment
    assert(plane.get_first_class_segment()->get_segment_type() == "FIRST_CLASS_SEGMENT");
    assert(plane.get_business_class_segment()->get_segment_type() == "BUSINESS_CLASS_SEGMENT");
    assert(plane.get_economy_class_segment()->get_segment_type() == "ECONOMY_CLASS_SEGMENT");

    // Test adding passengers with luggage that exceeds the segment's capacity but fits in the economy baggage compartment
    assert(plane.add_unit(new EconomyPassenger({5.0}, {200.0}))); // Should fit in the economy baggage compartment
    assert(plane.add_unit(new BusinessPassenger({10.0, 10.0}, {150.0, 50.0}))); // Should fit in the economy baggage compartment
    assert(plane.add_unit(new FirstClassPassenger({30.0, 30.0}, {100.0, 10.0}))); // Should fit in the economy baggage compartment

    // Test get_unit_count for each segment
    assert(plane.get_first_class_segment()->get_unit_count() == 3);
    assert(plane.get_business_class_segment()->get_unit_count() == 3);
    assert(plane.get_economy_class_segment()->get_unit_count() == 200);

    // Test get_current_hand_luggage_weight and get_current_luggage_weight for each segment
    assert(plane.get_first_class_segment()->get_current_hand_luggage_weight() == 120.0);
    assert(plane.get_first_class_segment()->get_current_luggage_weight() == 170.0);
    assert(plane.get_business_class_segment()->get_current_hand_luggage_weight() == 40.0);
    assert(plane.get_business_class_segment()->get_current_luggage_weight() == 240.0);
    assert(plane.get_economy_class_segment()->get_current_hand_luggage_weight() == 995.0);
    assert(plane.get_economy_class_segment()->get_current_luggage_weight() == 2400.0);

    // Test can_add_passenger for each segment
    assert(plane.get_first_class_segment()->can_add_passenger(FirstClassPassenger({}, {})));
    assert(plane.get_business_class_segment()->can_add_passenger(BusinessPassenger({}, {})));
    assert(!plane.get_economy_class_segment()->can_add_passenger(EconomyPassenger({}, {})));

    // Test adding passengers with luggage that exceeds the segment's capacity and cannot fit in the economy baggage compartment
    assert(!plane.add_unit(new EconomyPassenger({5.0}, {3000.0}))); // Should not fit in the economy baggage compartment
    assert(plane.add_unit(new BusinessPassenger({10.0, 10.0}, {3000.0, 50.0}))); // Should not fit in the economy baggage compartment
    assert(plane.add_unit(new FirstClassPassenger({30.0, 30.0}, {3000.0, 10.0}))); // Should not fit in the economy baggage compartment

    std::cout << "All tests passed!" << std::endl;

    return 0;
}

