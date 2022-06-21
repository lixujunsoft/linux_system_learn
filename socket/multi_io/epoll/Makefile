all: server client server_optimize

OBJ_server = server
OBJS_server += server.c
OBJS_server += wrap.c

cc=gcc

$(OBJ_server):$(OBJS_server)
	$(CC) $^ -o $@ -g

OBJ_client = client
OBJS_client += client.c
OBJS_client += wrap.c

cc=gcc

$(OBJ_client):$(OBJS_client)
	$(CC) $^ -o $@ -g


OBJ_server_optimize = server_optimize
OBJS_server_optimize += server_optimize.c
OBJS_server_optimize += wrap.c
$(OBJ_server_optimize):$(OBJS_server_optimize)
	$(CC) $^ -o $@ -g

