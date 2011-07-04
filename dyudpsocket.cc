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

