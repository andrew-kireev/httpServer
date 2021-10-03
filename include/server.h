//
// Created by Andrew Kireev on 25.09.2021.
//

#ifndef HTTPSERVER_SERVER_H
#define HTTPSERVER_SERVER_H

#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include "thread_pool.h"
#include "handler.h"

class Server {
public:
	Server(int port, int thread_amount) : thread_pool_(thread_amount), port_(port) {}

	int Open() {
		struct sockaddr_in addr{};

		addr.sin_family = AF_INET;
		addr.sin_port = htons(port_);
		addr.sin_addr.s_addr = INADDR_ANY;

		server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
		if (server_socket_ == -1) {
			std::cerr << "error creating server socket" << std::endl;
			return -1;
		}

		if (bind(server_socket_, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
			std::cerr << "error binding socket" << std::endl;
			return -1;
		}
		if (listen(server_socket_, SOMAXCONN) < 0) {
			std::cerr << "error listing socket" << std::endl;
			return -1;
		}


		return 0;
	}

	int Start() {
		while (true) {
			int client_socket = ::accept(server_socket_, nullptr, nullptr);

			if (client_socket < 0) {
				std::cerr << "Ошибка соединения" << std::endl;
				return -1;
			}

			thread_pool_.add_task([client_socket] {
				Handler handler;
				handler.Handle(client_socket);
			});

		}
		return 0;
	}

private:
	ThreadPool thread_pool_;
	int server_socket_ = -1;
	int port_;
};

#endif //HTTPSERVER_SERVER_H
