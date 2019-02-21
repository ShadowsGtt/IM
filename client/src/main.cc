#include "Client.h"
#include <unistd.h>


int main(int argc, char* argv[])
{
  LOG_INFO << "pid = " << getpid();
  if (argc > 2)
  {
	uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
    InetAddress serverAddr(argv[1], port);

    Login query;
    query.set_id(1);
    query.set_username("This is UserName");
    query.set_password("This is Password");


	EventLoop loop;
    Client client(&loop,serverAddr);
    client.start();
	loop.loop();
	
    client.send(&query);

	if (argc > 3 && argv[3][0] == 'e')
    {
		  Empty empty;
          client.send(&empty);
    }
  }
  else
  {
    printf("Usage: %s host_ip port [q|e]\n", argv[0]);
  }
}


