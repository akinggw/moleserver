#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#include "cdefines.h"

//数值掩码
#define	LOGIC_MASK_COLOR			0xFF00								//花色掩码
#define	LOGIC_MASK_VALUE			0x00FF								//数值掩码

#define CT_HIGH_CARD				1									//高牌
#define CT_ONE_PAIR					2									//对子
#define	CT_TWO_PAIR					3									//两对
#define CT_THREE_KIND				4									//三条
#define CT_STRAIGHT					5									//顺子
#define CT_FLUSH					6									//同花
#define CT_FULL_HOUSE				7									//葫芦
#define CT_FOUR_KIND				8									//四条
#define CT_STRAIGHT_FLUSH			9									//同花顺
#define CT_ROYAL_FLUSH				10									//皇家同花顺

//分析结构
struct tagAnalyseResult
{
	int 							nFourCount;								//四张数目
	int 							nThreeCount;							//三张数目
	int 							nDoubleCount;							//两张数目
	int								nSignedCount;							//单张数目

	WORD							wFourCardData[IDD_MAX_WEAVE_CARD];		//四张列表
	WORD							wThreeCardData[IDD_MAX_WEAVE_CARD];		//三张列表
	WORD							wDoubleCardData[IDD_MAX_WEAVE_CARD];	//两张列表
	WORD							wSignedCardData[IDD_MAX_WEAVE_CARD];	//单张列表
};






class CGameLogic
{
public:
	CGameLogic();
	~CGameLogic();


	//变量定义
private:
	static WORD						m_wCardListData[IDD_MAX_CARD_COUNT];				//扑克定义



public:
	//对比扑克
	int CompareCard(WORD wFirstWeave[IDD_WEAVE_COUNT], WORD wNextWeave[IDD_WEAVE_COUNT],WORD wFirstType,WORD wNextType);
	//获取组合类型
	WORD GetCardType(WORD wHandCartd[IDD_HAND_COUNT],WORD wBoardCard[], int nCardCount, WORD wBestWeaveData[IDD_WEAVE_COUNT]);


	//获取数值
	WORD GetCardValue(WORD wCardData) { return wCardData&LOGIC_MASK_VALUE; }
	//获取花色
	WORD GetCardColor(WORD wCardData) { return (wCardData&LOGIC_MASK_COLOR)>>8; }

	//混乱扑克
	void RandCardList(WORD wCardData[IDD_MAX_CARD_COUNT]);
	//获取逻辑值
	WORD GetCardLogicValue(WORD wCardData);

private:
	//分析扑克
	void AnalysebCardData(const WORD wCardData[], int nCardCount, tagAnalyseResult & AnalyseResult);

	//排列扑克
	void SortCard(WORD wCardData[], int nCardCount);
	//排列扑克
	void SortCardByLogicValue(WORD wCardData[], int nCardCount);


	//是否同花
	int GetTongHua(WORD wCardData[],int nCardCount,WORD wTongHuaData[]);
	//是否顺子
	bool IsShunZi(WORD wCardData[],int nCardCount,WORD wShunziData[IDD_WEAVE_COUNT]);
};

#endif
