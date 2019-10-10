#include "WebsocketDataMessage.h"


WebsocketDataMessage::WebsocketDataMessage(const char* buffer)
	:WebsocketMessage(DataMessage)
{
	raw = buffer;
}

WebsocketDataMessage::WebsocketDataMessage( int serviceId )
	:WebsocketMessage(DataMessage)
{
	this->serviceId = serviceId;
}

WebsocketDataMessage::~WebsocketDataMessage(void)
{
}

bool WebsocketDataMessage::Encode()
{
	std::stringstream ss;

	ss << serviceId << arg1;
    
    if (!arg2.empty())
        ss << "|" << arg2;
  
	raw = ss.str();

	return true;
}

bool WebsocketDataMessage::Decode()
{
	std::stringstream ss(raw);

    serviceId = atoi(raw.substr(0, 1).c_str());

    std::string::size_type separator = raw.find_first_of("|");
    
    arg1 = raw.substr(1, separator-1);
    
    if(separator == std::string::npos)
        return true;
    
    arg2 = raw.substr(separator+1);
	return true;
}

int WebsocketDataMessage::GetServiceId()
{
	return serviceId;
}

std::string WebsocketDataMessage::GetArg1()
{
	return arg1;
}

std::string WebsocketDataMessage::GetArg2()
{
	return arg2;
}

void WebsocketDataMessage::SetServiceId( int serviceId )
{
	this->serviceId = serviceId;
}

void WebsocketDataMessage::SetArguments( std::string arg1, std::string arg2 /*= ""*/ )
{
	this->arg1 = arg1;
	this->arg2 = arg2;
}

std::string WebsocketDataMessage::GetEncodedData()
{
	return raw;
}
