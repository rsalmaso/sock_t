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

#include <dysocket.h>
#include <iostream>

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

