// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_UDP_TRANSPORT_HPP_INCLUDED
#define _TIME_SHIELD_UDP_TRANSPORT_HPP_INCLUDED

#include <cstddef>
#include <string>

namespace time_shield {
namespace detail {

    /// \brief UDP request parameters for NTP transactions.
    struct UdpRequest {
        std::string host;                ///< Target host name or IP address.
        int         port = 123;          ///< Target port.
        const void* send_data = nullptr; ///< Pointer to outgoing payload.
        std::size_t send_size = 0;       ///< Outgoing payload size in bytes.
        void*       recv_data = nullptr; ///< Pointer to receive buffer.
        std::size_t recv_size = 0;       ///< Receive buffer size in bytes.
        int         timeout_ms = 5000;   ///< Receive timeout in milliseconds.
    };

    /// \brief Abstract UDP transport interface for NTP queries.
    class IUdpTransport {
    public:
        /// \brief Virtual destructor.
        virtual ~IUdpTransport() {}
        /// \brief Send request and receive response over UDP.
        virtual bool transact(const UdpRequest& req, int& out_error_code) noexcept = 0;
    };

} // namespace detail
} // namespace time_shield

#endif // _TIME_SHIELD_UDP_TRANSPORT_HPP_INCLUDED
