#include "GameLogic.h"

#include <vector>
#include <map>

#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameLogic::CGameLogic()
{
	m_cbCardListData[0] = CARDTYPE_XIAOBAILONG;
	m_cbCardListData[1] = CARDTYPE_XIAOLAOHU;
	m_cbCardListData[2] = CARDTYPE_XIAOFENGHUANG;

	m_cbCardListData[3] = CARDTYPE_QINGLONG;

	m_cbCardListData[4] = CARDTYPE_XIAOLAOHU;
	m_cbCardListData[5] = CARDTYPE_XIAOFENGHUANG;
	m_cbCardListData[6] = CARDTYPE_XIAOWUGUI;

	m_cbCardListData[7] = CARDTYPE_BAIHU;

	m_cbCardListData[8] = CARDTYPE_XIAOFENGHUANG;
	m_cbCardListData[9] = CARDTYPE_XIAOWUGUI;
	m_cbCardListData[10] = CARDTYPE_XIAOBAILONG;

	m_cbCardListData[11] = CARDTYPE_ZHUQUE;

	m_cbCardListData[12] = CARDTYPE_XIAOWUGUI;
	m_cbCardListData[13] = CARDTYPE_XIAOLAOHU;
	m_cbCardListData[14] = CARDTYPE_XIAOBAILONG;

	m_cbCardListData[15] = CARDTYPE_XUANWU;
}

//析构函数
CGameLogic::~CGameLogic()
{
}

/// 根据区域类型得到位置
int CGameLogic::GetPostionByCarType(tagCardType pCarType)
{
	int pTmpCarType[16];
	int pTmpCarTypeCount = 0;

	for(int i=0;i<16;i++)
	{
		if(m_cbCardListData[i] == pCarType)
		{
			pTmpCarType[pTmpCarTypeCount] = i;
			pTmpCarTypeCount+=1;
		}
	}

	return pTmpCarType[rand()%pTmpCarTypeCount];
}
