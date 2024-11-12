#include "../units/passengers.hpp"
#include <cassert>
#include <vector>

int main() {
    std::vector<double> hand_luggage = {5.0};
    std::vector<double> luggage = {20.0};

    EconomyPassenger *p1 = new EconomyPassenger(hand_luggage, luggage);
    assert(p1->get_class_type() == PassengerClassType::ECONOMY);
    assert(p1->get_class_type_string() == "ECONOMY");
    assert(p1->get_total_hand_luggage_weight() == 5.0);
    assert(p1->get_total_luggage_weight() == 20.0);

    BusinessPassenger *p2 = new BusinessPassenger({10.0, 5.0}, {30.0, 10.0});
    assert(p2->get_class_type() == PassengerClassType::BUSINESS);
    assert(p2->get_class_type_string() == "BUSINESS");
    assert(p2->get_total_hand_luggage_weight() == 15.0);
    assert(p2->get_total_luggage_weight() == 40.0);

    FirstClassPassenger *p3 = new FirstClassPassenger({20.0, 20.0}, {50.0, 40.0});
    assert(p3->get_class_type() == PassengerClassType::FIRST_CLASS);
    assert(p3->get_class_type_string() == "FIRST CLASS");
    assert(p3->get_total_hand_luggage_weight() == 40.0);
    assert(p3->get_total_luggage_weight() == 90.0);

    delete p1;
    delete p2;
    delete p3;

    return 0;
}