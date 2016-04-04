gcc server.c -lpthread -o server
gcc service.c -o backup
gcc client.c -o client