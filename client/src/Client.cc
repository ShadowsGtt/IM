#include "Client.h"


Client::Client(EventLoop* loop , InetAddress& serverAddr) : loop_(loop) , conn_(),
				client_(loop_, serverAddr, "Client") ,
				dispatcher_(std::bind(&Client::onUnknownMessage, this, _1, _2, _3)),
				codec_(std::bind(&ProtobufDispatcher::onProtobufMessage, &dispatcher_, _1, _2, _3))
{
 	/* 收到服务器回应的回调 */
    dispatcher_.registerMessageCallback<IM::Response>(
        std::bind(&Client::onResponse, this, _1, _2, _3));

    dispatcher_.registerMessageCallback<IM::Empty>(
        std::bind(&Client::onEmpty, this, _1, _2, _3));
    client_.setConnectionCallback(
        std::bind(&Client::onConnection, this, _1));
    client_.setMessageCallback(
        std::bind(&ProtobufCodec::onMessage, &codec_, _1, _2, _3));

}

void Client::start()
{
	client_.connect();
	//loop_->loop();
}

void Client::send(google::protobuf::Message *mesg)
{
	if(conn_->connected())
	{
		cout << "will send mesg " << endl;
		codec_.send(conn_,*mesg);
	}
	else
	{
		
	}

}

void Client::onConnection(const TcpConnectionPtr& conn)
{
	conn_ = conn;
	if(conn->connected())
	{
		cout << "连接至服务器 ：" << conn->peerAddress().toIpPort() << endl ;
		Login query;
   	 	query.set_id(1);
    	query.set_username("This is UserName");
    	query.set_password("This is Password");
		codec_.send(conn, query);
	}	
	else
	{
		cout << "与服务器 ：" << conn->peerAddress().toIpPort() << "   断开连接" << endl ;			
      		//loop_->quit();	
	}
}

void Client::onUnknownMessage(const TcpConnectionPtr&,
                        const MessagePtr& message,
                        Timestamp)
{
    LOG_INFO << "onUnknownMessage: " << message->GetTypeName();
}


/* 客户端收到的回应 */
void Client::onResponse(const net::TcpConnectionPtr&,
                const ResponsePtr& message,
                Timestamp)
{
	cout << endl;
	cout << "--------------------protobuf----response----------------" << endl;
    cout << "protobuf message type : "<< message->GetTypeName() <<endl
         << message->DebugString();
  	cout << "--------------------protobuf----response----------------" << endl;
}

void Client::onEmpty(const net::TcpConnectionPtr&,
               const EmptyPtr& message,
               Timestamp)
{
    LOG_INFO << "onEmpty: " << message->GetTypeName();
}

