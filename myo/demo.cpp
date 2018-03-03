#include "UdpServer.h"
#include <iostream>
#include <unistd.h> // for sleep()
#include <string>

using namespace std;

int main(int argc, char *argv[]) {

  int status;
  char* data;
  UdpServer myServer; // Creates server object

  // first checks arguments
  if (argc < 3) {
    cout << ("ERROR: Need to pass IP and PORT\n./client <IP_of_server> <PORT_on_server>\n") << endl;
    return -1;
  }

  data = (char*)malloc(sizeof(char) * MAX_MSG_SIZE);
  if (data == NULL) {
    cout << ("ERRPR: Malloc for data failed\n") << endl;
  }

  status = myServer.connectSocket(argv[1], atoi(argv[2]));

  if (status != 0) {
    cout << "ERROR connecting with value: " << status << endl;
  }

  while(1) {
    sprintf(data, "%f,%f,%f", 4.5f, 3.3f, 2.2f);
    myServer.send(data, strlen(data));
    cout << "Data Sent" << endl;
    sleep(1);
  }
}
