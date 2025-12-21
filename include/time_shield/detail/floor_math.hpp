// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_DETAIL_FLOOR_MATH_HPP_INCLUDED
#define _TIME_SHIELD_DETAIL_FLOOR_MATH_HPP_INCLUDED

/// \file floor_math.hpp
/// \brief Floor division and modulus helpers.

namespace time_shield {
namespace detail {

    /// \brief Floor division for positive divisor.
    template<class T>
    constexpr T floor_div(T a, T b) noexcept {
        T q = a / b;
        T r = a % b;
        if (r != 0 && a < 0) --q;
        return q;
    }

    /// \brief Floor-mod for positive modulus (returns r in [0..b)).
    template<class T>
    constexpr T floor_mod(T a, T b) noexcept {
        T r = a % b;
        if (r < 0) r += b;
        return r;
    }

} // namespace detail
} // namespace time_shield

#endif // _TIME_SHIELD_DETAIL_FLOOR_MATH_HPP_INCLUDED
