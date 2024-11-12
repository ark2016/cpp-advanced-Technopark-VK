#include "units/plane.hpp"
#include "units/crew_members.hpp"
#include "units/passengers.hpp"

#include <iostream>
#include <sstream>
#include <vector>

int main() {
    Plane plane;

    // Placeholder variables
    std::string line;
    int line_num = 0;

    // The first 11 entries are special
    while (std::getline(std::cin, line)) {
        if (line.empty())
            continue;
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "FIRST_CLASS_SEGMENT") {
            double max_weight;
            iss >> max_weight;
            plane.set_first_class_segment(new FirstClassSegment(max_weight));
        } else if (type == "BUSINESS_CLASS_SEGMENT") {
            double max_weight;
            iss >> max_weight;
            plane.set_business_class_segment(new BusinessClassSegment(max_weight));
        } else if (type == "ECONOMY_CLASS_SEGMENT") {
            double max_weight;
            iss >> max_weight;
            plane.set_economy_class_segment(new EconomyClassSegment(max_weight));
        } else if (type == "PILOT") {
            Unit *unit = new Pilot();
            plane.add_unit(unit);
        } else if (type == "FLIGHT_ATTENDANT") {
            Unit *unit = new FlightAttendant();
            plane.add_unit(unit);
        } else {
            // Now we handle passengers
            std::vector<double> hand_luggage_weights;
            std::vector<double> luggage_weights;

            double weight;
            if (type == "ECONOMY") {
                // Read up to 1 hand luggage weight and up to 1 luggage weight
                if (iss >> weight) {
                    hand_luggage_weights.push_back(weight);
                }
                if (iss >> weight) {
                    luggage_weights.push_back(weight);
                }
                Unit *passenger = new EconomyPassenger(hand_luggage_weights, luggage_weights);
                plane.add_unit(passenger);
            } else if (type == "BUSINESS") {
                // Read up to 2 hand luggage weights and up to 2 luggage weights
                if (iss >> weight) {
                    hand_luggage_weights.push_back(weight);
                }
                if (iss >> weight) {
                    hand_luggage_weights.push_back(weight);
                }
                if (iss >> weight) {
                    luggage_weights.push_back(weight);
                }
                if (iss >> weight) {
                    luggage_weights.push_back(weight);
                }
                Unit *passenger = new BusinessPassenger(hand_luggage_weights, luggage_weights);
                plane.add_unit(passenger);
            } else if (type == "FIRST_CLASS") {
                // Read up to 2 hand luggage weights and up to 2 luggage weights
                if (iss >> weight) {
                    hand_luggage_weights.push_back(weight);
                }
                if (iss >> weight) {
                    hand_luggage_weights.push_back(weight);
                }
                if (iss >> weight) {
                    luggage_weights.push_back(weight);
                }
                if (iss >> weight) {
                    luggage_weights.push_back(weight);
                }
                Unit *passenger = new FirstClassPassenger(hand_luggage_weights, luggage_weights);
                plane.add_unit(passenger);
            }
        }
        line_num++;
    }

    // Output the total luggage weights if necessary
    // std::cout << "Total luggage weight: " << plane.get_total_luggage_weight() << std::endl;
    // std::cout << "Total hand luggage weight: " << plane.get_total_hand_luggage_weight() << std::endl;

    return 0;
}