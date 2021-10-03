//
// Created by Andrew Kireev on 25.09.2021.
//

#ifndef HTTPSERVER_HANDLER_H
#define HTTPSERVER_HANDLER_H

#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <vector>
#include <sys/socket.h>
#include <sstream>
#include <fstream>
#include "http.h"

const int BUFF_SIZE = 512;

class Handler {
private:
	struct Response {
		std::string status;
		std::string content_type;
		std::string body;
	};

public:
	Handler() {
		buffer.resize(4095);
	}

	static void read_file(std::string &path, std::string &body) {
		std::fstream file(path, std::ios::binary | std::ios::in);
		std::stringstream fl;
		if (file.is_open()) {
			fl << file.rdbuf();
		}
		body = fl.str();
		file.close();
	}

	void Handle(int socket) {
		std::string response = "HTTP/1.1 ";
		std::string contentType;


		ssize_t bytes = ::read(socket, (char *) this->buffer.c_str(), BUFF_SIZE);
		if (bytes < 0) {
			std::cerr << socket;
			close(socket);
			return;
		}


		if (is_not_allowed(this->buffer)) {
			response += STATUS_METHOD_NOT_ALLOWED;
			set_headers(response, contentType, 0);
		} else {
			Response final_response = get_resp(this->buffer);
			response += final_response.status;
			set_headers(response, final_response.content_type, final_response.body.size());
			if (this->buffer.find(METHOD_GET) == 0) {
				response += final_response.body;
			}
		}

//		std::cout << resp << std::endl;

		::write(socket, response.c_str(), response.size());
		close(socket);
	}


	static bool is_not_allowed(std::string &req) {
		if (req.find(METHOD_GET) == std::string::npos &&
			req.find(METHOD_HEAD) == std::string::npos) {
			return true;
		}
		return false;
	}

	static void set_headers(std::string &response, std::string &content_type, int body_size) {
		time_t now = time(nullptr);
		response += "Connection: close\r\n";
		response += "Server: http_server\r\n";
		response += "Date: " + std::string(ctime(&now));

		if (!content_type.empty()) {
			response += "Content-Type: " + content_type + "\r\n";
		}
		if (body_size) {
			response += "Content-Length: " + std::to_string(body_size) + "\r\n";
		}
		response += "\r\n";
	}

	static std::string url_decode(std::string &url) {
//		std::cout << "url " << url << std::endl;
		std::string new_url;
		char ch;
		int j;
//		std::cout << url << std::endl;
		for (auto i = 0; i < url.size(); i++) {
			if (url[i] == '%') {
				sscanf(url.substr(i + 1, 2).c_str(), "%x", &j);
				ch = static_cast<char>(j);
				new_url += ch;
				i = i + 2;
			} else {
				new_url += url[i];
			}
		}
//		std::cout << "url after " << ret << std::endl;
		return new_url;
	}

	static Response get_resp(std::string &req) {
		Response response;
		bool is_forbidden = false;
		std::string path = get_path(req, is_forbidden);
		std::cout << "path: " << path << std::endl;
		std::filesystem::path file = path;

		if (is_forbidden) {
			response.status = STATUS_FORBIDDEN;
			return response;
		}

		if (path[path.size() - 1] == '/') {
			response.status = STATUS_NOT_FOUND;
			return response;
		}

		read_file(path, response.body);
		if (response.body.empty()) {
			response.status = STATUS_NOT_FOUND;
			response.content_type = "";
		} else {
			response.status = STATUS_OK;
		}

		response.content_type = get_type(file.extension());
		if (response.content_type.empty()) {
			response.status = STATUS_FORBIDDEN;
			response.body.clear();
		}

		return response;
	}

	static std::string get_path(std::string &req, bool &is_forbidden) {
		auto file_path_begin = req.find(' ') + 1;
		std::cout << file_path_begin << std::endl;
		std::string path = "..";
		auto file_path_end = req.find(' ', file_path_begin);
		if (file_path_end > req.find('?', file_path_begin)) {
			file_path_end = req.find('?', file_path_begin);
		}
		for (; file_path_begin < file_path_end; file_path_begin++) {
			path += req[file_path_begin];
		}

		path = url_decode(path);

		if (path[path.size() - 1] == '/') {
			std::filesystem::path file = path;
			if (std::filesystem::is_directory(file)) {
				bool no_files = true;

				for (auto const &dir_entry: std::filesystem::directory_iterator{file}) {
					std::string pth = dir_entry.path();
					std::string file_path = std::string(pth.begin() + pth.find_last_of("/") + 1, pth.end());
					if (!dir_entry.is_directory() && file_path == "index.html") {
						no_files = false;
					}
				}

				if (no_files) {
					is_forbidden = true;
				}

				return path + "index.html";
			}
			return path + "index.html";
		}
		return path;
	}

	static std::string get_type(const std::string &extension) {
		if (extension == ".txt")
			return "text/txt";
		else if (extension == ".css")
			return "text/css";
		else if (extension == ".html")
			return "text/html";
		else if (extension == ".js")
			return "application/javascript";
		else if (extension == ".jpeg" || extension == ".jpg")
			return "image/jpeg";
		else if (extension == ".png")
			return "image/png";
		else if (extension == ".gif")
			return "image/gif";
		else if (extension == ".swf")
			return "application/x-shockwave-flash";
		return "";
	}

private:
	std::string buffer;
};

#endif //HTTPSERVER_HANDLER_H
