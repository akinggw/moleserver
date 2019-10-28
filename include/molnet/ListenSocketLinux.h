/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * ListenSocket<T>: Creates a socket listener on specified address and port,
 *                  requires Update() to be called every loop.
 *
*/

#ifndef _LISTENSOCKET_H
#define _LISTENSOCKET_H
#ifdef CONFIG_USE_EPOLL

#include "SocketDefines.h"
#include <errno.h>

struct ClientLogin
{
    ClientLogin():m_LoginTimer(0),m_LoginCount(0) {}
    ClientLogin(uint32 lt, uint32 lc):m_LoginTimer(lt),m_LoginCount(lc) {}

    uint32 m_LoginTimer;
    uint32 m_LoginCount;
};

class ListenSocketBase
{
	public:
		virtual ~ListenSocketBase() {}
		virtual void OnAccept() = 0;
		virtual int GetFd() = 0;
};

template<class T>
class ListenSocket : public ListenSocketBase
{
	public:
		ListenSocket() : ListenSocketBase()
		{

		}

		~ListenSocket()
		{
			if(m_opened)
				SocketOps::CloseSocket(m_socket);
		}

		void Close()
		{
			if(m_opened)
				SocketOps::CloseSocket(m_socket);
			m_opened = false;
		}

		bool Start(const char* ListenAddress, uint32 Port)
		{
			m_socket = socket(AF_INET, SOCK_STREAM, 0);
			SocketOps::ReuseAddr(m_socket);
			SocketOps::Nonblocking(m_socket);
			SocketOps::SetTimeout(m_socket, 60);

			m_address.sin_family = AF_INET;
			m_address.sin_port = ntohs((u_short)Port);
			m_address.sin_addr.s_addr = htonl(INADDR_ANY);
			m_opened = false;

			if(strcmp(ListenAddress, "0.0.0.0"))
			{
				struct hostent* hostname = gethostbyname(ListenAddress);
				if(hostname != 0)
					memcpy(&m_address.sin_addr.s_addr, hostname->h_addr_list[0], hostname->h_length);
			}

			// bind.. well attempt to.
			int ret = ::bind(m_socket, (const sockaddr*)&m_address, sizeof(m_address));
			if(ret != 0)
			{
				sLog.outError("Bind unsuccessful on port %u.", (unsigned int)Port);
				perror("bind");
				return false;
			}

			ret = listen(m_socket, 256);
			if(ret != 0)
			{
				sLog.outError("Unable to listen on port %u.", (unsigned int)Port);
				return false;
			}
			len = sizeof(sockaddr_in);
			m_opened = true;
			sSocketMgr.AddListenSocket(this);

			return true;
		}

		/*void Update()
		{
		    aSocket = accept(m_socket, (sockaddr*)&m_tempAddress, (socklen_t*)&len);
		    if(aSocket == -1)
		        return;

		    dsocket = new T(aSocket);
		    dsocket->Accept(&m_tempAddress);
		}*/

		void OnAccept()
		{
		    while(true)
		    {
                aSocket = accept(m_socket, (sockaddr*)&m_tempAddress, (socklen_t*)&len);
                if(aSocket == -1)
                    return;

                std::string ipaddress = (char*)inet_ntoa(m_tempAddress.sin_addr);
                std::map<std::string,ClientLogin>::iterator iter = m_clientsList.find(ipaddress);

                if(iter == m_clientsList.end())
                {
                    m_clientsList[ipaddress] = ClientLogin((uint32)time(0),1);
                }
                else
                {
                    if((*iter).second.m_LoginTimer == 0)
                        (*iter).second.m_LoginTimer = (uint32)time(0);
                    (*iter).second.m_LoginCount+=1;

                    uint32 tmpTime = (uint32)time(0) - (*iter).second.m_LoginTimer;

                    bool isOk = true;

                    if(tmpTime >= 5)
                    {
                        if((*iter).second.m_LoginCount <= 50)
                        {
                            m_clientsList.erase(iter);
                            isOk = false;
                        }
                    }

                    if(isOk && (*iter).second.m_LoginCount > 10)
                    {
                        SocketOps::CloseSocket(aSocket);

                        (*iter).second.m_LoginTimer = 0;

                        continue;
                    }
                }

                //dsocket = new T(aSocket);
                //dsocket->Accept(&m_tempAddress);
                T * socket = (T*)sSocketMgr.GetFreeSocket();

                try
                {
                    if(socket == NULL)
                        socket = new T(aSocket);
                }
                catch (std::exception e)
                {
                    char str[256];
                    sprintf(str,"接收socket异常%s:\n",e.what());
                    LOG_DEBUG(str);
                    //perr->Delete();

                    if(socket)
                    {
                        delete socket;
                        socket = NULL;
                    }
                }

                if(socket)
                {
                    socket->SetFd(aSocket);
                    socket->Accept(&m_tempAddress);
                }
		    }
		}

		inline bool IsOpen() { return m_opened; }
		int GetFd() { return m_socket; }

	private:
		SOCKET m_socket;
		SOCKET aSocket;
		struct sockaddr_in m_address;
		struct sockaddr_in m_tempAddress;
		bool m_opened;
		uint32 len;
		std::map<std::string,ClientLogin> m_clientsList;
};

extern ListenSocket<NetClient> *m_ServerSocket;

#endif
#endif

