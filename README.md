# a simple http server written in c++  

* responds to basic GET and POST requests  
* all clients run on separate detached threads
* no external libraries used  
* runs on port 4221, currently only locally

## how to build :-  
```zsh
g++ helpers.cpp server.cpp -o server
```

## how to run :-
```zsh
./server
```  

## send GET requests for a file :-
```zsh
⚡ curl -i http://localhost:4221/files/foo
HTTP/1.1 200 OK
Content-Length: 34
Content-Type: application/octet-stream

HI IAM FOO!!!!!
```

## send POST requests for a file :-
```zsh
⚡ curl -i --data "<file-contents>" -H "Content-Type: application/octet-stream" http://localhost:4221/files/bar

HTTP/1.1 201 Created
```

### use testing tools like OHA :-
```zsh
⚡ oha -n 50 http://localhost:4221/echo/foobar
```