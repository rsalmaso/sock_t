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

