#ifndef _MOL_NET_MESSAGE_H_INCLUDE
#define _MOL_NET_MESSAGE_H_INCLUDE

/**
* MolNet网络引擎
*
* 描述:系统中用到的网络消息
* 作者:akinggw
* 日期:2010.2.13
*/

#include "common.h"

/**
* 网络处理的消息类型
*/
enum MessageType
{
	MES_TYPE_ON_CONNECTED = 0,        // 连接建立成功后
	MES_TYPE_ON_DISCONNECTED,         // 连接断开后
	MES_TYPE_ON_READ,                 // 数据到达后
	MES_TYPE_NULL
};

class CMolMessageIn;

/**
* 系统用到的消息结构
*/
struct MessageStru
{
	MessageStru()
		: type(MES_TYPE_NULL),socket(0),mes(NULL)
	{

	}
	MessageStru(MessageType t,uint32 s)
		: type(t),socket(s),mes(NULL)
	{

	}
	MessageStru(MessageType t,uint32 s,CMolMessageIn *in)
		: type(t),socket(s),mes(in)
	{

	}

	/// 得到消息发送客户端ID
	inline uint32 GetSocket(void)
	{
		return socket;
	}
	/// 得到消息类型
	MessageType GetType(void)
	{
		return type;
	}
	/// 得到客户端发送的消息
	CMolMessageIn* GetMes(void)
	{
		return mes;
	}

	uint32 socket;                    // 消息发送客户端
	MessageType type;                 // 消息类型
	CMolMessageIn *mes;                // 要处理的消息
};

/**
* 我们要处理的消息结构
*/
class NetMessage
{
public:
	/// 带参数的构造函数
	NetMessage(int count=5000);
	/// 析构函数
	~NetMessage();

	/// 设置最大消息个数
	inline void SetMaxCount(int count) { m_MaxCount = count; }
	/// 得到最大消息个数
	inline int GetMaxCount(void) { return m_MaxCount; }
	/// 得到实际消息个数
	inline int GetCount(void) { return (int)m_MesList.size(); }

	/// 添加一个消息到消息列表中
	void AddMessage(MessageStru mes);
	/// 得到消息列表
	std::vector<MessageStru>& GetMessage(void);
	/// 得到指定id的消息
	MessageStru* GetMesById(int id);
	/// 清空消息列表
	void Clear(void);

private:
	int m_MaxCount;               /**< 设置的最大个数 */
	std::vector<MessageStru> m_MesList;       /**< 用于存储消息的消息列表 */
};

#endif
