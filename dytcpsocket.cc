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

/* for tcp_socket::accept
 * @param impl Must not be NULL otherwise it will BANG!
 */
tcp_socket::tcp_socket(socket_impl* impl) :
    impl(impl)
{
    this->impl->ref();
}

/**
 * Copy constructor
 */
tcp_socket::tcp_socket(const tcp_socket& s) :
    impl(s.impl)
{
    this->impl->ref();
}

tcp_socket::tcp_socket() :
    impl(new socket_impl(dy::family::tcp))
{
}

/*
 */
tcp_socket::~tcp_socket() {
    this->impl->unref();
}

bool tcp_socket::connect(const std::string& ip, int port) {
    socket_addr addr(ip, port);
	return !(::connect(this->impl->fd, addr.addr, addr.size) == -1);
}

void tcp_socket::disconnect() {
    ::close(this->impl->fd);
}

bool tcp_socket::shutdown(dy::shutdown_mode::type mode) {
	switch (mode) {
		case dy::shutdown_mode::read:
			return ::shutdown(this->impl->fd, 0) == 0;
			break;
		case dy::shutdown_mode::write:
			return ::shutdown(this->impl->fd, 1) == 0;
			break;
		case dy::shutdown_mode::read_write:
			return ::shutdown(this->impl->fd, 2) == 0;
			break;
		default:
            throw shutdown_mode_exception();
	}
    return false; // to make compiler happy, should never reach there
}

bool tcp_socket::bind(const std::string& ip, int port) {
    this->impl->addr = socket_addr(ip, port);
	return !(::bind(this->impl->fd, this->impl->addr.addr, this->impl->addr.size) <0);
}

bool tcp_socket::listen(int backlog) {
	return (::listen(this->impl->fd, backlog) == 0);
}

tcp_socket tcp_socket::accept() {
    socket_addr addr;
	int newfd = ::accept(this->impl->fd, addr.addr, &(addr.size));
    return tcp_socket(new socket_impl(this->impl, newfd, addr));
}

int tcp_socket::write(const std::string& data) /*, dint flags*/{
	return ::write(this->impl->fd, data.c_str(), data.size());
}

std::string tcp_socket::read(int buffersize) {
    dy::buffer buf(buffersize);
    int rec = ::read(this->impl->fd, *buf, buffersize);
    return std::string(*buf, (rec > -1) ? rec : 0);
}

std::string tcp_socket::ip() const {
    return ::inet_ntoa(this->impl->addr.ipv4.sin_addr);
}

int tcp_socket::port() const {
    return ntohs(this->impl->addr.ipv4.sin_port);
}

int tcp_socket::fd() const {
    return this->impl->fd;
}

} // namespace dy

