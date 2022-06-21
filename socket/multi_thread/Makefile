all: server client
OBJ_server=server

OBJS_server += server.c
OBJS_server += wrap.c

cc=gcc

$(OBJ_server):$(OBJS_server)
	$(CC) $^ -o $@ -g -lpthread

OBJ_client=client
OBJS_client += client.c
OBJS_client += wrap.c

cc=gcc

$(OBJ_client):$(OBJS_client)
	$(CC) $^ -o $@ -g -lpthread