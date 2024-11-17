#include "../units/passengers.hpp"
#include <cassert>
#include <vector>
#include <iostream>

int main() {
    std::vector<double> hand_luggage = {5.0};
    std::vector<double> luggage = {20.0};

    EconomyPassenger *p1 = new EconomyPassenger(hand_luggage, luggage);
    assert(p1->get_class_type() == PassengerClassType::ECONOMY);
    assert(p1->get_class_type_string() == "ECONOMY");
    assert(p1->get_total_hand_luggage_weight() == 5.0);
    assert(p1->get_total_luggage_weight() == 20.0);
    assert(p1->get_hand_luggage_weights() == hand_luggage);
    assert(p1->get_luggage_weights() == luggage);

    BusinessPassenger *p2 = new BusinessPassenger({10.0, 5.0}, {30.0, 10.0});
    assert(p2->get_class_type() == PassengerClassType::BUSINESS);
    assert(p2->get_class_type_string() == "BUSINESS");
    assert(p2->get_total_hand_luggage_weight() == 15.0);
    assert(p2->get_total_luggage_weight() == 40.0);
    assert(p2->get_hand_luggage_weights() == std::vector<double>({10.0, 5.0}));
    assert(p2->get_luggage_weights() == std::vector<double>({30.0, 10.0}));

    FirstClassPassenger *p3 = new FirstClassPassenger({20.0, 20.0}, {50.0, 40.0});
    assert(p3->get_class_type() == PassengerClassType::FIRST_CLASS);
    assert(p3->get_class_type_string() == "FIRST CLASS");
    assert(p3->get_total_hand_luggage_weight() == 40.0);
    assert(p3->get_total_luggage_weight() == 90.0);
    assert(p3->get_hand_luggage_weights() == std::vector<double>({20.0, 20.0}));
    assert(p3->get_luggage_weights() == std::vector<double>({50.0, 40.0}));


    EconomyPassenger *p4 = new EconomyPassenger({}, {});
    assert(p4->get_total_hand_luggage_weight() == 0.0);
    assert(p4->get_total_luggage_weight() == 0.0);
    assert(p4->get_hand_luggage_weights().empty());
    assert(p4->get_luggage_weights().empty());

    delete p1;
    delete p2;
    delete p3;
    delete p4;

    std::cout << "All tests passed!" << std::endl;

    return 0;
}