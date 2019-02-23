#include "Client.h"
#include <unistd.h>
#include "../../server/src/EventLoopThread.h"



void runClient(Client *client)
{
}



int main(int argc, char* argv[])
{
  //LOG_INFO << "pid = " << getpid();
  if (argc > 2)
  {
	uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
    const char *ip = argv[1]; 

    InetAddress serverAddr(ip, port);
    Client client(serverAddr);

    client.start();

    Login query;
    query.set_id(1);
    query.set_username("This is username");
    query.set_password("This is password");
    client.send(&query);
    while(1)
    {
    }
  }
  else
  {
    printf("Usage: %s host_ip port [q|e]\n", argv[0]);
  }
}


