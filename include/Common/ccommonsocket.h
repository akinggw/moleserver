#ifndef _C_COMMON_SOCKET_H_INCLUDE_
#define _C_COMMON_SOCKET_H_INCLUDE_

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

class CMolMessageOut;

class CCommonSocket
{
public:
	/// 关闭连接
	virtual void CloseConnect(void) = 0;
	/// 连接指定的服务器
	virtual bool Connect(std::string ipaddress,int port) = 0;
	/// 发送数据
	virtual int Send(CMolMessageOut &msg) = 0;
	/// 返回当前连接状态
	virtual ConnectState GetConnectState(void) = 0;
	/// 检测当前是否还在连接中
	virtual bool IsConnected(void) = 0;
};

#endif
