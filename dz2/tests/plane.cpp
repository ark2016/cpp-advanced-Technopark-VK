#include "../units/plane.hpp"
#include "../units/passengers.hpp"
#include "../units/crew_members.hpp"

#include <cassert>

int main() {
    Plane plane;
    // Set up segments
    plane.set_first_class_segment(new FirstClassSegment(1000.0));
    plane.set_business_class_segment(new BusinessClassSegment(2000.0));
    plane.set_economy_class_segment(new EconomyClassSegment(3000.0));

    // Add crew
    plane.add_unit(new Pilot());
    plane.add_unit(new Pilot());
    plane.add_unit(new Pilot()); // Should print error

    for (int i = 0; i < 6; ++i) {
        plane.add_unit(new FlightAttendant());
    }
    plane.add_unit(new FlightAttendant()); // Should print error

    // Add passengers
    plane.add_unit(new EconomyPassenger({5.0}, {20.0}));
    plane.add_unit(new BusinessPassenger({10.0, 10.0}, {30.0, 10.0}));
    plane.add_unit(new FirstClassPassenger({30.0, 30.0}, {40.0, 20.0}));

    // Print weights
    double total_luggage_weight = plane.get_total_luggage_weight();
    double total_hand_luggage_weight = plane.get_total_hand_luggage_weight();

    // For now, just check that methods return positive numbers
    assert(total_luggage_weight > 0.0);
    assert(total_hand_luggage_weight > 0.0);

    return 0;
}