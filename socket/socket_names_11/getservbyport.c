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
    char **aliases;
    sptr = getservbyport(htons(53), "udp");
    printf("s_name: %s\n", sptr->s_name);

    for (aliases = sptr->s_aliases; *aliases != NULL; aliases++) {
        printf("s_aliases: %s\n", *aliases);
    }
    printf("s_port: %d\n", ntohs(sptr->s_port));
    printf("s_proto: %s\n", sptr->s_proto);
}