#include <stdio.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define PORT 8005
#define BUFFER_SIZE 2048


int main() { 
  char header[] = "HTTP/1.0 200 OK\nContent-Type: text/html\n\n";



  int sfd = socket(AF_INET, SOCK_STREAM, 0);


  struct sockaddr_in addr;

  memset(&addr, 0, sizeof(addr));
  
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);


  if(bind(sfd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
    printf("Error binding.\n");
    return 1;
  }

  listen(sfd, 10);

  int cfd = accept(sfd, 0, 0);
  char buffer[BUFFER_SIZE] = {0};
  recv(cfd, buffer, BUFFER_SIZE, 0);

  // A basic HTTP header:
  // GET /index.html
  // We strip the first five bytes to get to the filename requested (index.html in this case):

  char* f = buffer + 5;
  *strchr(f, ' ') = 0;      // Replace first space with a null terminator \0

  char content[BUFFER_SIZE] = {0};

  strcpy(content, header);
  strcat(content, "<h1>It works!</h1>");

  printf("Content: %s\n", content);

  send(cfd, content, sizeof(content), 0);
  
  printf("Closing sockets.\n");

  shutdown(sfd, SHUT_RDWR);
  shutdown(cfd, SHUT_RDWR);
  close(sfd);
  close(cfd);

  return 0;
}
