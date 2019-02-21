#include "Client.h"
#include <unistd.h>
#include "../../server/src/EventLoopThread.h"



void runClient(Client *cli,const char *ip,uint16_t port)
{
	EventLoop loop;	
	InetAddress serverAddr(ip, port);
	Client client(&loop,serverAddr);
	cli = &client;
	
	client.start();
	loop.loop();
}



int main(int argc, char* argv[])
{
  LOG_INFO << "pid = " << getpid();
  if (argc > 2)
  {
	uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
    
    InetAddress serverAddr(argv[1], port);
	EventLoop loop;	
	Client client(&loop,serverAddr);
	client.start();
	loop.loop();
    
    //client.send(&query);
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


