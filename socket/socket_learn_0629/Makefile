all: server client test server_select
OBJ_server=server

OBJS_server += server.c
OBJS_server += wrap.c

cc=gcc

$(OBJ_server):$(OBJS_server)
	$(CC) $^ -o $@ -g

OBJ_client=client
OBJS_client += client.c
OBJS_client += wrap.c

$(OBJ_client):$(OBJS_client)
	$(CC) $^ -o $@ -g

OBJ_test = test
OBJS_test += wrap.c
OBJS_test += test.c

$(OBJ_test):$(OBJS_test)
	$(CC) $^ -o $@ -g


OBJ_server_select = server_select

OBJS_server_select += server_select.c
OBJS_server_select += wrap.c

cc=gcc

$(OBJ_server_select):$(OBJS_server_select)
	$(CC) $^ -o $@ -g
