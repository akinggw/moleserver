#ifndef _WEBSOCKET_HAND_SHAKE_MESSAGE_H_INCLUDE_
#define _WEBSOCKET_HAND_SHAKE_MESSAGE_H_INCLUDE_

#include "WebsocketDataMessage.h"

#include <map>

class WebsocketHandshakeMessage : public WebsocketMessage {
public:
	WebsocketHandshakeMessage(char* pRaw, int nSize);
	WebsocketHandshakeMessage();
	~WebsocketHandshakeMessage(void);

	bool Parse();
	
	std::string GetField(std::string key);
	void SetField(std::string key, std::string val);


	std::string Serialize();

private:
	char* pRaw;
	int nSize;
	std::string m_method;
	std::string m_uri;
	std::string m_version;
	std::map<std::string, std::string> fields;
};

#endif


