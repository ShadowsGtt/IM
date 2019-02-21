#include "Server.h"


Server::Server() : addr_("0.0.0.0",8888) , server_(&loop_,addr_,"IM Server"),
		dispatcher_(std::bind(&Server::onUnknownMessage, this, _1, _2, _3)),
		codec_(std::bind(&ProtobufDispatcher::onProtobufMessage, &dispatcher_, _1, _2, _3))
{	
	//loop_ = new net::Eventloop();
	//addr_ = new net::InetAddress("0.0.0.0",8888);
	//server_(loop_.get(),addr_.get(),"IM Server");
	IoThreadNum = 5;
	server_.setThreadNum(IoThreadNum);

	dispatcher_.registerMessageCallback<muduo::Query>(
        std::bind(&Server::onQuery, this, _1, _2, _3));
    dispatcher_.registerMessageCallback<muduo::Answer>(
        std::bind(&Server::onAnswer, this, _1, _2, _3));
    

	server_.setConnectionCallback(
        std::bind(&Server::onConnection, this, _1));
    server_.setMessageCallback(
        std::bind(&ProtobufCodec::onMessage, &codec_, _1, _2, _3));
	
/*
	dispatcher_.registerMessageCallback<IM::Request>(
        std::bind(&Server::onRequest, this, _1, _2, _3));
    dispatcher_.registerMessageCallback<IM::Response>(
        std::bind(&Server::onResponse, this, _1, _2, _3));
    server_.setConnectionCallback(
        std::bind(&QueryServer::onConnection, this, _1));
    server_.setMessageCallback(
        std::bind(&ProtobufCodec::onMessage, &codec_, _1, _2, _3));

*/
}
void Server::start()
{
	server_.start();
	loop_.loop();
}



void Server::onConnection(const TcpConnectionPtr& conn)
{
	LOG_INFO << conn->localAddress().toIpPort() << " -> "
        << conn->peerAddress().toIpPort() << " is "
        << (conn->connected() ? "UP" : "DOWN");
}
void Server::onQuery(const net::TcpConnectionPtr& conn,
               const QueryPtr& message,
               Timestamp)
{
    LOG_INFO << "onQuery:\n" << message->GetTypeName() << message->DebugString();
    Answer answer;
    answer.set_id(1);
    answer.set_questioner("Chen Shuo");
    answer.set_answerer("blog.csdn.net/Solstice");
    answer.add_solution("Jump!");
    answer.add_solution("Win!");
    codec_.send(conn, answer);

    conn->shutdown();
}

void Server::onAnswer(const net::TcpConnectionPtr& conn,
                const AnswerPtr& message,
                Timestamp)
{
    LOG_INFO << "onAnswer: " << message->GetTypeName();
    conn->shutdown();
}

/*
 void onRequest(const net::TcpConnectionPtr& conn,
               const RequestPtr& message,
               Timestamp)
  {
    LOG_INFO << "onRequest:\n" << message->GetTypeName() << message->DebugString();
    Response answer;
    answer.set_id(1);
    answer.set_questioner("Chen Shuo");
    answer.set_answerer("blog.csdn.net/Solstice");
    answer.add_solution("Jump!");
    answer.add_solution("Win!");
    codec_.send(conn, answer);

    conn->shutdown();
  }
*/
