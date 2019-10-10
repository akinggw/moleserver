#include "CTcpSocketClient.h"
#include "CircularBuffer.h"
#include "MolMessageIn.h"
#include "MolMessageOut.h"
#include "gettimeofday.h"
#include "rc4.h"
#include "ThreadPool.h"

#ifdef _WIN32
	#pragma comment(lib, "ws2_32.lib")
#else

#endif

#pragma pack(push, 1)
typedef struct
{
	uint16 opcode;
	uint32 size;
	uint16 compresss;
	uint32 checksum;
}logonpacket;
#pragma pack(pop)

#define MOL_NETWORK_VERSION 100                     // 网络消息协议版本
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1

initialiseSingleton(CMolGameTcpSocketClient);
initialiseSingleton(CMolChatTcpSocketClient);
initialiseSingleton(CTcpSocketClientManager);

static Thread* pThread = NULL;

/// 构造函数
CMolTcpSocketClient::CMolTcpSocketClient()
	:m_Socket(NULL),m_bConnectState(NOCONNECT),m_ReadBuffer(NULL),
	remaining(0),opcode(0),compress(0),mchecksum(0),sendedhearthintcount(0)
{
	FD_ZERO(&m_readableSet);
	FD_ZERO(&m_writeableSet);
	FD_ZERO(&m_exceptionSet);

	remaining = 0;
	compress = 0;
	opcode = 0;
	mchecksum=0;

	m_ReadBuffer = new CircularBuffer();
	m_ReadBuffer->Allocate(REV_SIZE);

	m_mainlooprunning=true;
	pThread = ThreadPool.StartThread(this);
}

/// 析构函数
CMolTcpSocketClient::~CMolTcpSocketClient(void)
{
	ClearMesList();
	CloseConnect();

	m_mainlooprunning=false;
	ThreadPool.ThreadExit(pThread);

#ifndef _WIN32
	usleep(100000);
#else
	Sleep(1000);
#endif

	delete m_ReadBuffer;
	m_ReadBuffer = NULL;
}

void CMolTcpSocketClient::ExitWorkingThread(void)
{

}

/// 关闭连接
void CMolTcpSocketClient::CloseConnect(bool isShow)
{
	if(m_bConnectState == NOCONNECT) return;

	m_ReadBufferLock.Acquire();
	bool bClose = (m_Socket != INVALID_SOCKET);
	m_bConnectState = NOCONNECT;
	if(m_Socket != INVALID_SOCKET)
	{
#ifdef _WIN32
	    shutdown(m_Socket, SD_BOTH);
		LINGER lingerStruct;
		lingerStruct.l_onoff = 1;
		lingerStruct.l_linger = 0;
		setsockopt(m_Socket, SOL_SOCKET, SO_LINGER,
			(char *)&lingerStruct, sizeof(lingerStruct) );
		CancelIo((HANDLE)m_Socket);
		closesocket(m_Socket);
#else
		shutdown(m_Socket, SHUT_RDWR);
		close(m_Socket);
#endif
		m_Socket = INVALID_SOCKET;
	}

	if(m_ReadBuffer && m_ReadBuffer->GetSize() > 0)
		m_ReadBuffer->Remove(m_ReadBuffer->GetSize());

	if(isShow) PushMessage(MessageStru(MES_TYPE_ON_DISCONNECTED,m_Socket));
	m_ReadBufferLock.Release();
	//::OutputDebugString(TEXT("Is running??\n"));
}

/**
 * 清空消息列表
 */
void CMolTcpSocketClient::ClearMesList(void)
{
	if(_MesList.empty()) return;

	_mesLock.Acquire();
	std::list<MessageStru>::iterator iter = _MesList.begin();
	for(;iter!=_MesList.end();++iter)
	{
		if((*iter).mes)
			delete (*iter).mes;
	}
	_MesList.clear();
	_mesLock.Release();
}

/**
 * 发送数据
 *
 * @param msg 要发送的数据
 *
 * @return 返回数据发送的数量，如果数据全部发送成功返回数据的大小，否则返回SOCKET_ERROR
 */
int CMolTcpSocketClient::Send(CMolMessageOut &msg)
{
	if(msg.getData() == NULL ||
		msg.getLength() <= 0 ||
		msg.getLength() >= MOL_REV_BUFFER_SIZE_TWO)
		return SOCKET_ERROR;

	if(m_Socket == INVALID_SOCKET) return SOCKET_ERROR;

	int iSendCount = SOCKET_ERROR;
	int uSended,uSendSize,iErrorCode;

	uSended = iErrorCode = 0;
	uSendSize = msg.getLength();

	char combuf[MOL_REV_BUFFER_SIZE_TWO];
	memset(combuf,0,MOL_REV_BUFFER_SIZE_TWO);

	memcpy(combuf+sizeof(logonpacket),(uint8*)msg.getData(),uSendSize);

	uint16 pchecksum = checksum((uint16*)(combuf+sizeof(logonpacket)), uSendSize);

	int len = Rc4Encrypt(RC4_KEY,(uint8*)combuf+sizeof(logonpacket),(uint8*)combuf+sizeof(logonpacket), uSendSize );

	bool isCompress = false;

	//压缩
	//if(len > 1024)
	//{
	//	isCompress = true;
	//	len = mole2d::network::CompressData((uint8*)combuf+sizeof(logonpacket),(uint8*)combuf+sizeof(logonpacket), len);
	//}

	logonpacket header;
	header.opcode = MOL_NETWORK_VERSION;
	header.size = len;
	header.compresss = (int)isCompress;
	header.checksum = pchecksum;

	// 先拷贝包头
	memcpy(combuf,&header,sizeof(logonpacket));

	// 加密数据
	//Encrypto((unsigned char*)msg.getData(),uSendSize);

    return Send(combuf,uSendSize);
}

int CMolTcpSocketClient::Send(char *msg,uint32 len)
{
    if(msg == NULL || len <= 0)
        return 0;

	if(m_Socket == INVALID_SOCKET) return SOCKET_ERROR;

	int iSendCount = SOCKET_ERROR;
	int uSended,uSendSize,iErrorCode;

	uSended = iErrorCode = 0;

	uSendSize = len;

	_sendLock.Acquire();

	if(uSendSize >= MOL_REV_BUFFER_SIZE_TWO)
	{
		_sendLock.Release();
		return SOCKET_ERROR;
	}

	try
	{
		do
		{
			iErrorCode = ::send(m_Socket,(msg+uSended),uSendSize-uSended,0);
			if(iErrorCode <= SOCKET_ERROR)
			{
#ifdef _WIN32
				if(::WSAGetLastError() == WSAEWOULDBLOCK)
				{
					if(iSendCount++ > 100)
					{
						_sendLock.Release();
						return SOCKET_ERROR;
					}
					else
					{
						Sleep(10);
						continue;
					}
				}
				else
				{
					_sendLock.Release();
					return SOCKET_ERROR;
				}
#else
                if(iSendCount++ > 100)
				{
					_sendLock.Release();
                    return SOCKET_ERROR;
				}
				else
                {
                    usleep(10000);
                    continue;
                }
#endif
			}

			uSended += iErrorCode;
			iSendCount = 0;
		} while (uSended < uSendSize);
	}
	catch(...)
	{

	}
	_sendLock.Release();

	//::OutputDebugString(TEXT("Is running2?\n"));

	return uSendSize;
}

int CMolTcpSocketClient::Sendhtml5(char *Bytes,uint32 Size)
{
	if(Bytes == NULL || Size <= 0)
		return false;

	CMolMessageOut out;

	int64 payloadSize = Size;

	int expectedSize = payloadSize + 1; //flags byte.
	if(payloadSize <= 125  && payloadSize <= 65535 )
		expectedSize += 1;
	else if(payloadSize > 125  && payloadSize <= 65535)
		expectedSize += 3;
	else
		expectedSize += 9;

	//create the flags byte
	uint8 payloadFlags = 129;
	out.writeBytes(&payloadFlags, 1);

	//create the length byte
	if (payloadSize <= 125)
	{
		uint8 basicSize = payloadSize;
		out.writeBytes(&basicSize, 1);
	}
	else if (payloadSize > 125 & payloadSize <= 65535)
	{
		uint8 basicSize = 126;
		out.writeBytes(&basicSize, 1);

		uint8 len[2];
		len[0] = ( payloadSize >> 8 ) & 255;
		len[1] = ( payloadSize ) & 255;
		out.writeBytes(len, 2);
	}
	else
	{
		uint8 basicSize = 127;
		out.writeBytes(&basicSize, 1);

		uint8 len[8];
		len[0] = ( payloadSize >> 56 ) & 255;
		len[1] = ( payloadSize >> 48  ) & 255;
		len[2] = ( payloadSize >> 40 ) & 255;
		len[3] = ( payloadSize >> 32  ) & 255;
		len[4] = ( payloadSize >> 24 ) & 255;
		len[5] = ( payloadSize >> 16  ) & 255;
		len[6] = ( payloadSize >> 8 ) & 255;
		len[7] = ( payloadSize ) & 255;
		out.writeBytes(len, 8);
	}

	out.writeBytes((uint8*)Bytes,Size);

	return Send(out.getData(),out.getLength());
}

/// 连接指定的服务器
bool CMolTcpSocketClient::Connect(std::string ipaddress,int port)
{
	if(ipaddress.empty())
		return false;

	if(IsConnected())
		CloseConnect(true);

	//::OutputDebugString(TEXT("socket running:7.\n"));

	remaining = 0;
	compress = 0;
	opcode = 0;
	mchecksum=0;
	sendedhearthintcount=0;
	ClearMesList();

#ifdef _WIN32
	long dwServerIP = inet_addr(ipaddress.c_str());
	if(dwServerIP == INADDR_NONE)
	{
		LPHOSTENT lpHost = ::gethostbyname(ipaddress.c_str());
		if(lpHost != NULL) return false;

		dwServerIP = ((LPIN_ADDR)(lpHost->h_addr))->s_addr;
	}

	m_Socket = ::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	unsigned int option = 1;
	setsockopt(m_Socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, 4);

	u_long arg = 1;
	::ioctlsocket(m_Socket, FIONBIO, &arg);

	int time_out = 5000;
	setsockopt(m_Socket,SOL_SOCKET,SO_RCVTIMEO,(char*)&time_out,sizeof(int));
	setsockopt(m_Socket,SOL_SOCKET,SO_SNDTIMEO,(char*)&time_out,sizeof(int));

	sockaddr_in SocketAddr;
	SocketAddr.sin_family = AF_INET;
	SocketAddr.sin_port = htons(port);
	SocketAddr.sin_addr.S_un.S_addr = dwServerIP;

	if((::WSAConnect(m_Socket,(const sockaddr*)(&SocketAddr),sizeof(SocketAddr),NULL,NULL,NULL,NULL) == SOCKET_ERROR) &&
		(::WSAGetLastError() != WSAEWOULDBLOCK))
	{
		CloseConnect(true);
		return false;
	}
#else
    struct hostent * lpHost = gethostbyname(ipaddress.c_str());
    if(lpHost == NULL) return false;

	m_Socket = ::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(m_Socket < 0)
        return false;

	unsigned int option = 1;
	setsockopt(m_Socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, 4);

	u_long arg = 1;
	::ioctl(m_Socket, FIONBIO, &arg);

	int time_out = 5000;
	setsockopt(m_Socket,SOL_SOCKET,SO_RCVTIMEO,(char*)&time_out,sizeof(int));
	setsockopt(m_Socket,SOL_SOCKET,SO_SNDTIMEO,(char*)&time_out,sizeof(int));

	sockaddr_in SocketAddr;
	SocketAddr.sin_family = AF_INET;
	SocketAddr.sin_port = ntohs((u_short)port);
	memcpy(&SocketAddr.sin_addr.s_addr , lpHost->h_addr_list[0], lpHost->h_length);

    int sockfd = connect(m_Socket,(const sockaddr*)(&SocketAddr),sizeof(SocketAddr)) ;
	//if(sockfd < 0)
	//{
	//	CloseConnect();
	//	return false;
	//}

#endif

	//::OutputDebugString(TEXT("socket running:8.\n"));

	m_bConnectState = CONNECTTING;

	//::OutputDebugString(TEXT("socket running:9.\n"));

	return true;
}

int CMolTcpSocketClient::GetNetMessage(NetMessage & mes)
{
	mes.Clear();

	if(GetMesCount() <= 0 ||
		mes.GetMaxCount() <= 0)
		return 0;

	//if(!IsConnected()) return 0;

	int count = 0;

	// 如果当前系统中的消息个数小于我们要读取的个数时，读取全部的消息；
	// 否则读取我们设置的消息个数的消息
	if(GetMesCount() < mes.GetMaxCount())
	{
		if(_mesLock.AttemptAcquire())
		{
			std::list<MessageStru> *meslist = GetMesList();
			if(meslist == NULL || meslist->empty())
			{
				_mesLock.Release();
				return 0;
			}

			std::list<MessageStru>::iterator iter = meslist->begin();
			for(;iter != meslist->end();)
			{
				mes.AddMessage((*iter));
				iter = meslist->erase(iter);
				count+=1;
			}
			_mesLock.Release();
		}
	}
	else
	{
		if(_mesLock.AttemptAcquire())
		{
			std::list<MessageStru> *meslist = GetMesList();
			if(meslist == NULL || meslist->empty())
			{
				_mesLock.Release();
				return 0;
			}

			std::list<MessageStru>::iterator iter = meslist->begin();
			for(int i=0;iter != meslist->end(),i<mes.GetMaxCount();i++)
			{
				if(iter == meslist->end()) break;

				mes.AddMessage((*iter));
				iter = meslist->erase(iter);
				count+=1;
			}

			_mesLock.Release();
		}
	}

	return count;
}

void CMolTcpSocketClient::ProcessSelect(void)
{
	if(!IsConnected())
		return;

	int nfds;
	timeval timeout;
	timeout.tv_usec = 20000;
	timeout.tv_sec = 0;

	FD_ZERO(&m_readableSet);
	FD_ZERO(&m_writeableSet);
	FD_ZERO(&m_exceptionSet);
	FD_SET(m_Socket, &m_readableSet);
	FD_SET(m_Socket, &m_writeableSet);

	nfds = select(FD_SETSIZE, &m_readableSet, &m_writeableSet, &m_exceptionSet, &timeout);

	if(nfds < 0)
	{
#ifdef _WIN32
		printf("Exception in select() somewhere.. :/ %u\n", WSAGetLastError());
#endif
		CloseConnect(true);
	}
	else if(nfds == 0)
	{
		// 如果连着发5个心跳包都失败，那么连接已经断开了
		if(sendedhearthintcount >= 5)
			CloseConnect(true);
	}
	else if(nfds > 0)
	{
		if(FD_ISSET(m_Socket, &m_readableSet))
		{
			m_ReadBufferLock.Acquire();
			int iLen = ::recv(m_Socket,(char*)(m_ReadBuffer->GetBuffer()),(int)m_ReadBuffer->GetSpace(),0);
			if(iLen == SOCKET_ERROR || iLen == 0)
			{
				CloseConnect(true);
				m_ReadBufferLock.Release();
				return;
			}

			try
			{
				if(iLen != 0xFFFFFFFF)
					m_ReadBuffer->IncrementWritten(iLen);

				while(true)
				{
					if(!remaining)
					{
						if(m_ReadBuffer->GetSize() < sizeof(logonpacket))
						{
							m_ReadBufferLock.Release();
							return;
						}

						// 首先取得版本号
						m_ReadBuffer->Read((unsigned char*)&opcode,sizeof(unsigned short));

						if(opcode != MOL_NETWORK_VERSION)
						{
							CloseConnect(true);
							m_ReadBufferLock.Release();
							return;
						}

						// 首先取得包头
						m_ReadBuffer->Read((unsigned char*)&remaining,sizeof(unsigned int));

						// 取得数据压缩标志
						m_ReadBuffer->Read((unsigned char*)&compress,sizeof(unsigned short));

						// 取得数据效验标志
						m_ReadBuffer->Read((uint8*)&mchecksum,sizeof(uint32));
					}

					if(m_ReadBuffer->GetSize() < remaining/* || m_ReadBuffer->GetSize() >= MOL_REV_BUFFER_SIZE*/)
					{
						//size_t length = m_ReadBuffer->GetSize();

						//CloseConnect();

						//// 处理与客户端建立连接
						//if(m_BaseFrame)
						//{
						//	m_BaseFrame->OnProcessNetMessage(m_bConnectState,in);
						//}
						m_ReadBufferLock.Release();
						return;
					}

					m_bConnectState = MESPROCESS;
					char buffer[MOL_REV_BUFFER_SIZE_TWO];
					memset(buffer,0,MOL_REV_BUFFER_SIZE_TWO);

					// 取得实际数据包
					m_ReadBuffer->Read((unsigned char*)buffer,remaining);

					int len = remaining;
					//Decrypto((unsigned char*)buffer,dlength);

					////解压缩
					//if(compress > 0)
					//	len = mole2d::network::UncompressData((uint8*)buffer,(uint8*)buffer, len );

					len = Rc4Decrypt(RC4_KEY,(uint8*)buffer,(uint8*)buffer, len );

					//校研数据
					if(mchecksum != checksum((uint16*)buffer, len))
					{
						CloseConnect(true);
						m_ReadBufferLock.Release();
						return;
					}

					// 让主窗体来处理这个网络消息
					if(len > 0 && len < MOL_REV_BUFFER_SIZE_TWO)
						PushMessage(MessageStru(MES_TYPE_ON_READ,m_Socket,new CMolMessageIn(buffer,len)));

					remaining = 0;
					compress = 0;
					opcode = 0;
					mchecksum = 0;
				}
			}
			catch (...)
			{
				CloseConnect(true);
			}
			m_ReadBufferLock.Release();
		}

		if(FD_ISSET(m_Socket, &m_writeableSet))
		{

		}

		if(FD_ISSET(m_Socket, &m_exceptionSet))
			CloseConnect(true);
	}
}

void CMolTcpSocketClient::GameMainLoop(void)
{
	//::OutputDebugString(TEXT("socket running:1.\n"));

#ifdef _WIN32
		gettimeofday(&lostHeartHintTime);
#else
		gettimeofday(&lostHeartHintTime, NULL);
#endif

	while(IsConnected())
	{
		//::OutputDebugString(TEXT("socket running:5.\n"));

#ifdef _WIN32
		MSG msg;   //增加一个MSG的变量msg来接收消息
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){      //将消息队列里的消息逐个读入msg
			if(msg.message==WM_QUIT){     //如果收到终止消息则退出
				break;      //线程正常返回，会释放局部变量等内存资源
			}
		}
#endif

		//::OutputDebugString(TEXT("socket running:2.\n"));

		ProcessSelect();

		//::OutputDebugString(TEXT("socket running:3.\n"));

		// 向服务器发送心跳消息
		struct timeval now;

#ifdef _WIN32
		gettimeofday(&now);
#else
		gettimeofday(&now, NULL);
#endif
		long temp = now.tv_sec - lostHeartHintTime.tv_sec;
		if(temp > 5)
		{
#ifdef _WIN32
		gettimeofday(&lostHeartHintTime);
#else
		gettimeofday(&lostHeartHintTime, NULL);
#endif

			CMolMessageOut out(100);
			if(Send(out) == SOCKET_ERROR)
				sendedhearthintcount+=1;
			else
				sendedhearthintcount=0;
		}

#ifdef _WIN32
		Sleep(1);
#else
		usleep(1000);
#endif

		//::OutputDebugString(TEXT("socket running:4.\n"));
	}
}

bool CMolTcpSocketClient::run()
{
	while(m_mainlooprunning)
	{
		GameMainLoop();

#ifdef _WIN32
		Sleep(1);
#else
		usleep(1000);
#endif
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

CTcpSocketClientManager::CTcpSocketClientManager()
{
#ifdef _WIN32
	unsigned short versionRequested = MAKEWORD (2, 2);
	WSADATA wsaData;

	if (WSAStartup (versionRequested, & wsaData))
		return;

	if (LOBYTE (wsaData.wVersion) != 2||
		HIBYTE (wsaData.wVersion) != 2)
	{
		WSACleanup ();
		return;
	}
#else

#endif

	ThreadPool.Startup();
}

CTcpSocketClientManager::~CTcpSocketClientManager()
{
	ExitWorkingThread();
	deleteAllTcpSocketClient();
	ThreadPool.Shutdown();

#ifdef _WIN32
	WSACleanup ();
#else

#endif
}

void CTcpSocketClientManager::ExitWorkingThread(void)
{
	if(m_TcpSocketClients.empty())
		return;

	for(int i=0;i<(int)m_TcpSocketClients.size();i++)
	{
		if(m_TcpSocketClients[i])
		{
			m_TcpSocketClients[i]->ExitWorkingThread();
			//ThreadPool.KillFreeThreads(ThreadPool.GetFreeThreadCount());
		}
	}
}

void CTcpSocketClientManager::deleteAllTcpSocketClient(void)
{
	if(m_TcpSocketClients.empty())
		return;

	for(int i=0;i<(int)m_TcpSocketClients.size();i++)
	{
		if(m_TcpSocketClients[i]) delete m_TcpSocketClients[i];
		m_TcpSocketClients[i] = NULL;
	}

	m_TcpSocketClients.clear();
}

bool CTcpSocketClientManager::addTcpSocketClient(CMolTcpSocketClient *pClient)
{
	if(pClient == NULL) return false;

	m_TcpSocketClients.push_back(pClient);

	return true;
}

bool CTcpSocketClientManager::delTcpSocketClient(CMolTcpSocketClient *pClient)
{
	if(pClient == NULL || m_TcpSocketClients.empty()) return false;

	std::vector<CMolTcpSocketClient*>::iterator iter = m_TcpSocketClients.begin();
	for(;iter != m_TcpSocketClients.end();++iter)
	{
		if((*iter) == pClient)
		{
			delete (*iter);
			m_TcpSocketClients.erase(iter);
			return true;
		}
	}

	return false;
}
