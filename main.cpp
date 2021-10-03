//
// Created by Andrew Kireev on 25.09.2021.
//

#include "include/server.h"

int main() {
	Server server = Server(80);

	int err = server.Open();
	if (err != 0) {
		return -1;
	}
	server.Start();

	return 0;
}