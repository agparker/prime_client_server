#ifndef REQUEST_REPLY_INTERFACE
#define REQUEST_REPLY_INTERFACE

class  RequestReplyInterface
{
public:
	virtual ~RequestReplyInterface() {}
	virtual bool Init() = 0;
	virtual bool Connect(const char *server, unsigned short port) = 0;
	virtual bool Send(unsigned int number) = 0;
	virtual bool GetReply() = 0;
};

#endif
