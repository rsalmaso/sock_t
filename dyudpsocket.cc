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

namespace dy {

udp_socket::udp_socket() :
    impl(new socket_impl(dy::family::udp))
{
}

udp_socket::udp_socket(const udp_socket& s) :
    impl(s.impl)
{
    this->impl->ref();
}

udp_socket::~udp_socket() {
    this->impl->unref();
}

bool udp_socket::bind(const std::string& ip, int port) {
    this->impl->addr = socket_addr(ip, port);
	return !(::bind(this->impl->fd, this->impl->addr.addr, this->impl->addr.size) <0);
}

int udp_socket::write(const std::string& ip, int port, const std::string& data) {
    socket_addr addr(ip, port);
    int ret = ::sendto(this->impl->fd, data.c_str(), data.size(), 0, addr.addr, addr.size);
    if (ret == -1) {
        throw udp_write_exception();
    }
    return ret;
}

int udp_socket::write(const std::string& data) {
    int ret = ::sendto(this->impl->fd, data.c_str(), data.size(), 0, this->impl->addr.addr, this->impl->addr.size);
    if (ret == -1) {
        throw udp_write_exception();
    }
    return ret;
}

std::string udp_socket::read(int buffersize) {
    socket_addr addr;
    dy::buffer buf(buffersize);
    int rec = ::recvfrom(this->impl->fd, *buf, buffersize, 0, addr.addr, &(addr.size));
    this->impl->addr = addr;
    return std::string(*buf, rec);
}

int tcp_socket::fd() const {
    return this->impl->fd;
}

std::string udp_socket::ip() const {
    return ::inet_ntoa(this->impl->addr.ipv4.sin_addr);
}

int udp_socket::port() const {
    return ntohs(this->impl->addr.ipv4.sin_port);
}

} // namespace dy

