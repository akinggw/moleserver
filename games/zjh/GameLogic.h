#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#include "cdefines.h"

#define CT_TE_SHU					0	//特殊
#define CT_SINGLE					1	//单张
#define CT_DOUBLE					2	//对子
#define CT_SHUN_ZI					3	//顺子
#define CT_JING_HUA					4	//三条
#define CT_TONG_HUA_SHUN			5	//同花顺
#define CT_BAO_ZI					6	//豹子



//数值掩码
#define	LOGIC_MASK_COLOR			0xFF00								//花色掩码
#define	LOGIC_MASK_VALUE			0x00FF								//数值掩码


class CzjhGameLogic
{
public:
	/// 构造函数
	CzjhGameLogic();
	/// 析构函数
	~CzjhGameLogic();

	//变量定义
private:
	static WORD						m_wCardListData[IDD_MAX_CARD_COUNT];				//扑克定义






	//控制函数
public:
	////排列扑克
	//void SortCard(WORD wCardData[IDD_MAX_HAND_COUNT]);
	//排列扑克
	void SortCardByLogicValue(WORD wCardData[IDD_MAX_HAND_COUNT]);
	//混乱扑克
	void RandCardList(WORD wCardBuffer[IDD_MAX_ROOM_USERCOUNT][IDD_MAX_HAND_COUNT]);

	//对比扑克
	bool CompareCard(WORD cbFirstCardData[IDD_MAX_HAND_COUNT], WORD cbNextCardData[IDD_MAX_HAND_COUNT]);



public:
	WORD GetCardValue(WORD wCardData){return wCardData&LOGIC_MASK_VALUE;}
	WORD GetCardColor(WORD wCardData){return wCardData&LOGIC_MASK_COLOR;}
	WORD GetCardType(WORD wCardData[IDD_MAX_HAND_COUNT]);
	WORD GetCardLogicValue(WORD wCardData);
};

#endif
