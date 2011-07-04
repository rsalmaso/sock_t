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

#ifndef DY_SOCKET_H
#define DY_SOCKET_H

#include <string>
#include <utility>

namespace dy {
    namespace family {
        enum type {
            tcp, /* -> SOCK_STREAM */
            udp, /* -> SOCK_DGRAM */
            raw  /* -> SOCK_RAW */
        };
    } // namespace type
    namespace shutdown_mode {
        enum type {
            read,
            write,
            read_write
        };
    } // namespace suhtdown_mode
    /*See RFC1700 */
    namespace service {
        enum type {
            echo = 7, /* Echo service */
            discard = 9, /* Discard transmissions service */
            systat = 11, /* System status service */
            daytime = 13, /* Time of day service */
            netstat = 15, /* Network status service */
            qotd = 17, /* Quote of the day */
            ftpdata = 20, /* File Transfer Protocol (Default Data) */
            ftp = 21, /* File Tranfer Protocol (Control) */
            ssh = 22, /* SSH Remote Login Protocol */
            telnet = 23, /* Telnet Protocol */
            smtp = 25, /* Simple Mail Transfer Protocol */
            timeserver = 37, /* Timeserver service */
            nameserver = 42, /* Domain Name Service */
            whois = 43, /* Internet whois service */
            bootps = 67, /* DHCP | Bootstrap Protocol Server */
            bootpc = 68, /* Bootstrap Protocol Client */
            tftp = 69, /* Trivial File Transfer Protocol */
            gopher = 70, /* Gopher */
            finger = 79, /* Finger service */
            hhtp = 80, /* Http */
            ttylink = 87, /*  */
            kerberos = 88, /* Kerberos authentication */
            npp = 92, /* Network Printing Protocol */
            rpc = 111, /* SUN Remote Procedure Call */
            sftp = 115, /* Simple File Transfer Protocol */
            nntp = 119, /* Network News Transfer Protocol Usenet */
            ntp = 123, /* Network Time Protocol */
            netbios_ns = 137, /* NETBIOS Name Service nbns */
            netbios_dgm = 138, /* NETBIOS Datagram Service */
            netbios_ssn = 139, /* NETBIOS Session Service */
            ipp = 631 /* Internet Printing Protocol (CUPS) */
        };
    } // namespace service

    class socket_exception {};
    class domain_exception : public socket_exception {};
    class no_ipv6_implementation_exception : public domain_exception {};
    class family_exception : public socket_exception {};
    class socket_addr_exception : public socket_exception {};
    class invalid_ip_exception : public socket_addr_exception {};
    class invalid_port_range_exception : public socket_addr_exception {};
    class shutdown_mode_exception : public socket_exception {};
    class udp_exception : public socket_exception {};
    class udp_write_exception : public udp_exception {};

    /** TCP Socket
     *
     * A socket class, which implements an connection stream.
     *
     * It is implemented as a concrete class, not meant to be derived from.
     * But it can be copied all around without worry (no pointer semantic).
     * It hides the complexity of address initialization and manipulation.
     */
    class tcp_socket {
        struct socket_impl* impl;
        tcp_socket(struct socket_impl* impl);

    public:
        /* for copying */
        tcp_socket(const tcp_socket& s);
        /**
         * Create a new tcp socket.
         */
        tcp_socket();
        ~tcp_socket();

        /** Initiate a connection on a socket.
         * Generally, connection based protocol sockets may successfully connect only once
         * (tipically dy::family::tcp); connectionless protocol sockets may use connect
         * multiple times to change their association.
         *
         * @param ip The host address to connect
         * @param port The port to connect
         * @return If the connection succeeds returns dtrue
         */
		bool connect(const std::string& ip, int port);

        /**
         * Close the active connection.
         */
		void disconnect();

        /** Shutdown part of a full-duplex connection.
         * The tcp_socket::shutdown causes all or part of a full-duplex connection on the
         * socket. The shutdown mode can be:
         * <li> dy::shutdown_mode::read further receptions will be disallowed</li>
         * <li> dy::shutdown_mode::write further transmissions will be disallowed</li>
         * <li> dy::shutdown_mode::read_write further receptions and transmissions will be disallowed</li>
         * It is important to note that tcp_socket::shutdown() doesn't actually close the socket; it
         * just changes its usability.
         *
         * @param mode In which way to shutdown the connection
         * @return It it succeeds or not
         */
		bool shutdown(dy::shutdown_mode::type mode);

        /** Assigning a name to a socket.
         * When a socket is created with socket(), it exists in a namespace but has no name
         * assigned. It is normally necessary to assign a name to a dy::domain::tcp before it can
         * receive connections (with socket::accept()).
         * 
         * @param ip Use this address for this socket
         * @param port 
         * @return Whenever it succeeds or not
         */
        bool bind(const std::string& ip, int port);

        /** Listen a socket for a connection.
         * This method waits for incoming connections.
         *
         * The correct use of this method is with other methods:
         *  ... create a new socket...
         *  s.bind(...);
         *  s.listen(...);
         *  ...
         *  s.accept(...);
         *
         * @param backlog The number of connections allowed on the incoming queue
         * @return If there is an error or not on listening
         */
		bool listen(int backlog = 1);

        /** Accept a remote connection.
         * When someone try to connect to a port, the connection will be queued up waiting to be accepted.
         * Calling socket::accept() will be accepted the connection, and a new socket socket will
         * be returned:
         *  the original one still listening on original port
         *  the new one ready to be used by the application
         * 
         * <code>
         * #include <dysocket.h>
         *
         * static const int BACKLOG = 10;
         *
         * void func() {
         *  socket server;
         *  socket remote_host;
         *
         *  server.bind();
         *  server.listen(BACKLOG);
         *
         *  remote_host = server.accept();
         * }
         * </code>
         * 
         * @return 
         */
		dy::tcp_socket accept();

        /** 
         * 
         * @param data 
         * @return 
         */
		int write(const std::string& data);
        /** 
         * 
         * @param buffersize 
         * @return 
         */
		std::string read(int buffersize = 1024);
        
        /**
         * The associated address of this socket
         */
        std::string ip() const;
        /**
         * The associated port of this socket
         */
        int port() const;
        /**
         * Access the underling socket descriptor, if it is needed by some
         * non-wrapped function.
         */
         int fd() const;
    };

    /** An UDP Socket.
     *
     * Implement a connectionless socket.
     *
     * It is implemented as a concrete class, not meant to be derived from.
     * But it can be copied all around without worry (no pointer semantic).
     * It hides the complexity of address initialization and manipulation.
     */
    class udp_socket {
        struct socket_impl* impl;

    public:
        udp_socket(const udp_socket& s);
        /**
         * Create a new udp socket.
         */
        udp_socket();
        ~udp_socket();

        /** Assigning a name to a socket.
         * When a socket is created with socket(), it exists in a namespace but has no name
         * assigned. It is normally necessary to assign a name to a dy::domain::tcp before it can
         * receive connections (with socket::accept()).
         * 
         * @param ip Use this address for this socket
         * @param port 
         * @return Whenever it succeeds or not
         */
        bool bind(const std::string& ip, int port);
        /**
         * for tcp connections the ip & port pair is ignored
         * If the domain socket is ipv4, the ip string must be in the standard
         * IPv4 dotted-decimal form:
         *
         *  ddd.ddd.ddd.ddd
         *
         * where ddd is a one to three digit decimal number between 0 and 255
         * socket_addr does not accept other formats (such as the octal numbers,
         * hexadecimal numbers, and fewer than four numbers that inet_addr() accepts).
         *
         * @param ip A string 
         * @param port The port, must be 0 < port < 65355, otherwise it throw an exception
         * @param data 
         * @return -1 on error, 
         */
		int write(const std::string& ip, int port, const std::string& data);
		int write(const std::string& data);

        /** 
         * @param buffersize 
         * @return 
         */
		std::string read(int buffersize = 1024);

        /**
         * The associated address of last read connection.
         */
        std::string ip() const;
        /**
         * The associated port of last read connection.
         */
        int port() const;
        /**
         * Access the underling socket descriptor, if it is needed by some
         * non-wrapped function.
         */
         int fd() const;
    };

    /**
     * Get the IP address corresponding to a given Internet host name
     *
     * @param ip An internet host name
     * @return Returns a pair with the official name of host and the 
     * first address of list of addresses from name server
     */
    std::pair<std::string, std::string> gethostbyname(const std::string& ip);

} // namespace dy

#endif // DY_SOCKET_H

