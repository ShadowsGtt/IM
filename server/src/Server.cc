#include "Server.h"
#include "ReadConfig.h"
#include <iostream>

using namespace std;


AsyncLogging* Server::log_ = NULL;

void logOutput(const char *msg, int len)
{
    Server::log_->append(msg,len);
}


Server::Server() : dispatcher_(std::bind(&Server::onUnknownMessage, this, _1, _2, _3)),
                   codec_(std::bind(&ProtobufDispatcher::onProtobufMessage, &dispatcher_, _1, _2, _3))
{
    //读取配置文件
    configMap_ = readConfig("../conf/server.conf");

    //创建异步日志系统
    log_ = new AsyncLogging("im",toByte(configMap_["logRollSie"]),std::stoi(configMap_["logFlushInterval"]));
    // 设置日志输出
    Logger::setOutput(logOutput);
    LOG_INFO << "config read finished" ;

    //创建线程池
    threadPool_ = new ThreadPool("ThreadPool");
    threadPool_->setMaxQueueSize(1024);
    LOG_INFO << "thread pool start running";

    // 创建事件循环
    loop_ = new EventLoop();
    addr_.setIpPort(configMap_["host"], std::stoi(configMap_["port"]));
    server_ = new TcpServer(loop_,addr_,configMap_["serverName"]);

    server_->setThreadNum(std::stoi(configMap_["ioThreadNumber"]));

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

//void Server::addTask(const Task &task)
//{
//    threadPool_->addTask(task);
//}

void Server::start()
{
    // 启动异步日志系统
    log_->start();
    threadPool_->start(std::stoi(configMap_["tpNum"]));
    server_->start();
    LOG_INFO << "server start running";
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