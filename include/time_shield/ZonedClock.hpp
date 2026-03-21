// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_ZONED_CLOCK_HPP_INCLUDED
#define _TIME_SHIELD_ZONED_CLOCK_HPP_INCLUDED

/// \file ZonedClock.hpp
/// \brief Header-only clock wrapper for named zones, fixed offsets, and optional NTP-backed UTC time.

#include "config.hpp"
#include "constants.hpp"
#include "DateTime.hpp"
#include "enums.hpp"
#include "time_parser.hpp"
#include "time_utils.hpp"
#include "time_zone_conversions.hpp"
#include "time_zone_offset_conversions.hpp"

#if TIME_SHIELD_ENABLE_NTP_CLIENT
#   include "ntp_time_service.hpp"
#endif

#include <cstddef>
#include <stdexcept>
#include <string>

namespace time_shield {

    /// \brief Stores a target local-time context backed by a named zone or fixed UTC offset.
    ///
    /// The class resolves the effective offset on demand. Named zones are recalculated
    /// for the requested UTC instant, while numeric offsets remain fixed. Current UTC time
    /// can come from the local realtime clock or from the global NTP service.
    class ZonedClock final {
    public:
        /// \brief Construct UTC fixed-offset clock without NTP.
        ZonedClock() noexcept
            : m_zone(UNKNOWN)
            , m_offset(0)
            , m_is_named_zone(false)
            , m_use_ntp(false) {}

        /// \brief Construct clock for a named zone.
        /// \param zone Supported named zone.
        /// \param use_ntp Use NTP-backed UTC time when true.
        explicit ZonedClock(TimeZone zone, bool use_ntp = false) noexcept
            : m_zone(UNKNOWN)
            , m_offset(0)
            , m_is_named_zone(false)
            , m_use_ntp(use_ntp) {
            set_zone(zone);
        }

        /// \brief Construct clock for a fixed UTC offset.
        /// \param utc_offset Fixed UTC offset in seconds.
        /// \param use_ntp Use NTP-backed UTC time when true.
        /// \throw std::invalid_argument if utc_offset is outside the supported range.
        explicit ZonedClock(tz_t utc_offset, bool use_ntp = false)
            : m_zone(UNKNOWN)
            , m_offset(0)
            , m_is_named_zone(false)
            , m_use_ntp(use_ntp) {
            if (!try_set_offset(utc_offset)) {
                throw std::invalid_argument("Invalid UTC offset");
            }
        }

        /// \brief Try to build fixed-offset clock without throwing.
        /// \param utc_offset Fixed UTC offset in seconds.
        /// \param out Output clock on success.
        /// \return True when the offset is valid and out is updated.
        static bool try_from_offset(tz_t utc_offset, ZonedClock& out) noexcept {
            ZonedClock candidate;
            if (!candidate.try_set_offset(utc_offset)) {
                return false;
            }
            out = candidate;
            return true;
        }

        /// \brief Set the stored named zone.
        /// \param zone Supported named zone. `UNKNOWN` resets the instance to fixed UTC offset `+00:00`.
        void set_zone(TimeZone zone) noexcept {
            if (zone == UNKNOWN) {
                m_zone = UNKNOWN;
                m_offset = 0;
                m_is_named_zone = false;
                return;
            }

            m_zone = zone;
            m_offset = 0;
            m_is_named_zone = true;
        }

        /// \brief Set the stored fixed UTC offset.
        /// \param utc_offset Fixed UTC offset in seconds.
        /// \return True when the offset is valid.
        bool try_set_offset(tz_t utc_offset) noexcept {
            if (!is_valid_tz_offset(utc_offset)) {
                return false;
            }

            m_zone = UNKNOWN;
            m_offset = utc_offset;
            m_is_named_zone = false;
            return true;
        }

        /// \brief Parse and set a named zone or numeric offset from string.
        /// \param zone_spec Input string with ASCII trimming applied before parsing.
        /// \return True when parsing succeeds.
        bool try_set_zone(const std::string& zone_spec) noexcept {
            const std::string trimmed = trim_ascii(zone_spec);
            if (trimmed.empty()) {
                return false;
            }

            TimeZone parsed_zone = UNKNOWN;
            if (parse_time_zone_name(trimmed, parsed_zone)) {
                set_zone(parsed_zone);
                return true;
            }

            TimeZoneStruct parsed_offset = create_time_zone_struct(0, 0, true);
            if (!parse_time_zone(trimmed, parsed_offset)) {
                return false;
            }

            return try_set_offset(time_zone_struct_to_offset(parsed_offset));
        }

        /// \brief Set preferred UTC source.
        /// \param use_ntp Use NTP-backed UTC time when true.
        void set_use_ntp(bool use_ntp) noexcept {
            m_use_ntp = use_ntp;
        }

        /// \brief Return true when the instance stores a named zone.
        bool has_named_zone() const noexcept {
            return m_is_named_zone;
        }

        /// \brief Return stored named zone or `UNKNOWN` for fixed-offset mode.
        TimeZone zone() const noexcept {
            return m_is_named_zone ? m_zone : UNKNOWN;
        }

        /// \brief Return the preferred UTC source flag.
        bool use_ntp() const noexcept {
            return m_use_ntp;
        }

        /// \brief Return true when the global NTP service is active for this clock.
        bool ntp_active() const noexcept {
#if TIME_SHIELD_ENABLE_NTP_CLIENT
            return m_use_ntp && NtpTimeService::instance().running();
#else
            return false;
#endif
        }

        /// \brief Return effective UTC offset in seconds for the current UTC instant.
        tz_t offset_now() const noexcept {
            return offset_at_utc_ms(current_utc_ms());
        }

        /// \brief Return effective UTC offset in seconds for a specific UTC instant.
        /// \param utc_ms UTC timestamp in milliseconds.
        /// \return Effective UTC offset in seconds.
        tz_t offset_at_utc_ms(ts_ms_t utc_ms) const noexcept {
            if (!m_is_named_zone) {
                return m_offset;
            }

            const ts_ms_t local_ms = gmt_to_zone_ms(utc_ms, m_zone);
            if (local_ms == ERROR_TIMESTAMP) {
                return 0;
            }

            const ts_ms_t delta_ms = local_ms - utc_ms;
            return static_cast<tz_t>(delta_ms / MS_PER_SEC);
        }

        /// \brief Return current UTC time in seconds.
        ts_t utc_time_sec() const noexcept {
            return static_cast<ts_t>(current_utc_us() / US_PER_SEC);
        }

        /// \brief Return current UTC time in milliseconds.
        ts_ms_t utc_time_ms() const noexcept {
            return current_utc_ms();
        }

        /// \brief Return current UTC time in microseconds.
        ts_us_t utc_time_us() const noexcept {
            return current_utc_us();
        }

        /// \brief Return current local timestamp in seconds.
        ts_t local_time_sec() const noexcept {
            const ts_t utc_sec = utc_time_sec();
            return utc_sec + static_cast<ts_t>(offset_at_utc_ms(static_cast<ts_ms_t>(utc_sec) * MS_PER_SEC));
        }

        /// \brief Return current local timestamp in milliseconds.
        ts_ms_t local_time_ms() const noexcept {
            const ts_ms_t utc_ms = current_utc_ms();
            return utc_ms + static_cast<ts_ms_t>(offset_at_utc_ms(utc_ms)) * MS_PER_SEC;
        }

        /// \brief Return current local timestamp in microseconds.
        ts_us_t local_time_us() const noexcept {
            const ts_us_t utc_us = current_utc_us();
            return utc_us + static_cast<ts_us_t>(offset_at_utc_ms(static_cast<ts_ms_t>(utc_us / MS_PER_SEC))) * US_PER_SEC;
        }

        /// \brief Return current time snapshot with resolved fixed offset.
        DateTime now() const noexcept {
            return from_utc_ms(current_utc_ms());
        }

        /// \brief Return a snapshot for a specific UTC instant in milliseconds.
        /// \param utc_ms UTC timestamp in milliseconds.
        /// \return DateTime snapshot with resolved fixed offset.
        DateTime from_utc_ms(ts_ms_t utc_ms) const noexcept {
            return DateTime::from_unix_ms(utc_ms, offset_at_utc_ms(utc_ms));
        }

        /// \brief Return a snapshot for a specific UTC instant in seconds.
        /// \param utc_s UTC timestamp in seconds.
        /// \return DateTime snapshot with resolved fixed offset.
        DateTime from_utc_s(ts_t utc_s) const noexcept {
            return from_utc_ms(static_cast<ts_ms_t>(utc_s) * MS_PER_SEC);
        }

        /// \brief Return short name of the stored named zone.
        /// \return Zone abbreviation or an empty string in fixed-offset mode.
        std::string zone_name() const {
            return m_is_named_zone ? std::string(to_cstr(m_zone)) : std::string();
        }

        /// \brief Return human-readable zone label.
        /// \return Full zone name for named zones or `UTC+/-HH:MM` for fixed offsets.
        std::string zone_full_name() const {
            if (m_is_named_zone) {
                return to_str(m_zone, FULL_NAME);
            }
            return std::string("UTC") + offset_string_for_offset(m_offset);
        }

        /// \brief Return effective numeric UTC offset as `+HH:MM` or `-HH:MM`.
        std::string offset_string() const {
            return offset_string_for_offset(offset_now());
        }

        /// \brief Return current local time formatted as ISO8601 with offset.
        std::string to_iso8601() const {
            return now().to_iso8601();
        }

        /// \brief Return current UTC time formatted as ISO8601 with `Z`.
        std::string to_iso8601_utc() const {
            return now().to_iso8601_utc();
        }

        /// \brief Format current local time using the custom formatter grammar.
        /// \param fmt Formatting pattern.
        /// \return Formatted string.
        std::string format(const std::string& fmt) const {
            return now().format(fmt);
        }

    private:
        static std::string trim_ascii(const std::string& value) {
            std::size_t begin = 0;
            std::size_t end = value.size();
            while (begin < end && is_ascii_space(value[begin])) {
                ++begin;
            }
            while (end > begin && is_ascii_space(value[end - 1])) {
                --end;
            }
            return value.substr(begin, end - begin);
        }

        static bool is_ascii_space(char ch) noexcept {
            return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '\f' || ch == '\v';
        }

        static std::string offset_string_for_offset(tz_t utc_offset) {
            return time_zone_struct_to_string(to_time_zone_struct(utc_offset));
        }

        ts_ms_t current_utc_ms() const noexcept {
            return static_cast<ts_ms_t>(current_utc_us() / 1000);
        }

        ts_us_t current_utc_us() const noexcept {
#if TIME_SHIELD_ENABLE_NTP_CLIENT
            if (m_use_ntp) {
                if (!NtpTimeService::instance().running()) {
                    (void)ntp::init(30000, true);
                }
                return static_cast<ts_us_t>(ntp::utc_time_us());
            }
#endif
            return static_cast<ts_us_t>(now_realtime_us());
        }

    private:
        TimeZone m_zone;
        tz_t m_offset;
        bool m_is_named_zone;
        bool m_use_ntp;
    };

} // namespace time_shield

#endif // _TIME_SHIELD_ZONED_CLOCK_HPP_INCLUDED
