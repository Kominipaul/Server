#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h>
#include <arpa/inet.h>

#define SIZE 1024

void write_file(int sockfd) 
{
    int n;
    FILE *fp;
    char *filename = "recv.txt";
    char buffer[SIZE];

    fp = fopen(filename, "w");
    while (1)
    {
        n = recv(sockfd, buffer, SIZE, 0);
        if (n <= 0)
        {
            break;
            return;
        }
        fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE);
    }
    return;
}

int main(int argc, char* argv[]) 
{
    
    if (argc < 3) 
    {
        printf("./Client IP PORT\n");
        exit(0);
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    socklen_t addr_size;
    char buffer[SIZE];

    //create a socket
    int network_socket, new_sock;
    network_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(network_socket < 0) 
    {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");

    //specify an address for the socket
    struct sockaddr_in server_address, new_addr;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr(ip);

    int connetion_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    if (connetion_status == -1) 
    {
        perror("[-]Error in socket");
        exit(1);
    }
	printf("[+]Connected to Server.\n");
    
    write_file(network_socket);
    printf("[+]Data written in the file successfully.\n");
    
    close(network_socket);

    return 0;
}