#ifndef H_UDPSERVER_H
#define H_UDPSERVER_H

#ifdef _WIN32
  /* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
  #include <winsock2.h>
  #include <Ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library
#else
  /* Assume that any non-Windows platform uses POSIX-style sockets instead. */
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <sys/types.h>
  #include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


#define MAX_MSG_SIZE 256

class UdpServer {

 public:
  // Constructor and destructor
  UdpServer();
  ~UdpServer();

  // used to setup and connect to server
  // returns 0 on success
  int connectSocket(const char* ip, int port);

  void closeSocket();

  // sends message to all other users online
  // returns 0 on success
  int send(char* message, int message_size);

 private:
  struct sockaddr_in server_addr; // socket struct object
  int socket_fd;                  // holds socket file discriptor

  char message_recv[MAX_MSG_SIZE];
};

#endif // H_UDPSERVER_H
