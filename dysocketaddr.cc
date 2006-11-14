/*
 * Copyright (C) 2002-2006, Salmaso Raffaele <raffaele@salmaso.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. If the binary program depends on a modified version of this package,
 *     you are encouraged to publicly release the modified version of this
 *     package.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

#include "dysocketprivate.h"
#include <cstdio>

namespace dy {

/** Copy constructor
 */
socket_addr::socket_addr(const socket_addr& saddr) :
    ipv4(),
    size(saddr.size),
    addr((::sockaddr*)(&(this->ipv4)))
{
    this->ipv4.sin_family = AF_INET;
    this->ipv4.sin_port = saddr.ipv4.sin_port;
    this->ipv4.sin_addr.s_addr = saddr.ipv4.sin_addr.s_addr;
    ::memset(&(this->ipv4.sin_zero), '\0', 8);
}

socket_addr::socket_addr() :
    ipv4(),
    size(sizeof(struct sockaddr_in)),
    addr((::sockaddr*)(&(this->ipv4)))
{
}

/*
 * If the domain socket is ipv4, the ip string must be in the standard
 * IPv4 dotted-decimal form:
 *
 *  ddd.ddd.ddd.ddd
 *
 * where ddd is a one to three digit decimal number between 0 and 255
 * socket_addr does not accept other formats (such as the octal numbers,
 * hexadecimal numbers, and fewer than four numbers that inet_addr() accepts).
 *
 * @param ip A string 
 * @param port The port, must be 0 < port < 65355, otherwise it throw an exception
 */
socket_addr::socket_addr(const std::string& ip, int port) :
    ipv4(),
    size(sizeof(struct sockaddr_in)),
    addr((::sockaddr*)(&(this->ipv4)))
{
    if (!check_port(port)) {
        throw invalid_port_range_exception();
    }

    this->ipv4.sin_port = htons(port);

    struct ::hostent* host_info = ::gethostbyname(ip.c_str());
    if (host_info == NULL) {
        this->ipv4.sin_family = AF_INET;
        if (inet_aton(ip.c_str(), &(this->ipv4.sin_addr)) == 0) {
            throw invalid_ip_exception();
        }
        ::memset(&(this->ipv4.sin_zero), '\0', 8);
    } else {
        this->ipv4.sin_family = host_info->h_addrtype;
        ::memcpy((char*)&(this->ipv4.sin_addr), host_info->h_addr, host_info->h_length);
    }
}

socket_addr::~socket_addr() {
}

} // namespace dy

