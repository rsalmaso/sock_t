/*
 * Copyright (C) 2002-2011, Salmaso Raffaele <raffaele@salmaso.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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

