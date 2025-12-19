// SPDX-License-Identifier: MIT
#pragma once
#ifndef TIME_SHIELD_UDP_TRANSPORT_POSIX_HPP_INCLUDED
#define TIME_SHIELD_UDP_TRANSPORT_POSIX_HPP_INCLUDED

#if TIME_SHIELD_PLATFORM_UNIX

#include "udp_transport.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

namespace time_shield {
namespace detail {

    class UdpTransportPosix : public IUdpTransport {
    public:
        bool transact(const UdpRequest& req, int& out_error_code) noexcept override {
            out_error_code = 0;
            const int sock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            if (sock < 0) {
                out_error_code = errno;
                return false;
            }

            addrinfo hints{}, *res = nullptr;
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_DGRAM;
            hints.ai_protocol = IPPROTO_UDP;

            const int resolve_code = getaddrinfo(req.host.c_str(), nullptr, &hints, &res);
            if (resolve_code != 0 || !res) {
                out_error_code = (resolve_code != 0) ? resolve_code : errno;
                ::close(sock);
                return false;
            }

            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_port = htons(static_cast<uint16_t>(req.port));
            addr.sin_addr = reinterpret_cast<sockaddr_in*>(res->ai_addr)->sin_addr;

            const int timeout_ms = req.timeout_ms > 0 ? req.timeout_ms : 5000;
            timeval tv;
            tv.tv_sec = timeout_ms / 1000;
            tv.tv_usec = (timeout_ms % 1000) * 1000;
            ::setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&tv), sizeof(tv));

            const ssize_t sent = ::sendto(sock,
                                          req.send_data,
                                          req.send_size,
                                          0,
                                          reinterpret_cast<sockaddr*>(&addr),
                                          sizeof(addr));
            if (sent < 0 || static_cast<std::size_t>(sent) != req.send_size) {
                out_error_code = errno;
                freeaddrinfo(res);
                ::close(sock);
                return false;
            }

            sockaddr_in from{};
            socklen_t from_len = sizeof(from);
            const ssize_t received = ::recvfrom(sock,
                                                req.recv_data,
                                                req.recv_size,
                                                0,
                                                reinterpret_cast<sockaddr*>(&from),
                                                &from_len);

            if (received < 0 || static_cast<std::size_t>(received) != req.recv_size) {
                out_error_code = errno;
                freeaddrinfo(res);
                ::close(sock);
                return false;
            }

            freeaddrinfo(res);
            ::close(sock);
            return true;
        }
    };

} // namespace detail
} // namespace time_shield

#endif // TIME_SHIELD_PLATFORM_UNIX

#endif // TIME_SHIELD_UDP_TRANSPORT_POSIX_HPP_INCLUDED
