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

/*
 * Used only by tcp_socket::accept
 */
socket_impl::socket_impl(socket_impl* impl,
                         dy::socket_t fd,
                         socket_addr& addr) :
    fd(fd),
    family(impl->family),
    ref_count(1),
    addr(addr)
{
}

/*
 * The real implementation. It opens the file descriptor
 */
socket_impl::socket_impl(dy::family::type family) :
    fd(0),
    family(family),
    ref_count(1),
    addr()
{
    int f;

    switch (family) {
        case dy::family::tcp:
            f = SOCK_STREAM;
            break;
        case dy::family::udp:
            f = SOCK_DGRAM;
            break;
        case dy::family::raw:
            f = SOCK_RAW;
            break;
        default:
            throw family_exception();
    }

    this->fd = ::socket(AF_INET, f, 0);
}

/*
 * Ensure to close the file descriptor
 */
socket_impl::~socket_impl() {
    ::close(this->fd);
}

void socket_impl::ref() {
    this->ref_count += 1;
}

void socket_impl::unref() {
    this->ref_count -= 1;
    if (this->ref_count < 1) {
        delete this;
    }
}

} // namespace dy

