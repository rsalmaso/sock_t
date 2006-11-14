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

#include "dysocket.h"
#include <cstdlib>
#include <unistd.h>

#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

namespace dy {
typedef int socket_t;

/*
 * Socket Address class
 * Wraps in a convenient way the sockaddr* interface.
 * It is implemented as a primitive type, so it can be copied without worrying.
 */
struct socket_addr {
    struct sockaddr_in ipv4;
    ::socklen_t size;
    struct ::sockaddr* addr;

    socket_addr(const socket_addr& saddr);
    socket_addr();
    socket_addr(const std::string& ip, int port);
    ~socket_addr();
};

/*
 * This is the real socket implementation. It can be shared with several *_socket
 * thanks the reference count mechanism, so *_socket can be uses as concrete types.
 *
 * The ref_count start as 1.
 */
struct socket_impl {
    dy::socket_t fd;
    dy::family::type family;
    int ref_count;
    socket_addr addr;

    socket_impl(socket_impl* impl,
                dy::socket_t fd,
                socket_addr& addr);
    socket_impl(dy::family::type type);
    ~socket_impl();
    void ref();
    void unref();
};

/*
 * check if port is in the correct range
 */
inline static bool check_port(int port) {
    return ((port > 0) && (port < 65355));
}

/*
 * Used for *::read methods. We are sure that the buffer is destroyed when
 * method goes out of scope. Don't use the VLA offered by gcc.
 */
class buffer {
    char* data;
public:
    explicit buffer(int size) throw(): data(new char[size]) {}
    ~buffer() { delete[] data; }
    char* operator *() const throw() {
        return data;
    }
};

} // namespace dy

