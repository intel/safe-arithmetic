#pragma once


#include <safe/dsl/add.hpp>
#include <safe/dsl/minus.hpp>
#include <safe/dsl/multiply.hpp>
#include <safe/dsl/divide.hpp>
#include <safe/dsl/modulo.hpp>

#include <safe/dsl/shift_left.hpp>
#include <safe/dsl/shift_right.hpp>
#include <safe/dsl/bitwise_or.hpp>
#include <safe/dsl/bitwise_and.hpp>
#include <safe/dsl/bitwise_xor.hpp>
#include <safe/dsl/bitwise_invert.hpp>

#include <safe/dsl/min.hpp>
#include <safe/dsl/max.hpp>
#include <safe/dsl/abs.hpp>

#include <safe/dsl/is_subset.hpp>
#include <safe/dsl/is_superset.hpp>
#include <safe/dsl/is_equal.hpp>

#include <safe/dsl/union.hpp>
#include <safe/dsl/intersection.hpp>

#include <safe/dsl/ival.hpp>
#include <safe/dsl/set.hpp>
#include <safe/dsl/mask.hpp>

#include <safe/dsl/simplify.hpp>


namespace safe {
    using safe::dsl::ival;
    using safe::dsl::ival_t;

    using safe::dsl::mask;
    using safe::dsl::mask_t;

    using safe::dsl::set;
    using safe::dsl::set_t;

    using safe::dsl::union_t;
    using safe::dsl::intersection_t;
}