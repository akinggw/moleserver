#include "GameLogic.h"

#include <vector>
#include <map>

//扑克数据
const uint8 CGameLogic::s_cbCardListData[CARD_COUNT]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K
	0x4E,0x4F,
};


//-----------------------------------------------
CGameLogic::CGameLogic()
{
}

CGameLogic::~CGameLogic()
{
}

//混乱扑克
void CGameLogic::RandCardList(uint8 cbCardBuffer[], uint8 cbBufferCount)
{
	//混乱准备
	uint8 cbCardData[CountArray(s_cbCardListData)];
	memcpy(cbCardData,s_cbCardListData,sizeof(s_cbCardListData));

	//混乱扑克
	uint8 cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(CountArray(cbCardData)-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[CountArray(cbCardData)-cbRandCount];
	} while (cbRandCount<cbBufferCount);

	return;
}

int CGameLogic::RetType(int itype)
{
	itype = itype%10;
	switch(itype)
	{
	case 0:
		return CT_SPECIAL_NIUNIU;
	case 1:
		return CT_SPECIAL_NIU1;
		break;
	case 2:
		return CT_SPECIAL_NIU2;
		break;
	case 3:
		return CT_SPECIAL_NIU3;
		break;
	case 4:
		return CT_SPECIAL_NIU4;
		break;
	case 5:
		return CT_SPECIAL_NIU5;
		break;
	case 6:
		return CT_SPECIAL_NIU6;
		break;
	case 7:
		return CT_SPECIAL_NIU7;
		break;
	case 8:
		return CT_SPECIAL_NIU8;
		break;
	case 9:
		return CT_SPECIAL_NIU9;
		break;
	default :
		return CT_POINT;
		break;
	}

}
//获取牌型
uint8 CGameLogic::GetCardType(const uint8 cbCardData[], uint8 cbCardCount,uint8 *bcOutCadData )
{
	//合法判断
	assert(5==cbCardCount);
	if (5!=cbCardCount) return CT_ERROR;

	//排序扑克
	uint8 cbCardDataSort[CARD_COUNT];
	memcpy(cbCardDataSort,cbCardData,sizeof(uint8)*cbCardCount);
	SortCardList(cbCardDataSort,cbCardCount,ST_NEW);

	if(bcOutCadData != NULL)
	{
		memcpy(bcOutCadData,cbCardDataSort,cbCardCount);
	}

	int igetW= 0;


	if(GetCardNewValue(cbCardDataSort[0])==GetCardNewValue(cbCardDataSort[cbCardCount-2]))
	{
		if(bcOutCadData != NULL)
		{
			memcpy(bcOutCadData,cbCardDataSort,cbCardCount);
		}
		return CT_SPECIAL_BOMEBOME;
	}else
	{
		if(GetCardNewValue(cbCardDataSort[1])==GetCardNewValue(cbCardDataSort[cbCardCount-1]))
		{
			if(bcOutCadData != NULL)
			{
				memcpy(bcOutCadData,cbCardDataSort,cbCardCount);
			}
			return CT_SPECIAL_BOMEBOME;
		}

	}
	if(GetCardColor(cbCardDataSort[0])==0x04&&GetCardColor(cbCardDataSort[1])==0x04)
	{
		if(GetCardNewValue(cbCardDataSort[2])==GetCardNewValue(cbCardDataSort[3]))
		{
			if(bcOutCadData != NULL)
			{
				bcOutCadData[0] = cbCardDataSort[2];
				bcOutCadData[1] = cbCardDataSort[3];
				bcOutCadData[2] = cbCardDataSort[0];
				bcOutCadData[3] = cbCardDataSort[1];
				bcOutCadData[4] = cbCardDataSort[4];

			}
			return CT_SPECIAL_BOMEBOME;
		}else
		{
			if(GetCardNewValue(cbCardDataSort[3])==GetCardNewValue(cbCardDataSort[4]))
			{
				if(bcOutCadData != NULL)
				{
					bcOutCadData[0] = cbCardDataSort[0];
					bcOutCadData[1] = cbCardDataSort[1];
					bcOutCadData[2] = cbCardDataSort[3];
					bcOutCadData[3] = cbCardDataSort[4];
					bcOutCadData[4] = cbCardDataSort[2];

				}
				return CT_SPECIAL_BOMEBOME;
			}

		}

	}
	if(GetCardColor(cbCardDataSort[0])==0x04)
	{
		if(GetCardNewValue(cbCardDataSort[1])==GetCardNewValue(cbCardDataSort[3]))
		{
			if(bcOutCadData != NULL)
			{
				bcOutCadData[0] = cbCardDataSort[1];
				bcOutCadData[1] = cbCardDataSort[2];
				bcOutCadData[2] = cbCardDataSort[3];
				bcOutCadData[3] = cbCardDataSort[0];
				bcOutCadData[4] = cbCardDataSort[4];

			}
			return CT_SPECIAL_BOMEBOME;
		}else
		{
			if(GetCardNewValue(cbCardDataSort[2])==GetCardNewValue(cbCardDataSort[4]))
			{
				if(bcOutCadData != NULL)
				{
					bcOutCadData[0] = cbCardDataSort[2];
					bcOutCadData[1] = cbCardDataSort[3];
					bcOutCadData[2] = cbCardDataSort[4];
					bcOutCadData[3] = cbCardDataSort[0];
					bcOutCadData[4] = cbCardDataSort[1];

				}
				return CT_SPECIAL_BOMEBOME;
			}

		}

	}
	bool blBig = true;
	int iCount = 0;
	int iLogicValue = 0;
	int iValueTen = 0;
	for (int i = 0;i<cbCardCount;i++)
	{
		uint8 bcGetValue = GetCardLogicValue(cbCardDataSort[i]);
		if(bcGetValue!=10&&bcGetValue!=11)
		{

			blBig = false;
			break;

		}else
		{
			if(GetCardNewValue(cbCardDataSort[i])==10)
			{
				iValueTen++;
			}
		}
		iCount++;
	}
	if(blBig)
	{
		if(bcOutCadData != NULL)
		{
			memcpy(bcOutCadData,cbCardDataSort,cbCardCount);
		}
		if(iValueTen==0)
			return CT_SPECIAL_NIUKING;
		else
		{
			if(iValueTen==1)
			{
				return CT_SPECIAL_NIUYING;
			}
		}
	}
	else
	{
		/*if(iCount>=4)
		{
		if(GetCardColor(cbCardDataSort[0])==0x04&&GetCardColor(cbCardDataSort[1])==0x04)
		{
		return CT_SPECIAL_NIUNIUDW
		}
		if(bcOutCadData != NULL)
		{
		memcpy(bcOutCadData,cbCardDataSort,cbCardCount);
		}
		return RetType(GetCardLogicValue(cbCardDataSort[3])+GetCardLogicValue(cbCardDataSort[4]));
		}*/
	}
	int n = 0;

	uint8 bcMakeMax[5];
	memset(bcMakeMax,0,5);
	int iBigValue = 0;
	uint8 iSingleA[2];
	int iIndex = 0;
	bcMakeMax[0]= cbCardDataSort[n];

	int iGetTenCount = 0;

	for (int iten = 0;iten<cbCardCount;iten++)
	{
		if(GetCardLogicValue(cbCardDataSort[iten])==10||GetCardLogicValue(cbCardDataSort[iten])==11)
		{
			iGetTenCount++;

		}
	}
	if( iGetTenCount>=3)
	{
		if(GetCardColor(cbCardDataSort[0])==0x04&&GetCardColor(cbCardDataSort[1])==0x04)
		{
			if(bcOutCadData != NULL)
			{
				bcOutCadData[0] = cbCardDataSort[0];
				bcOutCadData[1] = cbCardDataSort[3];
				bcOutCadData[2] = cbCardDataSort[4];
				bcOutCadData[3] = cbCardDataSort[1];
				bcOutCadData[4] = cbCardDataSort[2];

			}
			return CT_SPECIAL_NIUNIUDW;

		}
		if(GetCardColor(cbCardDataSort[0])==0x04)
		{
			//大小王与最小的组合成牛
			if(bcOutCadData != NULL)
			{
				bcOutCadData[0] = cbCardDataSort[0];
				bcOutCadData[1] = cbCardDataSort[3];
				bcOutCadData[2] = cbCardDataSort[4];
				bcOutCadData[3] = cbCardDataSort[1];
				bcOutCadData[4] = cbCardDataSort[2];
			}
			if(cbCardDataSort[0]==0x42)
				return CT_SPECIAL_NIUNIUDW;
			else
				return CT_SPECIAL_NIUNIUXW;
		}else
		{
			return RetType(GetCardLogicValue(cbCardDataSort[3])+GetCardLogicValue(cbCardDataSort[4]));
		}

	}
	if(iGetTenCount==2||(iGetTenCount==1&&GetCardColor(cbCardDataSort[0])==0x04))
	{

		if(GetCardColor(cbCardDataSort[0])==0x04&&GetCardColor(cbCardDataSort[1])==0x04)
		{
			if(bcOutCadData != NULL)
			{
				bcOutCadData[0] = cbCardDataSort[0];
				bcOutCadData[1] = cbCardDataSort[3];
				bcOutCadData[2] = cbCardDataSort[4];
				bcOutCadData[3] = cbCardDataSort[1];
				bcOutCadData[4] = cbCardDataSort[2];
			}
			return CT_SPECIAL_NIUNIUDW;
		}else
		{
			//如果有一张王 其他任意三张组合为10则是牛牛
			if(GetCardColor(cbCardDataSort[0])==0x04)
			{

				for ( n=1;n<cbCardCount;n++)
				{
					for (int j = 1;j<cbCardCount;j++)
					{
						if(j != n)
						{
							for (int w = 1;w<cbCardCount;w++)
							{
								if(w != n&&w!=j)
								{
									//如果剩余的四张中任意三张能组合位10的整数倍

									if((GetCardLogicValue(cbCardDataSort[n])+GetCardLogicValue(cbCardDataSort[j])+GetCardLogicValue(cbCardDataSort[w]))%10==0)
									{

										int add = 0;
										for (int y = 1;y<cbCardCount;y++)
										{
											if(y != n&&y!=j&&y!=w)
											{
												iSingleA[add] =cbCardDataSort[y];
												add++;

											}

										}
										if(bcOutCadData != NULL)
										{
											bcOutCadData[0] = cbCardDataSort[n];
											bcOutCadData[1] = cbCardDataSort[j];
											bcOutCadData[2] = cbCardDataSort[w];
											bcOutCadData[3] = cbCardDataSort[0];
											bcOutCadData[4] = iSingleA[0];
										}
										if(cbCardDataSort[0]==0x42)
											return CT_SPECIAL_NIUNIUDW;
										else
											return CT_SPECIAL_NIUNIUXW;


									}
								}
							}
						}
					}
				}
				//如果有一张王 其他任意三张组合不为10则 取两张点数最大的组合
				uint8 bcTmp[4];
				int iBig = 0;
				int in = 0;
				for ( in = 1;in<cbCardCount;in++)
				{
					for (int j = 1;j<cbCardCount;j++)
					{
						if(in != j)
						{
							uint8 bclogic = (GetCardLogicValue(cbCardDataSort[in])+GetCardLogicValue(cbCardDataSort[j]))%10;
							if(bclogic>iBig)
							{
								iBig = bclogic;
								int add = 0;
								bcTmp[0]=cbCardDataSort[in];
								bcTmp[1]=cbCardDataSort[j];
								for (int y = 1;y<cbCardCount;y++)
								{
									if(y != in&&y!=j)
									{
										iSingleA[add] =cbCardDataSort[y];
										add++;
									}

								}
								bcTmp[2]=iSingleA[0];
								bcTmp[3]=iSingleA[1];

							}

						}
					}
				}

				if(bcOutCadData != NULL)
				{
					bcOutCadData[0] = cbCardDataSort[0];
					bcOutCadData[1] = bcTmp[2];
					bcOutCadData[2] = bcTmp[3];
					bcOutCadData[3] = bcTmp[0];
					bcOutCadData[4] = bcTmp[1];
				}
				if(iGetTenCount==1&&GetCardColor(cbCardDataSort[0])==0x04)
				{
					//下面还能组合 有两张为 10 也可以组合成牛牛

				}else
				{
					//如果没有则比较 完与最小组合最大点数和组合
					return RetType(GetCardLogicValue(bcTmp[0])+GetCardLogicValue(bcTmp[1]));
				}
			}else
			{
				if((GetCardLogicValue(cbCardDataSort[2])+GetCardLogicValue(cbCardDataSort[3])+GetCardLogicValue(cbCardDataSort[4]))%10==0)
				{
					if(bcOutCadData != NULL)
					{
						bcOutCadData[0] = cbCardDataSort[2];
						bcOutCadData[1] = cbCardDataSort[3];
						bcOutCadData[2] = cbCardDataSort[4];
						bcOutCadData[3] = cbCardDataSort[0];
						bcOutCadData[4] = cbCardDataSort[1];
					}
					return CT_SPECIAL_NIUNIU;
				}else
				{
					for ( n= 2;n<cbCardCount;n++)
					{
						for (int j = 2;j<cbCardCount;j++)
						{
							if(j != n)
							{
								if((GetCardLogicValue(cbCardDataSort[n])+GetCardLogicValue(cbCardDataSort[j]))%10==0)
								{
									int add = 0;
									for (int y = 2;y<cbCardCount;y++)
									{
										if(y != n&&y!=j)
										{
											iSingleA[add] =cbCardDataSort[y];
											add++;

										}
									}
									if(iBigValue<=iSingleA[0]%10)
									{
										iBigValue = GetCardLogicValue(iSingleA[0])%10;
										if(bcOutCadData != NULL)
										{
											bcOutCadData[0]= cbCardDataSort[0];
											bcOutCadData[1]= cbCardDataSort[n];
											bcOutCadData[2]= cbCardDataSort[j];
											bcOutCadData[3]= cbCardDataSort[1];
											bcOutCadData[4]= iSingleA[0];

										}

										if(iBigValue==0)
										{

											return CT_SPECIAL_NIUNIU;
										}
									}

								}
							}
						}
					}
					if(iBigValue != 0)
					{
						return RetType(iBigValue);
					}
				}
			}

		}

		iGetTenCount = 1;

	}
	//4个组合
	if(iGetTenCount==1)
	{
		if(GetCardColor(cbCardDataSort[0])==0x04)
		{
			for ( n= 1;n<cbCardCount;n++)
			{
				for (int j = 1;j<cbCardCount;j++)
				{
					if(j != n)
					{
						//任意两张组合成牛
						if((GetCardLogicValue(cbCardDataSort[n])+GetCardLogicValue(cbCardDataSort[j]))%10==0)
						{
							int add = 0;
							for (int y = 1;y<cbCardCount;y++)
							{
								if(y != n&&y!=j)
								{
									iSingleA[add] =cbCardDataSort[y];
									add++;

								}

							}

							if(bcOutCadData != NULL)
							{
								bcOutCadData[0] = cbCardDataSort[0];
								bcOutCadData[1] = iSingleA[0];
								bcOutCadData[2] = iSingleA[1];
								bcOutCadData[3] = cbCardDataSort[n];
								bcOutCadData[4] = cbCardDataSort[j];
							}
							if(cbCardDataSort[0]==0x42)
								return CT_SPECIAL_NIUNIUDW;
							else
								return CT_SPECIAL_NIUNIUXW;

						}
					}

				}
			}

			//取4张中组合最大的点数

			uint8 bcTmp[4];
			int iBig = 0;
			int in = 0;
			for ( in = 1;in<cbCardCount;in++)
			{
				for (int j = 1;j<cbCardCount;j++)
				{
					if(in != j)
					{
						uint8 bclogic = (GetCardLogicValue(cbCardDataSort[in])+GetCardLogicValue(cbCardDataSort[j]))%10;
						if(bclogic>iBig)
						{
							iBig = bclogic;
							int add = 0;
							bcTmp[0]=cbCardDataSort[in];
							bcTmp[1]=cbCardDataSort[j];
							for (int y = 1;y<cbCardCount;y++)
							{
								if(y != in&&y!=j)
								{
									iSingleA[add] =cbCardDataSort[y];
									add++;
								}

							}
							bcTmp[2]=iSingleA[0];
							bcTmp[3]=iSingleA[1];

						}

					}
				}
			}

			if(bcOutCadData != NULL)
			{
				bcOutCadData[0] = cbCardDataSort[0];
				bcOutCadData[1] = bcTmp[2];
				bcOutCadData[2] = bcTmp[3];
				bcOutCadData[3] = bcTmp[0];
				bcOutCadData[4] = bcTmp[1];
			}
			return RetType(GetCardLogicValue(bcTmp[0])+GetCardLogicValue(bcTmp[1]));

		}
		//取4张中任两张组合为10 然后求另外两张的组合看是否是组合中最大
		for ( n= 1;n<cbCardCount;n++)
		{
			for (int j = 1;j<cbCardCount;j++)
			{
				if(j != n)
				{
					if((GetCardLogicValue(cbCardDataSort[n])+GetCardLogicValue(cbCardDataSort[j]))%10==0)
					{
						int add = 0;
						for (int y = 1;y<cbCardCount;y++)
						{
							if(y != n&&y!=j)
							{
								iSingleA[add] =cbCardDataSort[y];
								add++;

							}

						}
						if(iBigValue<=(GetCardLogicValue(iSingleA[0])+GetCardLogicValue(iSingleA[1]))%10)
						{
							iBigValue = GetCardLogicValue(iSingleA[0])+GetCardLogicValue(iSingleA[1])%10;
							bcMakeMax[0]= cbCardDataSort[0];
							bcMakeMax[1]= cbCardDataSort[j];
							bcMakeMax[2]= cbCardDataSort[n];
							bcMakeMax[3]= iSingleA[0];
							bcMakeMax[4]= iSingleA[1];

							if(bcOutCadData != NULL)
							{
								memcpy(bcOutCadData,bcMakeMax,cbCardCount);
							}
							if(iBigValue==0)
							{

								return CT_SPECIAL_NIUNIU;
							}
						}

					}
				}
			}
		}
		if(iBigValue!= 0)
		{
			return RetType(iBigValue);
		}else
		{
			//如果组合不成功
			iGetTenCount = 0;
		}
	}
	if(iGetTenCount==0)
	{
		//5个组合
		for ( n= 0;n<cbCardCount;n++)
		{
			for (int j = 0;j<cbCardCount;j++)
			{
				if(j != n)
				{
					for (int w = 0;w<cbCardCount;w++)
					{
						if(w != n&&w!=j)
						{
							int valueAdd = GetCardLogicValue(cbCardDataSort[n]);
							valueAdd += GetCardLogicValue(cbCardDataSort[j]);
							valueAdd += GetCardLogicValue(cbCardDataSort[w]);

							if(valueAdd%10==0)
							{
								int add = 0;
								for (int y = 0;y<cbCardCount;y++)
								{
									if(y != n&&y!=j&&y!=w)
									{
										iSingleA[add] =cbCardDataSort[y];
										add++;

									}

								}
								if(iBigValue<=(GetCardLogicValue(iSingleA[0])+GetCardLogicValue(iSingleA[1]))%10)
								{
									iBigValue = GetCardLogicValue(iSingleA[0])+GetCardLogicValue(iSingleA[1])%10;
									bcMakeMax[0]= cbCardDataSort[n];
									bcMakeMax[1]= cbCardDataSort[j];
									bcMakeMax[2]= cbCardDataSort[w];
									bcMakeMax[3]= iSingleA[0];
									bcMakeMax[4]= iSingleA[1];

									if(bcOutCadData != NULL)
									{
										memcpy(bcOutCadData,bcMakeMax,cbCardCount);
									}
									if(iBigValue==0)
									{

										return CT_SPECIAL_NIUNIU;
									}
								}

							}

						}
					}
				}
			}
		}
		if(iBigValue!=0)
		{
			return RetType(iBigValue);
		}
		else
		{
			return CT_POINT;
		}

	}

	return CT_POINT;
}

//大小比较
/*
cbNextCardData>cbFirstCardData  返回1
cbNextCardData<cbFirstCardData  返回-1
cbNextCardData==cbFirstCardData 返回0
*/
//Multiple 比较出来的倍数
int CGameLogic::CompareCard(const uint8 cbFirstCardData[], uint8 cbFirstCardCount,const uint8 cbNextCardData[], uint8 cbNextCardCount,uint8 &Multiple)
{
	//合法判断
	assert(5==cbFirstCardCount && 5==cbNextCardCount);
	if (!(5==cbFirstCardCount && 5==cbNextCardCount)) return 0;

	//获取牌型
	uint8 cbFirstCardType=GetCardType(cbFirstCardData, cbFirstCardCount);
	uint8 cbNextCardType=GetCardType(cbNextCardData, cbNextCardCount);

	//牌型比较
	if (cbFirstCardType != cbNextCardType)
	{
		if (cbNextCardType > cbFirstCardType)
		{
			Multiple = GetMultiple((emCardType)cbNextCardType);
			return 1;
		}
		else
		{
			Multiple = GetMultiple((emCardType)cbFirstCardType);
			return -1;
		}
	}

	//特殊牌型判断
	if (CT_POINT!=cbFirstCardType && cbFirstCardType==cbNextCardType)
	{
		//排序扑克
		uint8 cbFirstCardDataTmp[CARD_COUNT], cbNextCardDataTmp[CARD_COUNT];

		memcpy(cbFirstCardDataTmp,cbFirstCardData,sizeof(uint8)*cbFirstCardCount);
		memcpy(cbNextCardDataTmp,cbNextCardData,sizeof(uint8)*cbNextCardCount);
		SortCardList(cbFirstCardDataTmp,cbFirstCardCount,ST_NEW);
		SortCardList(cbNextCardDataTmp,cbNextCardCount,ST_NEW);

		Multiple = GetMultiple((emCardType)cbFirstCardType);

		uint8 firstValue = GetCardNewValue(cbFirstCardDataTmp[0]);
		uint8 NextValue = GetCardNewValue(cbNextCardDataTmp[0]);

		uint8 firstColor = GetCardColor(cbFirstCardDataTmp[0]);

		uint8 NextColor = GetCardColor(cbNextCardDataTmp[0]);


		if(firstValue<NextValue)
		{
			if (NextColor==0x04&&firstColor!=0x04)
			{
				return -1;
			}
			return 1;
		}
		else if(firstValue==NextValue)
		{
			if(firstColor<NextColor)
			{
					return 1;

			}else
			{
					return -1;
			}
		}
		else if (firstValue>NextValue)
		{
			if (firstColor==0x04&&NextColor!=0x04)
			{
				return 1;
			}
			return -1;
		}
	}

	//排序扑克
	uint8 cbFirstCardDataTmp[CARD_COUNT], cbNextCardDataTmp[CARD_COUNT];
	memcpy(cbFirstCardDataTmp,cbFirstCardData,sizeof(uint8)*cbFirstCardCount);
	memcpy(cbNextCardDataTmp,cbNextCardData,sizeof(uint8)*cbNextCardCount);
	SortCardList(cbFirstCardDataTmp,cbFirstCardCount,ST_NEW);
	SortCardList(cbNextCardDataTmp,cbNextCardCount,ST_NEW);

	uint8 firstValue = GetCardNewValue(cbFirstCardDataTmp[0]);
	uint8 NextValue = GetCardNewValue(cbNextCardDataTmp[0]);
	uint8 firstColor = GetCardColor(cbFirstCardDataTmp[0]);
	uint8 NextColor = GetCardColor(cbNextCardDataTmp[0]);

	if(firstValue<NextValue)
	{
		if (NextColor==0x04&&firstColor!=0x04)
		{
			return -1;
		}
		return 1;
	}else if(firstValue==NextValue)
	{
		if(firstColor<NextColor)
		{
			return 1;
		}else
		{
			return -1;
		}
	}
	else if (firstValue>NextValue)
	{
		if (firstColor==0x04&&NextColor!=0x04)
		{
			return 1;
		}
		return -1;

	}
	return -1;
}

//获取牌点
uint8 CGameLogic::GetCardListPip(const uint8 cbCardData[], uint8 cbCardCount)
{
	//变量定义
	uint8 cbPipCount=0;

	//获取牌点
	uint8 cbCardValue=0;
	for (uint8 i=0;i<cbCardCount;i++)
	{
		cbCardValue=GetCardValue(cbCardData[i]);
		if(cbCardValue>10)
		{
			cbCardValue = 10;

		}
		cbPipCount+=cbCardValue;
	}
	return (cbPipCount%10);
}
uint8 CGameLogic::GetCardNewValue(uint8 cbCardData)
{
	//扑克属性
	uint8 cbCardColor=GetCardColor(cbCardData);
	uint8 cbCardValue=GetCardValue(cbCardData);

	//转换数值
	if (cbCardColor==0x04) return cbCardValue+13+2;
	return cbCardValue;

}
//逻辑大小
uint8 CGameLogic::GetCardLogicValue(uint8 cbCardData)
{
	uint8 cbValue=GetCardValue(cbCardData);

	//获取花色
	uint8 cbColor=GetCardColor(cbCardData);

	if(cbValue>10)
	{
		cbValue = 10;

	}
	if(cbColor==0x4)
	{
		return 11;
	}
	return cbValue;
}

//排列扑克
void CGameLogic::SortCardList(uint8 cbCardData[], uint8 cbCardCount, uint8 cbSortType)
{
	//数目过虑
	if (cbCardCount==0) return;

	//转换数值
	uint8 cbSortValue[CARD_COUNT];
	if (ST_VALUE==cbSortType)
	{
		for (uint8 i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardValue(cbCardData[i]);
	}
	else
	{
		if(cbSortType==ST_NEW)
		{
			for (uint8 i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardNewValue(cbCardData[i]);

		}else
		{
			for (uint8 i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardLogicValue(cbCardData[i]);

		}

	}


	//排序操作
	bool bSorted=true;
	uint8 cbThreeCount,cbLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (uint8 i=0;i<cbLast;i++)
		{
			if ((cbSortValue[i]<cbSortValue[i+1])||
				((cbSortValue[i]==cbSortValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
			{
				//交换位置
				cbThreeCount=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbThreeCount;
				cbThreeCount=cbSortValue[i];
				cbSortValue[i]=cbSortValue[i+1];
				cbSortValue[i+1]=cbThreeCount;
				bSorted=false;
			}
		}
		cbLast--;
	} while(bSorted==false);

	return;
}


void CGameLogic::SortCardList_Big( uint8 cbCardDataArray[5][5] )
{
	std::vector<tagCardData>  CardArray_;
	tagCardData CardItem;
	for(uint8 i=0; i<5; i++)
	{
		CardItem.Init();
		CardItem.cbCardCnt = 5;
		memcpy(CardItem.cbCard_,cbCardDataArray[i] ,sizeof(CardItem.cbCard_));
		CardArray_.push_back(CardItem);
	}
	//排序操作[从小到大]

	bool bSorted=true;
	uint8 cbMultiple = 1;
	tagCardData tempCardItem;
	uint8 cbLast=CardArray_.size()-1;
	do
	{
		bSorted=true;
		for (uint8 i=0;i<cbLast;i++)
		{
			int nCompare = CompareCard(CardArray_[i].cbCard_, 5,
												CardArray_[i+1].cbCard_, 5,
												cbMultiple);
			//大的放前
			if (nCompare == 1)
			{
				//交换位置
				tempCardItem=CardArray_[i];
				CardArray_[i]=CardArray_[i+1];
				CardArray_[i+1]=tempCardItem;
				bSorted=false;
			}
		}
		cbLast--;
	} while(bSorted==false);

	for(uint8 i=0; i<CardArray_.size(); i++)
	{
		memcpy(cbCardDataArray[i],CardArray_[i].cbCard_,sizeof(CardArray_[i].cbCard_));
	}
}


//根据牌型得到倍数
uint8 CGameLogic::GetMultiple( emCardType CardType )
{
	uint8 cbMultiple = 1;
	//牛牛以上的10倍
	if(CardType>=CT_SPECIAL_NIUNIUXW)
	{
		cbMultiple = 6;
	}
	//牛牛4倍
	if (CardType>=CT_SPECIAL_NIU1 && CardType<=CT_SPECIAL_NIU6)
	{
		cbMultiple = 2;
	}
	if (CardType==CT_SPECIAL_NIU7)
	{
		cbMultiple = 3;
	}
	if (CardType==CT_SPECIAL_NIU8)
	{
		cbMultiple = 4;
	}
	if (CardType==CT_SPECIAL_NIU9)
	{
		cbMultiple = 5;
	}
	//无牛到牛6  1倍
	if (CardType==CT_POINT)
	{
		cbMultiple = 1;
	}
	return cbMultiple;
}
