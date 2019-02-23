#include "Client.h"


Client::Client(InetAddress serverAddr) : servAddr_(serverAddr), 
            loop_(NULL), tcpClient_(NULL),
            dispatcher_(NULL), codec_(NULL),
            latch_(1)
{}

void Client::runLoop()
{
    loop_ = std::make_shared<EventLoop>();
    //latch_.wait();
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
    latch_.wait();

    tcpClient_->connect();
}

void Client::send(google::protobuf::Message *mesg)
{
    cout << tcpClient_->connection().use_count() << endl;
    //cout << "3 send  use_count :" << conn_.use_count() << endl;
    if(this->conn_ == NULL)
        cout << "is NULL" << endl;
    else 
        cout << "not NULL " << endl;
        /*
    if(tcpClient_->connection() != NULL)
    {
        
        if(tcpClient_->connection()->connected())
        {
            cout << "will send mesg " << endl;
            codec_->send(tcpClient_->connection(),*mesg);
        }
        else
        {

        }
    }
    else
        cout << "not NULL " << endl;
*/
}

void Client::onConnection(const TcpConnectionPtr &conn)
{
    conn_ = conn;
    cout << "use count :" << conn_.use_count() << endl;
	if(conn->connected())
    {
		cout << "与" << conn->peerAddress().toIpPort() << "建立连接"  << endl ;
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

