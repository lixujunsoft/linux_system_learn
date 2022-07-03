#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

/*  */
int main(int argc, char *argv[])
{
    char *ptr, **pptr;
    char str[INET_ADDRSTRLEN];
    struct hostent *hptr;
    struct in_addr addr;
    
    while (--argc > 0) {
        ptr = *++argv;
        inet_pton(AF_INET ,ptr, &addr);
        if ((hptr = gethostbyaddr(&addr, sizeof(addr), AF_INET)) == NULL) {
            printf("gethostbyaddr error for host: %s: %s\n", ptr, hstrerror(h_errno));
            continue;
        }

        printf("official hostname: %s\n", hptr->h_name);

        for (pptr = hptr->h_aliases; *pptr != NULL; pptr++) {
            printf("\talias: %s\n", *pptr);
        }

        switch (hptr->h_addrtype) {
        case AF_INET:
            pptr = hptr->h_addr_list;
            for (; *pptr != NULL; pptr++) {
                printf("\taddress: %s\n", inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
            }
            break;
        default:
            printf("unknown address type\n");
            break;
        }
    }
    return 0;
}