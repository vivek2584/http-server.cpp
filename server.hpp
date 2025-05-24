#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <array>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

constexpr const char* CRLF = "\r\n";

struct HTTP_RESPONSE{
  std::string http_version;
  int status_code;
  std::string reason_phrase;
  std::string headers;
  std::string response_body;
};

std::string get_http_response(const HTTP_RESPONSE& response);

#endif