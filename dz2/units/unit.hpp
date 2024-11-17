#ifndef UNIT_HPP
#define UNIT_HPP

#include <cstddef>

class Unit {
protected:
    static size_t next_id_;
    size_t id_;

public:
    Unit();

    virtual ~Unit() = default;

    size_t get_id() const;

    virtual double get_total_hand_luggage_weight() const = 0;

    virtual double get_total_luggage_weight() const = 0;
};

#endif // UNIT_HPP