all: base

base: server client

server:
	gcc server.c -o server -I include -g -l pthread

client:
	gcc client.c -o client -I include -g -l pthread
