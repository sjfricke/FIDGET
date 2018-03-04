#include "UdpServer.h"

UdpServer::UdpServer() {
}

UdpServer::~UdpServer() {
//  close(socket_fd);
}
// used to setup and connect to server
// returns 0 on success
int UdpServer::connectSocket( std::string ip, int port ) {

  int status; // used to check status returns
  
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) { printf("socket() ERROR\n"); return 1; }

  server_addr.sin_addr.s_addr = inet_addr(ip.c_str()); // sets IP of server
  server_addr.sin_family = AF_INET; // uses internet address domain
  server_addr.sin_port = htons(port); // sets PORT on server

  status = connect(socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));
  if (status < 0) { printf("connect() ERROR\n"); return 2; }

  
  printf("end connect, %d\n", status);
  return 0;
}

// sends message to all other users online
// returns 0 on success
int UdpServer::send(std::string message ) {
  int status;
  
  status = sendto(socket_fd, message.c_str() ,strlen(message.c_str()) , 0, (struct sockaddr*) &server_addr, sizeof(server_addr));
  if (status < 0) {
    printf("sendto() ERROR\n"); return 1;
  } else {
    return 0;
  }
}

int UdpServer::send_c(char*  message ) {
  int status;
  
  status = sendto(socket_fd, message ,strlen(message) , 0, (struct sockaddr*) &server_addr, sizeof(server_addr));
  if (status < 0) {
    printf("sendto() ERROR\n"); return 1;
  } else {
    return 0;
  }
}
