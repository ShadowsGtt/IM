#ifndef _CLIENT_H
#define _CLIENT_H

#include "../../server/src/dispatcher.h"
#include "../../server/src/CountDownLatch.h"
#include "../../server/src/codec.h"
#include "../../server/src/Thread.h"
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
  
  //Client(EventLoop* loop , InetAddress& serverAddr);
  Client(InetAddress serverAddr);
  
  void start();

  void runLoop();

  void onConnection(const TcpConnectionPtr &conn);

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
  InetAddress servAddr_;
  std::shared_ptr<EventLoop> loop_;
  std::shared_ptr<TcpClient> tcpClient_;
  std::shared_ptr<ProtobufDispatcher> dispatcher_;
  std::shared_ptr<ProtobufCodec> codec_;
  CountDownLatch latch_;
  std::shared_ptr<Thread> thread_;
  TcpConnectionPtr conn_;
};

#endif

