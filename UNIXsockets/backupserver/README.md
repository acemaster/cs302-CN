#Backup server
Server and Client
```
gcc server.c -o server
gcc service.c -o backup
gcc client.c -o client
./server /tmp/socket 8000
./backup /tmp/socket 
./client 8000 //Will work
./client 8000 //Will work
./client 8000 //threshold crossed. Backup server now servicing the previous clients
```
