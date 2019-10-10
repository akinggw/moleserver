#ifndef _C_TCP_SOCKET_CLIENT_H_INCLUDE_
#define _C_TCP_SOCKET_CLIENT_H_INCLUDE_

#include "common.h"
#include "Singleton.h"
#include "Mutex.h"
#include "ThreadStarter.h"
#include "MolNetMessage.h"
#include <string>
#include <list>

#define REV_SIZE      30000                     // 接收数据的缓冲大小

class CMolMessageIn;
class CMolMessageOut;
class CircularBuffer;

/**
* 当前socket的连接状态
*/
enum ConnectState
{
	NOCONNECT = 0,     // 没有连接
	CONNECTTING,       // 连接中
	CONNECTED,         // 连接上
	MESPROCESS         // 网络消息处理
};

class CMolTcpSocketClient : public ThreadBase
{
public:
	/// 构造函数
	CMolTcpSocketClient();
	/// 析构函数
	~CMolTcpSocketClient(void);

	/// 关闭连接
	void CloseConnect(bool isShow=false);
	/// 连接指定的服务器
	bool Connect(std::string ipaddress,int port);
		/// 返回当前连接状态
	inline ConnectState GetConnectState(void) { return m_bConnectState; }
	/// 发送数据
	int Send(CMolMessageOut &msg);
	int Send(char *msg,uint32 len);
	int Sendhtml5(char *Bytes,uint32 len);
	/// 检测当前是否还在连接中
	inline bool IsConnected(void)
	{
		return m_bConnectState != NOCONNECT ? true : false;
	}

	int GetNetMessage(NetMessage & mes);
	void ExitWorkingThread(void);

private:
	virtual bool run();
	/// 得到消息列表
	inline std::list<MessageStru>* GetMesList(void)
	{
		return &_MesList;
	}
	/// 添加一个消息到列表中
	inline void PushMessage(MessageStru mes)
	{
		_mesLock.Acquire();
		_MesList.push_back(mes);
		//++_mesLock_count;
		_mesLock.Release();
	}
	/// 得到当前消息个数
	inline int GetMesCount(void)
	{
		return (int)_MesList.size();
	}
	/// 清空消息列表
	void ClearMesList(void);

private:
	void ProcessSelect(void);
	void GameMainLoop(void);

private:
	int m_Socket;                 /**< 当前控件的socket句柄 */
	ConnectState m_bConnectState;    /**< 当前的连接状态 */
	CircularBuffer *m_ReadBuffer;  /**< 接收数据缓冲区 */

	std::list<MessageStru> _MesList;
	Mutex _mesLock,_sendLock,m_ReadBufferLock;

	fd_set m_readableSet,m_writeableSet;
	fd_set m_exceptionSet;
	struct timeval lostHeartHintTime;
	volatile int sendedhearthintcount;

	volatile bool m_mainlooprunning;
	unsigned int remaining;
	unsigned short opcode;
	uint16 compress;
	uint32 mchecksum;
};

class CTcpSocketClientManager : public Singleton<CTcpSocketClientManager>
{
public:
	CTcpSocketClientManager();
	~CTcpSocketClientManager();

	bool addTcpSocketClient(CMolTcpSocketClient *pClient);
	bool delTcpSocketClient(CMolTcpSocketClient *pClient);
	void ExitWorkingThread(void);
	void deleteAllTcpSocketClient(void);

private:
	std::vector<CMolTcpSocketClient*> m_TcpSocketClients;
};

class CMolGameTcpSocketClient : public CMolTcpSocketClient,public Singleton<CMolGameTcpSocketClient>
{
public:
	CMolGameTcpSocketClient() {}
	virtual ~CMolGameTcpSocketClient() {}
};

class CMolChatTcpSocketClient : public CMolTcpSocketClient,public Singleton<CMolChatTcpSocketClient>
{
public:
	CMolChatTcpSocketClient() {}
	virtual ~CMolChatTcpSocketClient() {}
};

#define MolTcpSocketClient CMolGameTcpSocketClient::getSingleton()
#define MolChatTcpSocketClient CMolChatTcpSocketClient::getSingleton()

#endif
