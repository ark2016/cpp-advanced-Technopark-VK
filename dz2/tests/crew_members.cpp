#include "../units/crew_members.hpp"
#include <cassert>
#include <iostream>

int main() {
    Pilot *pilot1 = new Pilot();
    assert(pilot1->get_type() == CrewMemberType::PILOT);
    assert(pilot1->get_type_string() == "PILOT");
    assert(pilot1->get_total_hand_luggage_weight() == 0.0);
    assert(pilot1->get_total_luggage_weight() == 0.0);

    FlightAttendant *fa1 = new FlightAttendant();
    assert(fa1->get_type() == CrewMemberType::FLIGHT_ATTENDANT);
    assert(fa1->get_type_string() == "FLIGHT ATTENDANT");
    assert(fa1->get_total_hand_luggage_weight() == 0.0);
    assert(fa1->get_total_luggage_weight() == 0.0);

    delete pilot1;
    delete fa1;

    std::cout << "All tests passed!" << std::endl;

    return 0;
}