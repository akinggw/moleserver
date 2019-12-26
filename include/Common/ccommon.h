#ifndef _C_COMMON_H_INCLUDE_
#define _C_COMMON_H_INCLUDE_

#include <jsoncpp/json/json.h>

//////////////////////////////////////////////////////////////////////////
//常用常量

//无效数值
#define INVALID_BYTE				((unsigned char)(0xFF))						//无效数值
#define INVALID_WORD				((unsigned short)(0xFFFF))					//无效数值
#define INVALID_DWORD				((unsigned int)(0xFFFFFFFF))				//无效数值

//////////////////////////////////////////////////////////////////////////
//常用常量

//无效数值
#define INVALID_TABLE				INVALID_WORD						//无效桌子
#define INVALID_CHAIR				INVALID_WORD						//无效椅子

//////////////////////////////////////////////////////////////////////////

//数组维数
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

//无效地址
#define INVALID_IP_ADDRESS(IPAddress) (((IPAddress==0L)||(IPAddress==INADDR_NONE)))

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//接口释放
#define  SafeRelease(pObject) { if (pObject!=NULL) { pObject->Release(); pObject=NULL; } }

//删除指针
#define  SafeDelete(pData) { try { delete pData; } catch (...) { assert(false); } pData=NULL; }

//删除数组
#define  SafeDeleteArray(pData) { try { delete [] pData; } catch (...) { assert(false); } pData=NULL; }

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                   游戏客户端使用的定时器1-1000                               //
//////////////////////////////////////////////////////////////////////////////////
#define IDD_TIMER_HALL_ADVERTISEMENT              100                   // 大厅广告滚动
#define IDD_TIMER_HALL_CONNECTGAMESERVER          101                   // 连接游戏服务器
#define IDD_TIMER_HALL_CONTROL_CHAT               102                   // 控制聊天语速
#define IDD_TIMER_HALL_RECONNECTGAMESERVER        103                   // 掉线重连
#define IDD_TIMER_HALL_ENTER_GAMESERVER           104                   // 进入游戏服务器
#define IDD_TIMER_HALL_HEART                      105                   // 心跳消息
#define IDD_TIMER_HALL_GET_USER_ADDRESS           106                   // 得到玩家地理位置
#define IDD_TIMER_HALL_GETLASTGAMINGNEWS          107                   // 每半分钟刷新一次游戏消息

//////////////////////////////////////////////////////////////////////////////////
//                   游戏系统逻辑使用的定时器1000以后                           //
//////////////////////////////////////////////////////////////////////////////////
#define IDD_TIMER_GAME_START                 1000                  // 游戏定时器从1000开始

//////////////////////////////////////////////////////////////////////////////////
//                   游戏系统逻辑使用的GUI的ID从1000以后                        //
//////////////////////////////////////////////////////////////////////////////////
#define IDD_GUI_ID_START                     1000                  // 游戏定时器从1000开始

//////////////////////////////////////////////////////////////////////////////////
//                   游戏系统要发送的消息类型                                   //
//////////////////////////////////////////////////////////////////////////////////
#define IDD_MESSAGE_TYPE_SUPER_BIG_MSG       1                     // 超级管理员大喇叭消息
#define IDD_MESSAGE_TYPE_SUPER_SMAILL_MSG    2                     // 超级管理员小喇叭消息
#define IDD_MESSAGE_TYPE_GAMESERVER_ENTERTIP 3                     // 游戏服务器进入提示
#define IDD_MESSAGE_TYPE_CLOSE_SERVER        4                     // 关闭游戏服务器
#define IDD_MESSAGE_TYPE_OPEN_SERVER         5                     // 打开游戏服务器
#define IDD_MESSAGE_TYPE_GAME_ADVERTISEMENT  6                     // 游戏广告
#define IDD_MESSAGE_TYPE_GAMESERVER_SYSTEM   7                     // 系统消息
#define IDD_MESSAGE_TYPE_CHAT_COMMON         8                     // 公聊
#define IDD_MESSAGE_TYPE_CHAT_PRIVATE        9                     // 私聊
#define IDD_MESSAGE_TYPE_NULL                10                    // 什么也没有

//////////////////////////////////////////////////////////////////////////

/**
 * 游戏场景类型
 */
enum SceneType
{
	SCENE_TYPE_HALL = 0,               // 大厅
	SCENE_TYPE_GAMEROOM,               // 游戏房间
	SCENE_TYPE_NULL
};

//开始模式
enum enStartMode
{
	enStartMode_FullReady,				//满人开始(游戏中至少两人以上准备)
	enStartMode_AllReady,				//所有准备(游戏中满桌并且所有人都准备)
	enStartMode_Symmetry,				//对称开始
	enStartMode_TimeControl,			//时间控制
};

// 日志等级
enum enLogLevel
{
    BASIC = 0,
    DETAIL,
    ERROR,
    DEBUG
};

#endif
