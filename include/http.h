//
// Created by Andrew Kireev on 03.10.2021.
//

#ifndef HTTPSERVER_HTTP_H
#define HTTPSERVER_HTTP_H

const char* METHOD_GET = "GET";
const char* METHOD_HEAD = "HEAD";

const char* STATUS_METHOD_NOT_ALLOWED = "405 Method Not Allowed\r\n";
const char* STATUS_NOT_FOUND = "404 Not Found\r\n";
const char* STATUS_FORBIDDEN = "403 Forbidden\r\n";
const char* STATUS_OK = "200 OK\r\n";



#endif //HTTPSERVER_HTTP_H
