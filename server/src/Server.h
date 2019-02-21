#ifndef _SERVER_H
#define _SERVER_H


#include "codec.h"
#include "dispatcher.h"
#include "IM.pb.h"

#include "Logging.h"
#include "Mutex.h"
#include "EventLoop.h"
#include "TcpServer.h"

#include <stdio.h>
#include <unistd.h>

using namespace net;
using namespace IM;



typedef std::shared_ptr<IM::Login> LoginPtr;
typedef std::shared_ptr<IM::Response> ResponsePtr;



class Server : noncopyable
{
 public:
	
  Server();

  void onConnection(const TcpConnectionPtr& conn);

  void onUnknownMessage(const TcpConnectionPtr& conn,
                        const MessagePtr& message,
                        Timestamp)
  {
    LOG_INFO << "onUnknownMessage: " << message->GetTypeName();
    conn->shutdown();
  }
	
  void start();


  void onLogin(const net::TcpConnectionPtr& conn,
               const LoginPtr& message,
               Timestamp);

 private:
	uint16_t IoThreadNum;
	EventLoop loop_; 
	InetAddress addr_;
	TcpServer server_;
  	ProtobufDispatcher dispatcher_;
  	ProtobufCodec codec_;

};

#endif
