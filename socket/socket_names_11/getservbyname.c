#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>

/*
struct servent
  char *s_name;	
  char **s_aliases;
  int s_port;	
  char *s_proto;
}
*/

int main(int argc, char *argv[])
{
    struct servent *sptr;

    sptr = getservbyname("daytime", "udp");
    printf("s_name: %s\n", sptr->s_name);
    printf("s_port: %d", ntohs(sptr->s_port));
}