#include <iostream>
#include "../include/TcpServer.h"
#include "../include/Logging.h"
#include "../include/EventLoop.h"
#include "../include/Timestamp.h"

using namespace std;
using namespace net;

void onConnection(const TcpConnectionPtr &conn)
{
    LOG_INFO << "EchoServer - " << conn->peerAddress().toIpPort() << " -> "
        << conn->localAddress().toIpPort() << " is "
        << (conn->connected() ? "UP" : "DOWN");
}

void onMessage(const TcpConnectionPtr &conn,Buffer *buf, Timestamp time)
{
    string msg(buf->retrieveAllAsString());
    LOG_INFO << conn->name() << " echo " << msg.size() << " bytes, "
        << "data received at " << time.toFormattedString();
    conn->send(msg);
}

int main(void)
{
    EventLoop loop;
    InetAddress serv_addr("127.0.0.1",40009);
    TcpServer server(&loop,serv_addr,"TcpServer",TcpServer::kReusePort);
    server.setThreadNum(5);

    server.setConnectionCallback(&onConnection);
    server.setMessageCallback(&onMessage);

    cout << "server start " << endl;
    server.start();
    loop.loop();
    return 0;
}
