#ifndef _WEB_SOCKET_DATA_MESSAGE_INCLUDE_
#define _WEB_SOCKET_DATA_MESSAGE_INCLUDE_

#include <string>
#include <sstream>

class WebsocketMessage
{
public:
	enum
	{
		Handshake = 0,
		DataMessage,
	};
	WebsocketMessage(int type)
	{
		this->type = type;
	}
	~WebsocketMessage(void)
	{
		//
	}
	int GetType()
	{
		return type;
	}
private:
	int type;
};

class WebsocketDataMessage : public WebsocketMessage
{
public:
	WebsocketDataMessage(const char* buffer);
	WebsocketDataMessage(int serviceId);
	~WebsocketDataMessage(void);
	
	int GetServiceId();
	void SetServiceId(int serviceId);
	void SetArguments(std::string arg1, std::string arg2 = "");
	std::string GetArg1();
	std::string GetArg2();
    bool Encode();
	bool Decode();
	std::string GetEncodedData();
private:
	int serviceId;
	std::string arg1;
	std::string arg2;

	std::string raw;
};

#endif

