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

