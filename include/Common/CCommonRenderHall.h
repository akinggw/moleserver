#ifndef _C_COMMON_RENDER_HALL_H_INCLUDE_
#define _C_COMMON_RENDER_HALL_H_INCLUDE_

#include "ccommon.h"

class CCommonRenderDevice;
class CCommonSocket;
class CMolMessageIn;
class Player;
class Room;

/** 
 * 通用绘图框架，只用于大厅的绘制
 */
class CCommonRenderHall
{
public:
	/// 构造函数
	CCommonRenderHall() { }
	/// 析构函数
	virtual ~CCommonRenderHall() {}

	/// 设置渲染设备
	virtual void SetDevice(CCommonRenderDevice *pDevice) = 0;
	/// 游戏资源加载
	virtual bool LoadGameResources(void) = 0;
	/// 卸载游戏资源
	virtual bool ShutdownGameResources(void) = 0;
	/// 重设游戏场景
	virtual void ResetGameScene(void) = 0;
	/// 游戏场景绘制
	virtual void DrawGameScene(void) = 0;
	/// 游戏中事件处理
	virtual bool OnEvent(const SEvent& event) = 0;
	/// 处理用户定时器消息
	virtual void OnProcessTimerMsg(int timerId,int curTimer) = 0;

	/// 处理用户大厅登陆信息
	virtual void OnProcessUserFrameLoginMes(CMolMessageIn *mes) = 0;
	/// 处理用户大厅获取在线游戏服务器列表信息
	virtual void OnProcessUserFrameGameServerListMes(CMolMessageIn *mes) = 0;
	/// 处理用户大厅获取在线游戏列表信息
	virtual void OnProcessUserFrameGameListMes(CMolMessageIn *mes) = 0;
	/// 处理用户登陆游戏服务器信息
	virtual void OnProcessUserGameServerLoginMes(CMolMessageIn *mes) = 0;
	/// 处理游戏服务器框架信息
	virtual void OnProcessGameFrameMes(CMolMessageIn *mes) = 0;
	/// 处理游戏服务器房间信息
	virtual void OnProcessGameRoomMes(CMolMessageIn *mes) = 0;
	/// 处理获取当前游戏的在线服务器人数
	virtual void OnProcessGetGameServerOnlineCount(CMolMessageIn *mes) = 0;
};

#endif