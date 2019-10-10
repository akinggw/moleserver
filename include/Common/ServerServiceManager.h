#ifndef _SERVER_SERVICE_MANAGER_H_INCLUDE_
#define _SERVER_SERVICE_MANAGER_H_INCLUDE_

class ServerServiceManager
{
public:
	/// 构造函数
	ServerServiceManager() {}
	/// 析构函数
	virtual ~ServerServiceManager() {}

	/// 设置玩家人数
	virtual void SetGamePlayerCount(int count) = 0;
	/// 得到玩家人数
	virtual int GetGamePlayerCount(void) = 0;
	/// 设置房间标识
	virtual void SetRoomMarking(int marking) = 0;
	/// 得到房间标识
	virtual int GetRoomMarking(void) = 0;
	/// 设置游戏名称
	virtual void SetGameName(const char* name) = 0;
	/// 得到游戏名称
	virtual const char* GetGameName(void) = 0;
	/// 设置游戏客户端组件名称
	virtual void SetGameModuleName(const char* name) = 0;
	/// 得到游戏客户端组件名称
	virtual const char* GetGameModuleName(void) = 0;
	/// 设置房间游戏类型
	virtual void SetRoomGameType(enStartMode type) = 0;
	/// 得到房间游戏类型
	virtual enStartMode GetRoomGameType(void) = 0;
	/// 设置房间最小进入倍数
	virtual void SetRoomLastDouble(int pdouble) = 0;
	/// 得到房间最小进入倍数
	virtual int GetRoomLastDouble(void) = 0;
	/// 设置服务器端口号
	virtual void SetServerPort(int pport) = 0;
	/// 得到服务器端口号
	virtual int GetServerPort(void) = 0;

	//创建接口
public:
	//创建桌子
	virtual void * CreateTableFrameSink(void) = 0;
	//创建机器
	virtual void * CreateAndroidUserItemSink(void) = 0;
};

#endif
