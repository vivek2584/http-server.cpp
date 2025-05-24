#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

struct HTTP_REQUEST{
  std::string http_method;
  std::string request_target;
  std::string http_version;
  std::unordered_map<std::string, std::string> headers;
  std::string request_body;
};

struct HTTP_RESPONSE{
  std::string http_version;
  int status_code;
  std::string reason_phrase;
  std::unordered_map<std::string, std::string> headers;
  std::string response_body;

  HTTP_RESPONSE() : http_version(""), status_code(0),
                    reason_phrase(""), headers({}), response_body(""){}

  HTTP_RESPONSE(const std::string& http_version, int status_code, const std::string& reason_phrase, 
                const std::unordered_map<std::string, std::string>& headers, const std::string& response_body) : 
                http_version(http_version), status_code(status_code), 
                reason_phrase(reason_phrase), headers(headers), response_body(response_body){}
};


std::string create_http_response(const HTTP_RESPONSE& response);
HTTP_REQUEST parse_request(const std::string& request);
HTTP_RESPONSE parse_response(const HTTP_REQUEST& parsed_request);

#endif