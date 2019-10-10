#ifndef _C_COMMON_RENDER_FRAME_H_INCLUDE_
#define _C_COMMON_RENDER_FRAME_H_INCLUDE_

#include "ccommon.h"

class CCommonRenderDevice;
class CCommonSocket;
class CMolMessageIn;
class Player;
class Room;

/** 
 * 通用绘图框架，用于各种场景的绘制
 */
class CCommonRenderFrame
{
public:
	/// 构造函数
	CCommonRenderFrame(SceneType type=SCENE_TYPE_GAMEROOM) { }
	/// 析构函数
	virtual ~CCommonRenderFrame() {}

	/// 设置游戏类型
	virtual void SetSceneType(SceneType type) = 0;
	/// 得到游戏类型
	virtual SceneType GetSceneType(void) = 0;
	/// 设置渲染设备
	virtual void SetDevice(CCommonRenderDevice *pDevice) = 0;
	/// 设置桌子
	virtual void SetGameRoom(Player *pPlayer,Room *pRoom) = 0;
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
	/// 处理系统消息
	virtual void OnProcessSystemMsg(int msgType,CString msg) = 0;
	/// 处理比赛消息
	virtual void OnProcessMatchingMsg(int msgType,CMolMessageIn *mes) = 0;

	///////////////////////////以下函数游戏中使用///////////////////////////////////////

	/// 用于处理用户开始游戏开始消息
	virtual void OnProcessPlayerGameStartMes(void) = 0;
	/// 用于处理用户结束游戏消息
	virtual void OnProcessPlayerGameOverMes(void) = 0;
	/// 用于处理用户进入游戏房间后的消息
	virtual void OnProcessPlayerRoomMes(CMolMessageIn *mes) = 0;
	/// 处理用户进入房间消息
	virtual void OnProcessEnterRoomMsg(int pChairId) = 0;
	/// 处理用户离开房间消息
	virtual void OnProcessLeaveRoomMsg(int pChairId) = 0;
	/// 处理用户断线消息
	virtual void OnProcessOfflineRoomMes(int pChairId) = 0;
	/// 处理用户断线重连消息
	virtual void OnProcessReEnterRoomMes(int pChairId,CMolMessageIn *mes) = 0;

	////////////////////////////////////////////////////////////////////////////////////
};

#endif