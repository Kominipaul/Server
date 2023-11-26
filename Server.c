#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


#define SIZE 1024

void send_file(FILE *fp, int sockfd){
  int n;
  char data[SIZE] = {0};

  while(fgets(data, SIZE, fp) != NULL) 
  {
    if (send(sockfd, data, sizeof(data), 0) == -1) 
    {
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, SIZE);
  }
}

int main(int argc, char* argv[]) {

  if (argc < 2) 
  {
    printf("./Client PORT\n");
    exit(0);
  }

  int port = atoi(argv[1]);

  FILE *fp;
  char *filename = "send.txt";
  
  int server_socket, new_sock;
  socklen_t addr_size;

  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) 
  {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");

  struct sockaddr_in server_address, server_addr, new_addr;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  server_address.sin_addr.s_addr = INADDR_ANY; // "195.251.38.38"

  int e = bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

  if(e < 0)
  {
    perror("[-]Error in bind");
    exit(1);
  }
  printf("[+]Binding successfull.\n");

  if(listen(server_socket, 10) == 0) 
  {
    printf("[+]Listening....\n");
  }
  else 
  {
    perror("[-]Error in listening");
    exit(1);
  }

  addr_size = sizeof(new_addr);
  new_sock = accept(server_socket, (struct sockaddr*)&new_addr, &addr_size);

  fp = fopen(filename, "r");
  if (fp == NULL)
  {
    perror("[-]Error in reading file.");
    exit(1);
  }

  send_file(fp, new_sock);
  printf("[+]File data sent successfully.\n");

  printf("[+]Closing the connection.\n");
  
  close(server_socket);

  return 0;
}
