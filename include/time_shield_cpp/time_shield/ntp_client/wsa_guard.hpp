#pragma once
#ifndef _TIME_SHIELD_WSA_GUARD_HPP_INCLUDED
#define _TIME_SHIELD_WSA_GUARD_HPP_INCLUDED

/// \file wsa_guard.hpp
/// \brief Singleton guard for WinSock initialization.
/// \ingroup ntp

#include "../config.hpp"

#if TIME_SHIELD_HAS_WINSOCK
#   include <winsock2.h>  // Must be included before windows.h
#   include <ws2tcpip.h>
#   include <windows.h>   // (optional, but safe if later needed)
#else
#   error "WsaGuard requires WinSock support"
#endif
#include <mutex>
#include <string>

namespace time_shield {

    /// \ingroup ntp
    /// \brief Singleton guard for WinSock initialization.
    class WsaGuard {
    public:
        /// \brief Returns the singleton instance, initializing WSA if needed.
        static const WsaGuard& instance() {
            static WsaGuard instance;
            return instance;
        }

        /// \brief Returns whether WSAStartup was successful.
        bool success() const noexcept {
            return m_ret_code == 0;
        }

        /// \brief Returns the result code from WSAStartup.
        int ret_code() const noexcept {
            return m_ret_code;
        }

        /// \brief Returns the WSAData structure (valid only if successful).
        const WSADATA& data() const noexcept {
            return m_wsa_data;
        }

    private:
        WsaGuard() {
            m_ret_code = WSAStartup(MAKEWORD(2, 2), &m_wsa_data);
        }

        ~WsaGuard() = default;

        WsaGuard(const WsaGuard&) = delete;
        WsaGuard& operator=(const WsaGuard&) = delete;

        WSADATA m_wsa_data{};
        int     m_ret_code = -1;
    };

} // namespace time_shield

#endif // _TIME_SHIELD_WSA_GUARD_HPP_INCLUDED

