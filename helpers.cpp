#include "server.hpp"

std::string create_http_response(const HTTP_RESPONSE& response){
  std::string header_string = "";
  for(const auto& header : response.headers){
    header_string += header.first + ": " + header.second + "\r\n";
  }
  return  response.http_version + " " 
        + std::to_string(response.status_code) + " "
        + response.reason_phrase + "\r\n"
        + header_string + "\r\n"
        + response.response_body;
}

HTTP_REQUEST parse_request(const std::string& request){
  HTTP_REQUEST parsed_request;
  size_t start = 0;
  size_t end;

  end = request.find(" ", start);
  parsed_request.http_method = request.substr(start, end - start);
  start = end + 1;
  end = request.find(" ", start);
  parsed_request.request_target = request.substr(start, end - start);
  start = end + 1;
  end = request.find("\r\n", start);
  parsed_request.http_version = request.substr(start, end - start);
  start = end + 2;

  while((end = request.find("\r\n", start)) != std::string::npos){
    if(end == start){
      start = end + 2;
      break;
    }
    std::string header = request.substr(start, end - start);
    int i = 0, j = 0;
    j = header.find(": ", i);
    std::string header_key = header.substr(i, j - i);
    i = j + 2;
    std::string header_value = header.substr(i);
    parsed_request.headers.insert({header_key, header_value});
    start = end + 2;
  }

  parsed_request.request_body = request.substr(start);

  return parsed_request;
}

HTTP_RESPONSE parse_response(const HTTP_REQUEST& parsed_request){
  HTTP_RESPONSE parsed_response;
  parsed_response.http_version = "HTTP/1.1";
  
  if(parsed_request.request_target.find("/echo/") == 0)
  {
    parsed_response.status_code = 200;
    parsed_response.reason_phrase = "OK";
    parsed_response.headers.insert({"Content-Type", "text/plain"});
    std::string response_body = parsed_request.request_target.substr(6);
    parsed_response.headers.insert({"Content-Length", std::to_string(response_body.size())});
    parsed_response.response_body = response_body;
  }
  else if(parsed_request.request_target == "/user-agent"){
    parsed_response.status_code = 200;
    parsed_response.reason_phrase = "OK";
    parsed_response.headers.insert({"Content-Type", "text/plain"});
    parsed_response.headers.insert({"Content-Length", std::to_string((parsed_request.headers.find("User-Agent") -> second).size())});
    parsed_response.response_body = parsed_request.headers.find("User-Agent") -> second;
  }
  else if(parsed_request.request_target.find("/files/") == 0){
    std::string file_path = "tmp/" + parsed_request.request_target.substr(7);
    if(!(std::filesystem::exists(file_path))){
      parsed_response.status_code = 404;
      parsed_response.reason_phrase = "Not Found";
      parsed_response.headers = {};
      parsed_response.response_body = "";
    }
    else{
      std::ifstream file(file_path);
      std::ostringstream ss;
      ss << file.rdbuf();
      std::string file_content = ss.str();
      parsed_response.status_code = 200;
      parsed_response.reason_phrase = "OK";
      parsed_response.headers.insert({"Content-Type", "application/octet-stream"});
      parsed_response.headers.insert({"Content-Length", std::to_string(file_content.size())});
      parsed_response.response_body = file_content;
    }
  }
  else if(parsed_request.request_target == "/"){
    parsed_response.status_code = 200;
    parsed_response.reason_phrase = "OK";
    parsed_response.headers = {};
    parsed_response.response_body = "";
  }
  else{
    parsed_response.status_code = 404;
    parsed_response.reason_phrase = "Not Found";
    parsed_response.headers = {};
    parsed_response.response_body = "";
  }

  return parsed_response;
}

void handle_client(int client_fd){
  char recvbuffer[2048];
  ssize_t bytes_recvd = recv(client_fd, recvbuffer, sizeof(recvbuffer) - 1, 0);

  if(bytes_recvd > 0){
    recvbuffer[bytes_recvd] = '\0';
  }

  std::string request(recvbuffer);

  HTTP_REQUEST parsed_request = parse_request(request);

  HTTP_RESPONSE parsed_response = parse_response(parsed_request);

  std::string sent_response = create_http_response(parsed_response);
  //std::cout << sent_response << "\n";
  send(client_fd, sent_response.c_str(), sent_response.size(), 0);
  close(client_fd);
}