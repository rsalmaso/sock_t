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

#include <dysocket.h>
#include <iostream>
#include <cstdlib>

void echo(dy::tcp_socket s) {
    s.write("Echo server, welcome to welcome to welcome to\n");
    s.write("Write 'quit' or 'exit' to stop the connection\n");
    while (true) {
        std::string buf = s.read();
        std::string::size_type i = buf.find_last_of("\r\n");
        if (i < buf.size()) {
            buf = buf.substr(0, i-1);
        }
        if ((buf == "quit") || (buf == "exit")) {
            s.write("Bybe bye\n");
            std::exit(0);
        } else {
            s.write(buf);
            s.write("\n");
        }
    }
}

int main() {
    dy::tcp_socket server;
    server.bind("localhost", 8080);
    std::cout << "Server ip: " << server.ip() << std::endl;
    std::cout << "Server port: " << server.port() << std::endl;

    server.listen();

    dy::tcp_socket client = server.accept();
    echo(client);

    return 0;
}

