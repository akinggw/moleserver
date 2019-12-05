#ifndef _C_GAME_LOGIC_FRAME_H_INCLUDE_
#define _C_GAME_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"
#include "GameLogic.h"
#include <map>
#include <vector>

#include "czhuangmanager.h"

#define IDD_TIMER_GAME_STARTING   IDD_TIMER_GAME_START+1                   // 游戏卡开始定时器
#define IDD_TIMER_GAME_XIAZHU     IDD_TIMER_GAME_START+2                   // 下注
#define IDD_TIMER_GAME_KAIPAI     IDD_TIMER_GAME_START+3                   // 开牌
#define IDD_TIMER_GAME_JIESUAN    IDD_TIMER_GAME_START+4                   // 结算

class CServerLogicFrame : public ServerLogicFrame
{
public:
	/// 构造函数
	CServerLogicFrame();
	/// 析构函数
	~CServerLogicFrame();

	/// 设置当前应用房间
	virtual void SetGameRoom(Room* pRoom) { m_g_GameRoom = pRoom; }
	/// 用于处理用户开始游戏开始消息
	virtual void OnProcessPlayerGameStartMes();
	/// 用于处理用户进入游戏房间后的消息
	virtual void OnProcessPlayerRoomMes(int playerId,Json::Value &mes);
	/// 处理用户进入房间消息
	virtual void OnProcessEnterRoomMsg(int playerId);
	/// 处理用户离开房间消息
	virtual void OnProcessLeaveRoomMsg(int playerId);
	/// 处理用户断线重连消息
	virtual void OnProcessReEnterRoomMes(int playerId);
	/// 处理用户断线消息
	virtual void OnProcessOfflineRoomMes(int playerId);
	/// 处理用户定时器消息
	virtual void OnProcessTimerMsg(int timerId,int curTimer);

private:
	/// 导入服务器配置
	void LoadServerConfig(void);
	/// 统计各个区域的下注总输赢
	int64 GetRectJettonTotal(tagCardType pCarType,bool isYouRobits=false);
	/// 统计指定玩家的下注总额
	int64 GetUserJettonTotal(uint32 pUserId);
	int64 getusertotalresult(void);
	int64 getuserswinandlostresult(void);
	int masterwinorlost(bool iswin, bool isYouRobits=true,int64 pLastMoney=0);

	/// 得到结果值
	int GetGamingResult(void);
	/// 游戏结束
	void GameEnd(void);

	std::string getCardTypeName(tagCardType ptype)
	{
		switch (ptype)
		{
		case CARDTYPE_QINGLONG:
		{
			return "青龙";
		}
			break;
		case CARDTYPE_BAIHU:
		{
			return "白虎";
		}
			break;
		case CARDTYPE_ZHUQUE:
		{
			return "朱雀";
		}
			break;
		case CARDTYPE_XUANWU:
		{
			return "玄武";
		}
			break;
		case CARDTYPE_XIAOBAILONG:
		{
			return "小白龙";
		}
			break;
		case CARDTYPE_XIAOLAOHU:
		{
			return "小老虎";
		}
			break;
		case CARDTYPE_XIAOFENGHUANG:
		{
			return "小凤凰";
		}
			break;
		case CARDTYPE_XIAOWUGUI:
		{
			return "小乌龟";
		}
			break;
		default:
			break;
		}

		return "";
	}

private:
	Room *m_g_GameRoom;                                  /**< 游戏房间 */

private:
	std::map<tagCardType,int>       m_carBeiLv;               /**< 车型的倍率 */
	std::map<int32,std::map<tagCardType,int64> >      m_xiazhus;          /**< 所有人的下注 */
	GameState                      m_GameState;
	CGameLogic                     m_GameLogic;
	std::vector<int>               m_GameRecords;

	CZhuangManager                 m_ZhuangManager;

	tagCardType                    m_curDecCardType;
	bool                           m_isZhuangYing;
	int                            m_supercontroltype;
	bool                           m_issupercontrol;

	int                            m_timegamestart;
	int                            m_timexiazhu;
	int                            m_timekaipai;
	int                            m_timejiesuan;
	int                            m_timecurlost;
	int                            m_curGamingResult;
};

#endif
