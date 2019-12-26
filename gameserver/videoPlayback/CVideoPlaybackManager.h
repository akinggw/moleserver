#ifndef _C_VIDEO_PLAYBACK_MANAGER_H_INCLUDE_
#define _C_VIDEO_PLAYBACK_MANAGER_H_INCLUDE_

#include <vector>
#include <string>

#include "../../include/molnet/common.h"
#include "../../include/Common/Player.h"
#include "../../include/Common/Room.h"

#define VIDEO_BUFFER_SIZE 2048

/**
 * 录像文件结构
 */
struct FileHeader
{
    char marker[5];             // 文件标识 "X","B","V","O","D"
	char gamelogic[256];        // 游戏逻辑dll
	uint32 useMe;               // 玩家自己的ID
	int wUserCount;             // 玩家个数
	int wFrameCount;            // 帧数
};

/**
 * 录像状态
 */
enum VideoState
{
	VIDEO_RECORDING = 0,                       //录制
	VIDEO_PLAYBACK,                            //回放
	VIDEO_NULL
};

/**
 * 录像中玩家结构
 */
struct VideoGameStep
{
	VideoGameStep()
		: wTimeSpace(0),wDataSize(0)
	{
	}
	VideoGameStep(DWORD ts,const void *buf,WORD size)
		: wTimeSpace(ts)
	{
		if(buf && size < VIDEO_BUFFER_SIZE)
		{
			memcpy(wBuffer,buf,size);
			wDataSize = size;
		}
	}

	DWORD wTimeSpace;                          // 时长
	char wBuffer[VIDEO_BUFFER_SIZE];           // 数据
	WORD wDataSize;                            // 数据长度
};

/**
 * 录像回放用到的玩家结构
 */
struct VPPlayer
{
	VPPlayer():m_Id(0),m_ConnectId(0),m_roomId(0),m_ChairIndex(0),m_isLookOn(false),m_Money(0),m_BankMoney(0),
		m_TotalResult(0),m_Revenue(0),m_level(0),m_experience(0),m_totalbureau(0),m_successbureau(0),m_RunawayBureau(0),
		m_runawayrate(0.0f),m_successrate(0.0f),sex(0),gtype(0),ipaddress(0),m_DeviceType(PLAYERDEVICETYPE_PC)
	{}

	VPPlayer(int id,uint32 connectid,int roomid,int chairindex,PlayerState playerstate,PlayerType playertype,
		bool islookon,std::string name,int64 money,int64 bankmoney,int64 totalresult,int64 revenue,int level,
		unsigned int experience,std::string useravatar,int totalbureau,int successbureau,int runawaybureau,
		int failbureau,float successrate,float runawayrate,int psex,std::string prealname,int pgtype,uint32 pipaddress,
		PlayerDeviceType pdt)
		: m_Id(id),m_ConnectId(connectid),m_roomId(roomid),m_ChairIndex(chairindex),m_PlayerState(playerstate),
		  m_PlayerType(playertype),m_isLookOn(islookon),m_Money(money),m_BankMoney(bankmoney),m_TotalResult(totalresult),
		  m_Revenue(revenue),m_level(level),m_experience(experience),m_totalbureau(totalbureau),
		  m_successbureau(successbureau),m_RunawayBureau(runawaybureau),m_failbureau(failbureau),m_successrate(successrate),
		  m_runawayrate(runawayrate),sex(psex),gtype(pgtype),ipaddress(pipaddress),m_DeviceType(pdt)
	{
		strncpy(m_Name,name.c_str(),128);
		strncpy(m_useravatar,useravatar.c_str(),128);
		strncpy(realname,prealname.c_str(),128);
	}

	int m_Id;         /**< 玩家ID */
	uint32 m_ConnectId;  /**< 玩家的连接ID */
	int m_roomId;     /**< 玩家所在的房间ID */
	int m_ChairIndex;   /**< 用户在房间中的索引 */
	PlayerState m_PlayerState;       /**< 玩家当前状态 */
	PlayerType m_PlayerType;           /**< 玩家类型 */
	PlayerDeviceType m_DeviceType;     /**< 玩家设备类型 */
	bool m_isLookOn;                   /**< 是否为旁观 */

	char m_Name[128];  /**< 玩家的名称 */

	int64 m_Money;              /**< 玩家金钱数量 */
	int64 m_BankMoney;                /**< 银行金钱数量 */
	int64 m_TotalResult;              /**< 玩家总的输赢值 */
	int64 m_Revenue;                  /**< 玩家税收 */
	int m_level;                       /**< 玩家等级 */
	unsigned int m_experience;         /**< 玩家经验值 */
	char m_useravatar[128];         /**< 用户头像 */
	int m_totalbureau;                 /**< 玩家总局 */
	int m_successbureau;               /**< 玩家赢局 */
	int m_RunawayBureau;			   /**< 玩家逃跑次数 */
	int m_failbureau;                  /**< 玩家输局 */
	float m_successrate;               /**< 玩家胜率 */
	float m_runawayrate;               /**< 玩家逃跑率 */

	int sex;                      //玩家性别
	char realname[128];           //真实姓名
	int gtype;                    //玩家类型
	uint32 ipaddress;             /**< 玩家IP */
};

/**
 * 录像回放中用到的房间结构
 */
struct VPRoom
{
	VPRoom():m_Id(0),m_MaxCount(0),m_RoomRevenue(0),m_RoomLastMoney(0),m_masterId(0),m_curPlayerId(0)
	{}

	VPRoom(int id,std::string name,int maxcount,RoomType roomtype,RoomState roomstate,
		enStartMode startmode,float revenue,int lastmoney,int master,int curplayer)
		: m_Id(id),m_MaxCount(maxcount),m_RoomType(roomtype),m_RoomState(roomstate),
		  m_RoomGameType(startmode),m_RoomRevenue(revenue),m_RoomLastMoney(lastmoney),m_masterId(master),
		  m_curPlayerId(curplayer)
	{
		strncpy(m_Name,name.c_str(),128);
	}

	int m_Id;                   /**< 房间ID */
	char m_Name[128];           /**< 房间名称 */
	int m_MaxCount;             /**< 房间中最大玩家个数 */
	RoomType m_RoomType;        /**< 房间类型 */
	RoomState m_RoomState;      /**< 房间当前状态 */
	enStartMode m_RoomGameType; /**< 房间游戏类型 */
	float m_RoomRevenue;        /**< 房间税收 */
	int m_RoomLastMoney;        /**< 房间最小金币 */

	int m_masterId;             /**< 当前房主ID */
	int m_curPlayerId;          /**< 当前玩家ID */
};
class CVideoPlaybackManager
{
public:
	/// 构造函数
	CVideoPlaybackManager();
	/// 析构函数
	~CVideoPlaybackManager();

	/// 保存到文件
	bool Sava(std::string filename);
	/// 载入记录文件
	bool Load(std::string filename);
	/// 清空所有数据
	void Clear(void);

	/// 设置录像状态
	inline void SetState(VideoState state) { m_VideoState = state; }
	/// 得到录像状态
	inline VideoState GetState(void) { return m_VideoState; }
	/// 设置要加载的游戏组件
	inline void SetGameDll(std::string dllname) { m_hDllGameName = dllname; }
	/// 得到要加载的游戏组件
	inline std::string GetGameDll(void) { return m_hDllGameName; }
	/// 清除所有的帧
	void ClearAllFrames(void);
	/// 得到帧数
	inline int GetFrameCount(void) { return (int)m_VideoGameSteps.size(); }
	/// 加入一帧
	inline void AddFrame(VideoGameStep *pGameStep) { m_VideoGameSteps.push_back(pGameStep); }
	/// 得到指定索引的帧
	VideoGameStep* GetFrame(int index);
	/// 得到当前帧索引
	inline int GetCurrentFrame(void) { return m_curFrameIndex; }
	/// 设置当前帧索引
	inline void SetCurrentFrame(int frame) { m_curFrameIndex = frame; }

	/// 加入一个玩家
	inline void AddUser(VPPlayer pUserData) { m_UserDatas.push_back(pUserData); }
	/// 得到指定索引的玩家
	VPPlayer* GetUser(int index);
	/// 添加玩家所在房间
	inline void AddRoom(VPRoom pRoom) { m_mySelfRoom = pRoom; }
	/// 得到玩家所在房间
	inline VPRoom* GetRoom(void) { return &m_mySelfRoom; }
	/// 得到玩家个数
	inline int GetUserCount(void) { return (int)m_UserDatas.size(); }

	/// 设置录像者的ID
	inline void SetMeUserID(uint32 id) { m_mySelfID = id; }
	/// 得到录像者的ID
	inline uint32 GetMeUserID(void) { return m_mySelfID; }
	/// 设置录像者的椅子号
	inline void SetMeUserChairIndex(int32 index) { m_mySelfChairIndex = index; }
	/// 得到录像者的椅子号
	inline int32 GetMeUserChairIndex(void) { return m_mySelfChairIndex; }

private:
	std::vector<VideoGameStep*> m_VideoGameSteps;         // 用于保存每一帧
	std::vector<VPPlayer> m_UserDatas;                      // 玩家数据
	uint32 m_mySelfID;                                    // 玩家自己的ID
	int32 m_mySelfChairIndex;                              // 玩家自己的椅子号
	VPRoom m_mySelfRoom;                                   // 玩家所在的房间
	int m_curFrameIndex;                                  // 当前帧
	VideoState m_VideoState;                              // 当前录像状态
	std::string  m_hDllGameName;                          /**< 要加载游戏的名称 */
};

#endif
