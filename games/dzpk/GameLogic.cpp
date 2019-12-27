#include "GameLogic.h"

#include <vector>
#include <map>

//扑克数据
WORD CGameLogic::m_wCardListData[IDD_MAX_CARD_COUNT]=
{
	0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008,0x0009,0x000A,0x000B,0x000C,0x000D,									//方块 A - K
	0x0101,0x0102,0x0103,0x0104,0x0105,0x0106,0x0107,0x0108,0x0109,0x010A,0x010B,0x010C,0x010D,									//梅花 A - K
	0x0201,0x0202,0x0203,0x0204,0x0205,0x0206,0x0207,0x0208,0x0209,0x020A,0x020B,0x020C,0x020D,									//红桃 A - K
	0x0301,0x0302,0x0303,0x0304,0x0305,0x0306,0x0307,0x0308,0x0309,0x030A,0x030B,0x030C,0x030D,									//黑桃 A - K
};



CGameLogic::CGameLogic()
{

}

CGameLogic::~CGameLogic()
{

}


//对比扑克
int CGameLogic::CompareCard(WORD wFirstWeave[IDD_WEAVE_COUNT], WORD wNextWeave[IDD_WEAVE_COUNT],WORD wFirstType,WORD wNextType)
{
	//类型不同，快速判断.
	if (wFirstType!=wNextType)
	{
		return wFirstType>wNextType?1:-1;
	}

	WORD wFirstLogicValue=GetCardLogicValue(wFirstWeave[0]);
	WORD wNextLogicValue=GetCardLogicValue(wNextWeave[0]);

	switch(wFirstType)
	{
	case CT_ROYAL_FLUSH:
		return 0;
		break;
	case CT_STRAIGHT_FLUSH:
		{
			if (wFirstLogicValue==wNextLogicValue)
			{
				return 0;
			}
			else
				return wFirstLogicValue>wNextLogicValue?1:-1;
		}
		break;
	case CT_FOUR_KIND:
		{
			return wFirstLogicValue>wNextLogicValue?1:-1;
		}
		break;
	case CT_FULL_HOUSE:
		{
			if (wFirstLogicValue!=wNextLogicValue)
			{
				return wFirstLogicValue>wNextLogicValue?1:-1;
			}
			else if (GetCardLogicValue(wFirstWeave[3])!=GetCardLogicValue(wNextWeave[3]))
			{
				return GetCardLogicValue(wFirstWeave[3])>GetCardLogicValue(wNextWeave[3])?1:-1;
			}
			return 0;
		}
		break;
	case CT_FLUSH:
		{
			for (int i=0; i<IDD_WEAVE_COUNT; i++)
			{
				if (GetCardLogicValue(wFirstWeave[i])!=GetCardLogicValue(wNextWeave[i]))
				{
					return GetCardLogicValue(wFirstWeave[i])>GetCardLogicValue(wNextWeave[i])?1:-1;
				}
			}
			return 0;
		}
		break;
	case CT_STRAIGHT:
		{
			if (wFirstLogicValue==wNextLogicValue)
			{
				return 0;
			}
			else
				return wFirstLogicValue>wNextLogicValue?1:-1;
		}
		break;
	case CT_THREE_KIND:
		{
			for (int i=3; i<IDD_WEAVE_COUNT; i++)
			{
				if (GetCardLogicValue(wFirstWeave[i])!=GetCardLogicValue(wNextWeave[i]))
				{
					return GetCardLogicValue(wFirstWeave[i])>GetCardLogicValue(wNextWeave[i])?1:-1;
				}
			}
		}
		break;
	case CT_TWO_PAIR:
		{
			for (int i=1; i<IDD_WEAVE_COUNT; i++)
			{
				if (GetCardLogicValue(wFirstWeave[i])!=GetCardLogicValue(wNextWeave[i]))
				{
					return GetCardLogicValue(wFirstWeave[i])>GetCardLogicValue(wNextWeave[i])?1:-1;
				}
			}
			return 0;
		}
		break;
	case CT_ONE_PAIR:
		{
			for (int i=1; i<IDD_WEAVE_COUNT; i++)
			{
				if (GetCardLogicValue(wFirstWeave[i])!=GetCardLogicValue(wNextWeave[i]))
				{
					return GetCardLogicValue(wFirstWeave[i])>GetCardLogicValue(wNextWeave[i])?1:-1;
				}
			}
			return 0;
		}
	case CT_HIGH_CARD:
		{
			for (int i=0; i<IDD_WEAVE_COUNT; i++)
			{
				if (GetCardLogicValue(wFirstWeave[i])!=GetCardLogicValue(wNextWeave[i]))
				{
					return GetCardLogicValue(wFirstWeave[i])>GetCardLogicValue(wNextWeave[i])?1:-1;
				}
			}
			return 0;
		}
		break;
	}
	return 0;
}


//获取组合类型
WORD CGameLogic::GetCardType(WORD wHandCartd[IDD_HAND_COUNT],WORD wBoardCard[], int nCardCount, WORD wBestWeaveData[IDD_WEAVE_COUNT])
{
	if (nCardCount==0)
	{
		memset(wBestWeaveData,0,sizeof(WORD)*IDD_WEAVE_COUNT);
		memcpy(wBestWeaveData,wHandCartd,sizeof(WORD)*IDD_HAND_COUNT);
		SortCardByLogicValue(wBestWeaveData,IDD_HAND_COUNT);

		if (GetCardValue(wHandCartd[0])==GetCardValue(wHandCartd[1]))
		{
			return CT_ONE_PAIR;
		}
		else
			return CT_HIGH_CARD;
	}



	WORD wCardData[IDD_MAX_WEAVE_CARD];
	memcpy(wCardData,wBoardCard,sizeof(WORD)*nCardCount);
	wCardData[nCardCount++]=wHandCartd[0];
	wCardData[nCardCount++]=wHandCartd[1];

	SortCardByLogicValue(wCardData,nCardCount);

	WORD TempSameColoer[IDD_MAX_WEAVE_CARD];
	memset(TempSameColoer,0,sizeof(TempSameColoer));
	int nSmaeColorCount=GetTongHua(wCardData,nCardCount,TempSameColoer);
	//同花顺
	if (nSmaeColorCount>=5)
	{
		bool Shunzi=IsShunZi(TempSameColoer,nSmaeColorCount,wBestWeaveData);
		if (Shunzi)
		{
			if (GetCardValue(wBestWeaveData[1])==13)
			{
				return CT_ROYAL_FLUSH;
			}
			return CT_STRAIGHT_FLUSH;
		}
	}
	//扑克分析
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(wCardData,nCardCount,AnalyseResult);

	//四条类型
	if (AnalyseResult.nFourCount==1)
	{
		memcpy(wBestWeaveData,&AnalyseResult.wFourCardData,sizeof(WORD)*4);
		int nBestWeaveCount=4;
		for (int i=0; i<nCardCount; i++)
		{
			if (GetCardValue(wCardData[i])!=GetCardValue(wBestWeaveData[0]))
			{
				wBestWeaveData[nBestWeaveCount++]=wCardData[i];
				if (nBestWeaveCount==5)break;
			}
		}
		return CT_FOUR_KIND;
	}

	//葫芦类型
	if (AnalyseResult.nThreeCount==1 && AnalyseResult.nDoubleCount>=1)
	{
		memcpy(wBestWeaveData,&AnalyseResult.wThreeCardData,sizeof(WORD)*3);
		memcpy(&wBestWeaveData[3],&AnalyseResult.wDoubleCardData,sizeof(WORD)*2);
		return CT_FULL_HOUSE;
	}
	if (AnalyseResult.nThreeCount==2)
	{
		memcpy(wBestWeaveData,&AnalyseResult.wThreeCardData,sizeof(WORD)*IDD_WEAVE_COUNT);
		return CT_FULL_HOUSE;
	}

	//同花
	if (nSmaeColorCount>=5)
	{
		memcpy(wBestWeaveData,TempSameColoer,sizeof(WORD)*IDD_WEAVE_COUNT);
		return CT_FLUSH;
	}

	//顺子
	bool Shunzi=IsShunZi(wCardData,nCardCount,wBestWeaveData);
	if (Shunzi)
	{
		return CT_STRAIGHT;
	}

	//三条
	if (AnalyseResult.nThreeCount>0)
	{
		memcpy(wBestWeaveData,&AnalyseResult.wThreeCardData,sizeof(WORD)*3);
		int nBestWeaveCount=3;
		for (int i=0; i<nCardCount; i++)
		{
			if (GetCardValue(wCardData[i])!=GetCardValue(wBestWeaveData[0]))
			{
				wBestWeaveData[nBestWeaveCount++]=wCardData[i];
				if (nBestWeaveCount==5)break;
			}
		}
		return CT_THREE_KIND;
	}

	//两对类型
	if (AnalyseResult.nDoubleCount>=2)
	{
		memcpy(wBestWeaveData,&AnalyseResult.wDoubleCardData,sizeof(WORD)*4);
		int nBestWeaveCount=4;
		for (int i=0; i<nCardCount; i++)
		{
			if (GetCardValue(wCardData[i])!=GetCardValue(wBestWeaveData[0]) && GetCardValue(wCardData[i])!=GetCardValue(wBestWeaveData[2]))
			{
				wBestWeaveData[nBestWeaveCount++]=wCardData[i];
				if (nBestWeaveCount==5)break;
			}
		}
		return CT_TWO_PAIR;
	}

	//一对类型
	if (AnalyseResult.nDoubleCount==1)
	{
		memcpy(wBestWeaveData,&AnalyseResult.wDoubleCardData,sizeof(WORD)*2);
		int nBestWeaveCount=2;
		for (int i=0; i<nCardCount; i++)
		{
			if (GetCardValue(wCardData[i])!=GetCardValue(wBestWeaveData[0]))
			{
				wBestWeaveData[nBestWeaveCount++]=wCardData[i];
				if (nBestWeaveCount==5)break;
			}
		}
		return CT_ONE_PAIR;
	}
	memcpy(wBestWeaveData,wCardData,sizeof(WORD)*IDD_WEAVE_COUNT);
	return CT_HIGH_CARD;
}
//获取逻辑值
WORD CGameLogic::GetCardLogicValue(WORD wCardData)
{
	WORD wCardValue=GetCardValue(wCardData);
	if (wCardValue==1)
	{
		wCardValue+=13;
	}
	return wCardValue;

}


//分析扑克
void CGameLogic::AnalysebCardData(const WORD wCardData[], int nCardCount, tagAnalyseResult & AnalyseResult)
{
	//设置结果
	memcpy(&AnalyseResult,0,sizeof(AnalyseResult));


	//变量定义
	int nSameCount;
	WORD wSameCardData[4];
	WORD wLogicValue;


	//扑克分析
	for (BYTE i=0;i<nCardCount;i++)
	{

		//变量定义
		nSameCount=1;
		memset(wSameCardData,0,sizeof(wSameCardData));
		wSameCardData[0]=wCardData[i];
		wLogicValue=GetCardLogicValue(wCardData[i]);


		//获取同牌
		for (int j=i+1;j<nCardCount;j++)
		{
			//逻辑对比
			if (GetCardLogicValue(wCardData[j])!=wLogicValue) break;

			//设置扑克
			wSameCardData[nSameCount++]=wCardData[j];
		}

		//保存结果
		switch (nSameCount)
		{
		case 1:		//单张
			{
				memcpy(&AnalyseResult.wSignedCardData[(AnalyseResult.nSignedCount++)*nSameCount],wSameCardData,nSameCount*sizeof(WORD));
				break;
			}
		case 2:		//两张
			{
				memcpy(&AnalyseResult.wDoubleCardData[(AnalyseResult.nDoubleCount++)*nSameCount],wSameCardData,nSameCount*sizeof(WORD));
				break;
			}
		case 3:		//三张
			{
				memcpy(&AnalyseResult.wThreeCardData[(AnalyseResult.nThreeCount++)*nSameCount],wSameCardData,nSameCount*sizeof(WORD));
				break;
			}
		case 4:		//四张
			{
				memcpy(&AnalyseResult.wFourCardData[(AnalyseResult.nFourCount++)*nSameCount],wSameCardData,nSameCount*sizeof(WORD));
				break;
			}
		}

		//设置递增
		i+=nSameCount-1;
	}

	return;
}
//排列扑克
void CGameLogic::SortCard(WORD wCardData[], int nCardCount)
{
	//转换数值
	WORD wLogicValue[IDD_MAX_WEAVE_CARD];

	for (WORD i=0;i<nCardCount;i++) wLogicValue[i]=GetCardValue(wCardData[i]);

	//排序操作
	bool bSorted=true;
	WORD wTempData,wLast=nCardCount-1;
	do
	{
		bSorted=true;
		for (int i=0;i<wLast;i++)
		{
			if ((wLogicValue[i]<wLogicValue[i+1])||
				((wLogicValue[i]==wLogicValue[i+1])&&(wCardData[i]<wCardData[i+1])))
			{
				//交换位置
				wTempData=wCardData[i];
				wCardData[i]=wCardData[i+1];
				wCardData[i+1]=wTempData;
				wTempData=wLogicValue[i];
				wLogicValue[i]=wLogicValue[i+1];
				wLogicValue[i+1]=wTempData;
				bSorted=false;
			}
		}
		wLast--;
	} while(bSorted==false);

	return;
}
//排列扑克
void CGameLogic::SortCardByLogicValue(WORD wCardData[], int nCardCount)
{
	//转换数值
	WORD wLogicValue[IDD_MAX_WEAVE_CARD];

	for (WORD i=0;i<nCardCount;i++) wLogicValue[i]=GetCardLogicValue(wCardData[i]);

	//排序操作
	bool bSorted=true;
	WORD wTempData,wLast=nCardCount-1;
	do
	{
		bSorted=true;
		for (int i=0;i<wLast;i++)
		{
			if ((wLogicValue[i]<wLogicValue[i+1])||
				((wLogicValue[i]==wLogicValue[i+1])&&(wCardData[i]<wCardData[i+1])))
			{
				//交换位置
				wTempData=wCardData[i];
				wCardData[i]=wCardData[i+1];
				wCardData[i+1]=wTempData;
				wTempData=wLogicValue[i];
				wLogicValue[i]=wLogicValue[i+1];
				wLogicValue[i+1]=wTempData;
				bSorted=false;
			}
		}
		wLast--;
	} while(bSorted==false);

	return;
}
//混乱扑克
void CGameLogic::RandCardList(WORD wCardData[IDD_MAX_CARD_COUNT])
{
	//混乱准备
	WORD wTempCardData[CountArray(m_wCardListData)];
	memcpy(wTempCardData,m_wCardListData,sizeof(m_wCardListData));

	//混乱扑克
	int nRandCount=0,nPosition=0;
	do
	{
		nPosition=rand()%(CountArray(wTempCardData)-nRandCount);
		wCardData[nRandCount]=wTempCardData[nPosition];
		nRandCount++;
		wTempCardData[nPosition]=wTempCardData[CountArray(wTempCardData)-nRandCount];
	} while (nRandCount<IDD_MAX_CARD_COUNT);

	return;

}

//获取同花
int CGameLogic::GetTongHua(WORD wCardData[],int nCardCount,WORD wTongHuaData[])
{
	int HuaCount[4];
	WORD HuaData[4][IDD_MAX_WEAVE_CARD];
	memset(HuaCount,0,sizeof(HuaCount));
	for (int i=0; i<nCardCount; i++)
	{
		int tempColor=GetCardColor(wCardData[i]);

		HuaData[tempColor][HuaCount[tempColor]++]=wCardData[i];
	}

	for (int i=0; i<4; i++)
	{
		if (HuaCount[i]>=5)
		{
			memcpy(wTongHuaData,HuaData[i],sizeof(WORD)*HuaCount[i]);
			return HuaCount[i];
		}
	}

	return 0;
}
//是否顺子
bool CGameLogic::IsShunZi(WORD wCardData[],int nCardCount,WORD wShunziData[IDD_WEAVE_COUNT])
{
	int shunziCount=0;
	for (int i=0; i<nCardCount-4; i++)
	{
		wShunziData[shunziCount++]=wCardData[i];

		int j = 0;
		for (j=i; j<nCardCount; j++)
		{
			while(GetCardValue(wCardData[j])==GetCardValue(wCardData[j+1]))
			{
				j++;
				if (j>=nCardCount)
				{
					break;;
				}
			}


			if(GetCardLogicValue(wCardData[j])!=GetCardLogicValue(wCardData[j+1])+1)break;

			wShunziData[shunziCount++]=wCardData[j+1];
			if (shunziCount==5)
			{
				return true;
			}
		}

		if (j==nCardCount && shunziCount==4 && GetCardValue(wShunziData[3])==2 && GetCardValue(wCardData[0])==1)
		{
			wShunziData[shunziCount++]=wCardData[0];
			return true;
		}

		shunziCount=0;
	}

	return false;
}
