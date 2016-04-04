gcc server.c -o server
gcc service.c -lpthread -o service
gcc client.c -o client