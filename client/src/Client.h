#ifndef _CLIENT_H
#define _CLIENT_H

#include "../../server/src/dispatcher.h"
#include "../../server/src/codec.h"
#include "../../server/src/IM.pb.h"

#include "../../server/src/Logging.h"
#include "../../server/src/Mutex.h"
#include "../../server/src/EventLoop.h"
#include "TcpClient.h"

#include <stdio.h>
#include <unistd.h>
#include <iostream>

using namespace std;
using namespace IM;
using namespace net;

typedef std::shared_ptr<IM::Empty> EmptyPtr;
typedef std::shared_ptr<IM::Response> ResponsePtr;

//google::protobuf::Message* messageToSends;

class Client : noncopyable
{
 public:
  
  Client(EventLoop* loop , InetAddress& serverAddr);
  //Client(InetAddress &serverAddr);
  
  void start();

  void onConnection(const TcpConnectionPtr& conn);

  void onUnknownMessage(const TcpConnectionPtr&,
                        const MessagePtr& message,
                        Timestamp);
  void onResponse(const net::TcpConnectionPtr&,
                const ResponsePtr& message,
                Timestamp);

  void onEmpty(const net::TcpConnectionPtr&,
               const EmptyPtr& message,
               Timestamp);
  
  
  void send(google::protobuf::Message*) ;

private:
	EventLoop* loop_;
  	TcpClient client_;
	ProtobufDispatcher dispatcher_;
	ProtobufCodec codec_;
	TcpConnectionPtr conn_;
};

#endif

