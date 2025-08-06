CC = gcc
CFLAGS = -Wall -Wextra -g

all: server client

server: server.c shared.h
	$(CC) $(CFLAGS) -o server server.c

client: client.c shared.h
	$(CC) $(CFLAGS) -o client client.c

run: all
	@echo "Launching server and client in separate Windows Terminal tabs..."
	wt.exe new-tab --title "Server" wsl bash -c "./server; exec bash"
	wt.exe new-tab --title "Client" wsl bash -c "./client; exec bash"


clean:
	rm -f server client
