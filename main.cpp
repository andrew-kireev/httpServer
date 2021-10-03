//
// Created by Andrew Kireev on 25.09.2021.
//

#include "include/server.h"

const char *config_path = "/app/httpd.conf";


void read_config(int &thread_limit, int &port, std::string &dir) {
//	std::string file_path = std::filesystem::current_path();
//	file_path += std::string(config_path);
	std::cout << config_path << std::endl;
	std::fstream file(config_path,
					  std::ios::binary | std::ios::in);
	if (file.is_open()) {
		std::string line;
		getline(file, line);
		thread_limit = std::atoi(std::string(line.begin() + line.find(" ") + 1, line.end()).c_str());
		line.clear();
		getline(file, line);
		port = std::atoi(std::string(line.begin() + line.find(" ") + 1, line.end()).c_str());
		line.clear();
		getline(file, line);
		dir = std::string(line.begin() + line.find(" ") + 1, line.end());
	}
}

int main() {
	int thread_limit = 0, port = 0;
	std::string dir;
	read_config(thread_limit, port, dir);
	std::cout << thread_limit << " " << port << " " << dir;

	if (thread_limit == 0 || port == 0) {
		thread_limit = 256;
		port = 80;
	}
	Server server = Server(port, thread_limit);

	int err = server.Open();
	if (err != 0) {
		return -1;
	}
	server.Start();

	return 0;
}