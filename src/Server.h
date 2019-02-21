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
using namespace muduo;

typedef std::shared_ptr<muduo::Query> QueryPtr;
typedef std::shared_ptr<muduo::Answer> AnswerPtr;
//typedef std::shared_ptr<IM::Request> RequestPtr;
//typedef std::shared_ptr<IM::Response> ResponsePtr;



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

  void onQuery(const net::TcpConnectionPtr& conn,
               const QueryPtr& message,
               Timestamp);

  void onAnswer(const net::TcpConnectionPtr& conn,
                const AnswerPtr& message,
                Timestamp);

/*
  void onRequest(const net::TcpConnectionPtr& conn,
               const QueryPtr& message,
               Timestamp);
*/ 
  void onResponse(const net::TcpConnectionPtr& conn,
                const AnswerPtr& message,
                Timestamp)
  {
    LOG_INFO << "onResponse: " << message->GetTypeName();
    conn->shutdown();
  }
 private:
	uint16_t IoThreadNum;
	EventLoop loop_; 
	InetAddress addr_;
	TcpServer server_;
  	ProtobufDispatcher dispatcher_;
  	ProtobufCodec codec_;

};


