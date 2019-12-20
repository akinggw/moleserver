#ifndef _C_GAME_LOGIC_FRAME_H_INCLUDE_
#define _C_GAME_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"
#include "GameLogic.h"
#include <map>
#include <vector>

#include "czhuangmanager.h"

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
	void GetPlayerPanData(void);
	void TongJiUserGamingResult(void);
	void startgame(void);
	void readygame(void);
	std::string getCardTypeName(YaZhuType ptype)
	{
		switch(ptype)
		{
		case YAZHUTYPE_TIAN:
			{
				return "天";
			}
			break;
		case YAZHUTYPE_DI:
			{
				return "地";
			}
			break;
		case YAZHUTYPE_XUAN:
			{
				return "玄";
			}
			break;
		case YAZHUTYPE_HUANG:
			{
				return "黄";
			}
			break;
		default:
			break;
		}

		return "";
	}
	int64 getuserareajetton(YaZhuType ptype);
	void masterwinorlost(bool iswin);
	void allareawinorlost(bool iswin);
	void thetypewin(YaZhuType ptype,bool isWin=true);
	void copycardtocards(uint8 *pcard,YaZhuType ptype);
	int64 getrealuserjettontotal(void);
	int64 getuserswinandlostresult(void);
	int64 getuserjettontotal(int pChairId);
	int64 getusertotalresult(void);
	/// 导入服务器配置
	void LoadServerConfig(void);

private:
	Room *m_g_GameRoom;                                  /**< 游戏房间 */

	CGameLogic m_CGameLogic;
	CZhuangManager m_ZhuangManager;

	std::map<int32,std::map<YaZhuType,int64> >        m_xiazhus;
	GameState                                         m_GameState;

	uint8                                              m_cards[25];
	uint8                                              m_zhuangcards[5];
	uint8                                              m_othercards[4][5];

	int                            m_timegamestart;
	int                            m_timexiazhu;
	int                            m_timekaipai;
	int                            m_timejiesuan;
	int                            m_timecurlost;

	bool                           m_issupercontrol;
};

#endif
