#include "Client.h"


Client::Client(InetAddress serverAddr) : servAddr_(serverAddr), 
            loop_(NULL), tcpClient_(NULL),
            dispatcher_(NULL), codec_(NULL),
            latch_(1),
            latch2_(1)
{}

void Client::runLoop()
{
    loop_ = std::make_shared<EventLoop>();
    tcpClient_ = std::make_shared<TcpClient>(loop_.get(), servAddr_, "Client");
    dispatcher_ = std::make_shared<ProtobufDispatcher>(std::bind(&Client::onUnknownMessage, this, _1, _2, _3) );
    codec_ = std::make_shared<ProtobufCodec>(std::bind(&ProtobufDispatcher::onProtobufMessage, dispatcher_.get(), _1, _2, _3));

    //收到服务器对应消息的回调 
    dispatcher_->registerMessageCallback<IM::Response>(
        std::bind(&Client::onResponse, this, _1, _2, _3));
    dispatcher_->registerMessageCallback<IM::Empty>(
        std::bind(&Client::onEmpty, this, _1, _2, _3));


    tcpClient_->setConnectionCallback(
        std::bind(&Client::onConnection, this, _1));
    tcpClient_->setMessageCallback(
        std::bind(&ProtobufCodec::onMessage, codec_.get(), _1, _2, _3));
    latch_.countDown();
    loop_->loop();
}


void Client::start()
{
    thread_ = std::make_shared<Thread>(std::bind(&Client::runLoop,this),"loop Thread");
    thread_->start();
    
    /* 等待新线程运行 */
    latch_.wait();

    /* 尝试与服务器建立连接 */
    tcpClient_->connect();
    
    /* 等待连接建立 */
    latch2_.wait();
}

void Client::send(google::protobuf::Message *mesg)
{
    if(conn_)
    {
        
        if(conn_->connected())
        {
            codec_->send(conn_,*mesg);
        }
        else
        {

        }
    }
    else
    {
        cout << "发送失败...(连接不存在)" << endl;
    }
}

void Client::onConnection(const TcpConnectionPtr &conn)
{
	if(conn->connected())
    {
        conn_ = conn;
        latch2_.countDown();
        LOG_INFO << " connect to " << conn->peerAddress().toIpPort() ;
    }
    else 
    {
		LOG_INFO << " disconnect with " << conn_->peerAddress().toIpPort() ;
		conn_ = NULL;    
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

