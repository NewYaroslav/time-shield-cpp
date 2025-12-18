// SPDX-License-Identifier: MIT
#pragma once
#ifndef TIME_SHIELD_UDP_TRANSPORT_HPP_INCLUDED
#define TIME_SHIELD_UDP_TRANSPORT_HPP_INCLUDED

#include "../config.hpp"

#include <cstddef>
#include <string>

namespace time_shield {
namespace detail {

    struct UdpRequest {
        std::string host;
        int port = 123;
        const void* send_data = nullptr;
        std::size_t send_size = 0;
        void* recv_data = nullptr;
        std::size_t recv_size = 0;
        int timeout_ms = 5000;
    };

    class IUdpTransport {
    public:
        virtual ~IUdpTransport() {}
        virtual bool transact(const UdpRequest& req, int& out_error_code) noexcept = 0;
    };

} // namespace detail
} // namespace time_shield

#endif // TIME_SHIELD_UDP_TRANSPORT_HPP_INCLUDED
