// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_DETAIL_MUL_HI_HPP_INCLUDED
#define _TIME_SHIELD_DETAIL_MUL_HI_HPP_INCLUDED

/// \file mul_hi.hpp
/// \brief Helpers for 64-bit multiply-high operations.

#include <cstdint>

#if defined(_MSC_VER)
#   include <intrin.h>
#endif

namespace time_shield {
namespace detail {

    /// \brief Return the high 64 bits of a 64x64-bit multiplication (constexpr variant).
    TIME_SHIELD_CONSTEXPR inline uint64_t mul_hi_u64_constexpr(uint64_t p_a, uint64_t p_b) noexcept {
        const uint64_t a_low = p_a & 0xFFFFFFFFULL;
        const uint64_t a_high = p_a >> 32;
        const uint64_t b_low = p_b & 0xFFFFFFFFULL;
        const uint64_t b_high = p_b >> 32;

        const uint64_t p0 = a_low * b_low;
        const uint64_t p1 = a_low * b_high;
        const uint64_t p2 = a_high * b_low;
        const uint64_t p3 = a_high * b_high;

        const uint64_t carry = ((p0 >> 32) + (p1 & 0xFFFFFFFFULL) + (p2 & 0xFFFFFFFFULL)) >> 32;
        return p3 + (p1 >> 32) + (p2 >> 32) + carry;
    }

    /// \brief Return the high 64 bits of a 64x64-bit multiplication.
    inline uint64_t mul_hi_u64(uint64_t p_a, uint64_t p_b) noexcept {
#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_ARM64))
        uint64_t high = 0;
        (void)_umul128(p_a, p_b, &high);
        return high;
#else
        const __uint128_t product = static_cast<__uint128_t>(p_a) * static_cast<__uint128_t>(p_b);
        return static_cast<uint64_t>(product >> 64);
#endif
    }

    /// \brief Alias for mul_hi_u64 used for shift-by-64 operations.
    inline uint64_t mul_shift_u64(uint64_t p_x, uint64_t p_c) noexcept {
        return mul_hi_u64(p_x, p_c);
    }

    /// \brief Alias for mul_hi_u64_constexpr used for shift-by-64 operations.
    TIME_SHIELD_CONSTEXPR inline uint64_t mul_shift_u64_constexpr(uint64_t p_x, uint64_t p_c) noexcept {
        return mul_hi_u64_constexpr(p_x, p_c);
    }

} // namespace detail
} // namespace time_shield

#endif // _TIME_SHIELD_DETAIL_MUL_HI_HPP_INCLUDED
