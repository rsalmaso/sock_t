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

