// SPDX-License-Identifier: MIT
#pragma once
#ifndef _TIME_SHIELD_UDP_TRANSPORT_WIN_HPP_INCLUDED
#define _TIME_SHIELD_UDP_TRANSPORT_WIN_HPP_INCLUDED

#if TIME_SHIELD_PLATFORM_WINDOWS

#include "wsa_guard.hpp"
#include "udp_transport.hpp"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>

namespace time_shield {
namespace detail {

    /// \brief Windows UDP transport for NTP queries.
    class UdpTransportWin : public IUdpTransport {
    public:
        /// \brief Send request and receive response over UDP.
        bool transact(const UdpRequest& req, int& out_error_code) noexcept override {
            out_error_code = 0;
            if (!WsaGuard::instance().success()) {
                out_error_code = WsaGuard::instance().ret_code();
                return false;
            }

            SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            if (sock == INVALID_SOCKET) {
                out_error_code = WSAGetLastError();
                return false;
            }

            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_port = htons(static_cast<u_short>(req.port));

            addrinfo hints{}, *res = nullptr;
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_DGRAM;
            hints.ai_protocol = IPPROTO_UDP;

            if (getaddrinfo(req.host.c_str(), nullptr, &hints, &res) != 0 || !res) {
                out_error_code = WSAGetLastError();
                closesocket(sock);
                return false;
            }
            addr.sin_addr = reinterpret_cast<sockaddr_in*>(res->ai_addr)->sin_addr;

            const int timeout_ms = req.timeout_ms > 0 ? req.timeout_ms : 5000;
            DWORD timeout = static_cast<DWORD>(timeout_ms);
            setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&timeout), sizeof(timeout));

            const int send_res = sendto(sock,
                                        static_cast<const char*>(req.send_data),
                                        static_cast<int>(req.send_size),
                                        0,
                                        reinterpret_cast<sockaddr*>(&addr),
                                        sizeof(addr));
            if (send_res == SOCKET_ERROR || static_cast<std::size_t>(send_res) != req.send_size) {
                out_error_code = WSAGetLastError();
                freeaddrinfo(res);
                closesocket(sock);
                return false;
            }

            sockaddr_in from{};
            int from_len = sizeof(from);
            const int recv_res = recvfrom(sock,
                                          static_cast<char*>(req.recv_data),
                                          static_cast<int>(req.recv_size),
                                          0,
                                          reinterpret_cast<sockaddr*>(&from),
                                          &from_len);

            if (recv_res == SOCKET_ERROR || static_cast<std::size_t>(recv_res) != req.recv_size) {
                out_error_code = WSAGetLastError();
                freeaddrinfo(res);
                closesocket(sock);
                return false;
            }

            freeaddrinfo(res);
            closesocket(sock);
            return true;
        }
    };

} // namespace detail
} // namespace time_shield

#endif // _TIME_SHIELD_PLATFORM_WINDOWS

#endif // _TIME_SHIELD_UDP_TRANSPORT_WIN_HPP_INCLUDED
