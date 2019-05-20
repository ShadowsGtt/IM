#ifndef _SERVER_H
#define _SERVER_H

#include "codec.h"
#include "dispatcher.h"
#include "IM.pb.h"
#include "Logging.h"
#include "Mutex.h"
#include "EventLoop.h"
#include "TcpServer.h"
#include "AsyncLogging.h"

#include <stdio.h>
#include <unistd.h>


using namespace net;
using namespace IM;

typedef std::shared_ptr<IM::Login> LoginPtr;
typedef std::shared_ptr<IM::Register> RegisterPtr;
typedef std::shared_ptr<IM::SingleChat> SingleChatPtr;
typedef std::shared_ptr<IM::GroupChat> GroupChatPtr;

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

    void Login(const net::TcpConnectionPtr &conn,
               const LoginPtr &message,
               Timestamp timestamp);

    void Register(const net::TcpConnectionPtr &conn,
                  const RegisterPtr &message,
                  Timestamp timestamp);

    void SingleChat(const net::TcpConnectionPtr &conn,
                    const SingleChatPtr &message,
                    Timestamp timestamp);

    void GroupChat(const net::TcpConnectionPtr &conn,
                   const GroupChatPtr &message,
                   Timestamp timestamp);

    static AsyncLogging *log_;                     // 异步日志
private:
    map <string, string> configMap_;        // 配置
    EventLoop *loop_;                       // 主线程循环
    InetAddress addr_;                      // 服务器地址
    TcpServer *server_;                     // tcp server
    ProtobufDispatcher dispatcher_;         // 协议包分发
    ProtobufCodec codec_;                   // 编解码
};

#endif
