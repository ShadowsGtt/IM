#include "Server.h"
#include <iostream>

using namespace std;

Server::Server() : //addr_("0.0.0.0",9999) , server_(&loop_,addr_,"IM Server"),
		dispatcher_(std::bind(&Server::onUnknownMessage, this, _1, _2, _3)),
		codec_(std::bind(&ProtobufDispatcher::onProtobufMessage, &dispatcher_, _1, _2, _3))
{
    //configMap_ = readConfig("../conf/server.conf");

    loop_ = new EventLoop();
    //addr_.setIpPort(configMap_["host"], std::stoi(configMap_["port"]));
    //server_ = new TcpServer(loop_,addr_,configMap_["serverName"]);

    //server_->setThreadNum(std::stoi(configMap_["ioThreadNumber"]));


    addr_.setIpPort("0.0.0.0", 9999);
    server_ = new TcpServer(loop_,addr_,"serverName");

    server_->setThreadNum(5);

    /* 根据消息类型注册该类型消息回调 */
	dispatcher_.registerMessageCallback<IM::Login>(
		std::bind(&Server::Login, this, _1, _2, _3));
    dispatcher_.registerMessageCallback<IM::Register>(
            std::bind(&Server::Register, this, _1, _2, _3));
    dispatcher_.registerMessageCallback<IM::SingleChat>(
            std::bind(&Server::SingleChat, this, _1, _2, _3));
    dispatcher_.registerMessageCallback<IM::GroupChat>(
            std::bind(&Server::GroupChat, this, _1, _2, _3));

    /* 连接状态发生改变时的回调函数 */
    server_->setConnectionCallback(
            std::bind(&Server::onConnection, this, _1));

    /* 接受到新消息时的回调函数 */
    server_->setMessageCallback(
            std::bind(&ProtobufCodec::onMessage, &codec_, _1, _2, _3));
}
void Server::start()
{
	server_->start();
	loop_->loop();
}



void Server::onConnection(const TcpConnectionPtr& conn)
{
	if(conn->connected())
	{
		LOG_INFO << conn->peerAddress().toIpPort() << "  connected! ";
	}
	else
	{	
		LOG_INFO << conn->peerAddress().toIpPort() << "  disconnected! ";
	}
}
void Server::Login(const net::TcpConnectionPtr& conn,
               const LoginPtr& message,
               Timestamp timestamp)
{
	cout << endl;
	cout << "-----------------------protobuf--start-------------------------------" << endl;
    cout << "protobuf message type :" << message->GetTypeName() <<endl 
    	 << message->DebugString();
	cout << "------------------------protobuf---end-------------------------------" << endl;

	Response answer;
    answer.set_result(true);
	answer.set_description("Login success");

    codec_.send(conn, answer);
    //conn->shutdown();
}

void Server::Register(const net::TcpConnectionPtr &conn,
        const RegisterPtr& message, Timestamp timestamp)
{
    cout << endl;
    cout << "-----------------------protobuf--start-------------------------------" << endl;
    cout << "protobuf message type :" << message->GetTypeName() <<endl
         << message->DebugString();
    cout << "------------------------protobuf---end-------------------------------" << endl;

    Response answer;
    answer.set_result(true);
    answer.set_description("Register success");

    codec_.send(conn, answer);
}
void Server::SingleChat(const net::TcpConnectionPtr &conn,
                      const SingleChatPtr& message, Timestamp timestamp)
{
    cout << endl;
    cout << "-----------------------protobuf--start-------------------------------" << endl;
    cout << "protobuf message type :" << message->GetTypeName() <<endl
         << message->DebugString();
    cout << "------------------------protobuf---end-------------------------------" << endl;

    Response answer;
    answer.set_result(true);
    answer.set_description("SingleChat success");

    codec_.send(conn, answer);

}
void Server::GroupChat(const net::TcpConnectionPtr &conn,
                      const GroupChatPtr& message, Timestamp timestamp)
{
    cout << endl;
    cout << "-----------------------protobuf--start-------------------------------" << endl;
    cout << "protobuf message type :" << message->GetTypeName() <<endl
         << message->DebugString();
    cout << "------------------------protobuf---end-------------------------------" << endl;

    Response answer;
    answer.set_result(true);
    answer.set_description("GroupChat success");

    codec_.send(conn, answer);

}