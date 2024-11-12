#include "unit.hpp"

size_t Unit::next_id_ = 0;

Unit::Unit() : id_(next_id_++) {
}

size_t Unit::get_id() const {
    return id_;
}