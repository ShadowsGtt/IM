#include "Server.h"
#include <iostream>

using namespace std;

Server::Server() : addr_("0.0.0.0",9999) , server_(&loop_,addr_,"IM Server"),
		dispatcher_(std::bind(&Server::onUnknownMessage, this, _1, _2, _3)),
		codec_(std::bind(&ProtobufDispatcher::onProtobufMessage, &dispatcher_, _1, _2, _3))
{	
	IoThreadNum = 5;
	server_.setThreadNum(IoThreadNum);

    /* 根据消息类型注册该类型消息回调 */
	dispatcher_.registerMessageCallback<IM::Login>(
		std::bind(&Server::onLogin, this, _1, _2, _3));
    
	/* 连接状态发生改变时的回调函数 */
	server_.setConnectionCallback(
        std::bind(&Server::onConnection, this, _1));
    
	/* 接受到新消息时的回调函数 */
	server_.setMessageCallback(
        std::bind(&ProtobufCodec::onMessage, &codec_, _1, _2, _3));
	
}
void Server::start()
{
	server_.start();
	loop_.loop();
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
void Server::onLogin(const net::TcpConnectionPtr& conn,
               const LoginPtr& message,
               Timestamp)
{
	cout << endl;
	cout << "-----------------------protobuf--start-------------------------------" << endl;
    cout << "protobuf message type :" << message->GetTypeName() <<endl 
    	 << message->DebugString();
	cout << "------------------------protobuf---end-------------------------------" << endl;
    Response answer;
    answer.set_id(1);
    answer.set_result(true);
	answer.set_description("Login success");

    codec_.send(conn, answer);
    //conn->shutdown();
}

