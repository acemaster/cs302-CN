#Thread server
Server and Client
```
gcc server.c -o server
gcc service.c -o service
gcc client.c -o client
./server /tmp/socket 8000
./service /tmp/socket 
./client 8000
```
