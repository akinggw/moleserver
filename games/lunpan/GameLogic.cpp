#include "GameLogic.h"

int m_fenzhus[][2] = {
{0,3},
{3,6},
{6,9},
{9,12},
{12,15},
{15,18},
{18,21},
{21,24},
{24,27},
{27,30},
{30,33},
{33,36},

{0,2},
{2,5},
{5,8},
{8,11},
{11,14},
{14,17},
{17,20},
{20,23},
{23,26},
{26,29},
{29,32},
{32,35},

{0,1},
{1,4},
{4,7},
{7,10},
{10,13},
{13,16},
{16,19},
{19,22},
{22,25},
{25,28},
{28,31},
{31,34},

{3,2},
{6,5},
{9,8},
{12,11},
{15,14},
{18,17},
{21,20},
{24,23},
{27,26},
{30,29},
{33,32},
{36,35},

{2,1},
{5,4},
{8,7},
{11,10},
{14,13},
{17,16},
{20,19},
{23,22},
{26,25},
{28,29},
{32,31},
{34,35}};

int m_jiezhus[][3] = {
{1,2,3},
{4,5,6},
{7,8,9},
{10,11,12},
{13,14,15},
{16,17,18},
{19,20,21},
{22,23,24},
{25,26,27},
{28,29,30},
{31,32,33},
{34,35,36}};

int m_sanshu[][3] = {
{0,2,3},
{0,2,1}};

int m_jiaozhus[][4] = {
{2,3,5,6},
{5,6,8,9},
{8,9,11,12},
{11,12,14,15},
{14,15,17,18},
{17,18,20,21},
{20,21,23,24},
{23,24,26,27},
{26,27,29,30},
{29,30,32,33},
{32,33,35,36},

{1,2,4,5},
{4,5,7,8},
{7,8,10,11},
{10,11,13,14},
{13,14,16,17},
{16,17,19,20},
{19,20,22,23},
{22,23,25,26},
{25,26,28,29},
{28,29,31,32},
{31,32,34,35}};

int m_sigehaomas[][4] = {
{0,1,2,3}};

int m_xianzhus[][6] = {
{1,2,3,4,5,6},
{4,5,6,7,8,9},
{7,8,9,10,11,12},
{10,11,12,13,14,15},
{13,14,15,16,17,18},
{16,17,18,19,20,21},
{19,20,21,22,23,24},
{22,23,24,25,26,27},
{25,26,27,28,29,30},
{28,29,30,31,32,33},
{31,32,33,34,35,36}};

int m_liezhus[][12] = {
{3,6,9,12,15,18,21,24,27,30,33,36},
{2,5,8,11,14,17,20,23,26,29,32,35},
{1,4,7,10,13,16,19,22,25,28,31,34}};

int m_xiazhuyidashu[][12] = {
{1,2,3,4,5,6,7,8,9,10,11,12},
{13,14,15,16,17,18,19,20,21,22,23,24},
{25,26,27,28,29,30,31,32,33,34,35,36}};

int m_hongheis[][18] = {
{1,3,5,7,9,12,14,16,18,19,21,23,25,27,30,32,34,36},
{2,4,6,8,10,11,13,15,17,20,22,24,26,28,29,31,33,35}};

int m_danshuangs[][18] = {
{2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36},
{1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31,33,35}};

int m_daxiaos[][18] = {
{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18},
{19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36}};

std::string GetNameByTypeMe(YanZhuType type)
{
	std::string tmpStr;

	switch(type)
	{
	case TYPE_ZHIJIEZHU:
		tmpStr="孤丁";
		break;
	case TYPE_FENZHU:
		tmpStr="两字";
		break;
	case TYPE_JIEZHU:
		tmpStr="三字";
		break;
	case TYPE_SANSHU:
		tmpStr="三字";
		break;
	case TYPE_JIAOZHU:
		tmpStr="四字";
		break;
	case TYPE_SIGEHAOMA:
		tmpStr="四字";
		break;
	case TYPE_XIANZHU:
		tmpStr="六字";
		break;
	case TYPE_LIEZHU:
		tmpStr="十二字";
		break;
	case TYPE_XIAZHUYIDASHU:
		tmpStr="十二字";
		break;
	case TYPE_HONGHEI:
		tmpStr="红黑";
		break;
	case TYPE_DANSHUANG:
		tmpStr="单双";
		break;
	case TYPE_DAXIAO:
		tmpStr="十八字";
		break;
	default:
		break;
	}

	return tmpStr;
}

int GetXiaZhuArea(YanZhuType type,int index)
{
	switch(type)
	{
	case TYPE_HONGHEI:
		{
			return m_hongheis[index][rand()%18];
		}
		break;
	case TYPE_DANSHUANG:
		{
			return m_danshuangs[index][rand()%18];
		}
		break;
	case TYPE_DAXIAO:
		{
			return m_daxiaos[index][rand()%18];
		}
		break;
	case TYPE_LIEZHU:
		{
			return m_liezhus[index][rand()%12];
		}
		break;
	case TYPE_XIAZHUYIDASHU:
		{
			return m_xiazhuyidashu[index][rand()%12];
		}
		break;
	default:
		break;
	}

	return -1;
}

int GetXiaZhuAreaCount(YanZhuType type)
{
	int tmpCount = 1;

	switch (type)
	{
	case TYPE_ZHIJIEZHU:
		tmpCount = 37;
		break;
	case TYPE_FENZHU:
		tmpCount = 60;
		break;
	case TYPE_JIEZHU:
		tmpCount = 12;
		break;
	case TYPE_SANSHU:
		tmpCount = 2;
		break;
	case TYPE_JIAOZHU:
		tmpCount = 22;
		break;
	case TYPE_SIGEHAOMA:
		tmpCount = 1;
		break;
	case TYPE_XIANZHU:
		tmpCount = 11;
		break;
	case TYPE_LIEZHU:
		tmpCount = 3;
		break;
	case TYPE_XIAZHUYIDASHU:
		tmpCount = 3;
		break;
	case TYPE_HONGHEI:
		tmpCount = 2;
		break;
	case TYPE_DANSHUANG:
		tmpCount = 2;
		break;
	case TYPE_DAXIAO:
		tmpCount = 2;
		break;
	default:
		break;
	}

	return tmpCount;
}

int GetXiazhuType(YanZhuType type,int index)
{
	switch(type)
	{
	case TYPE_HONGHEI:
		{
			int i = 0;
			for(i=0;i<2;i++)
			{
				for(int j=0;j<18;j++)
				{
					if(m_hongheis[i][j] == index)
						return i;
				}
			}
		}
		break;
	case TYPE_DANSHUANG:
		{
			int i = 0;
			for(i=0;i<2;i++)
			{
				for(int j=0;j<18;j++)
				{
					if(m_danshuangs[i][j] == index)
						return i;
				}
			}
		}
		break;
	case TYPE_DAXIAO:
		{
			int i = 0;
			for(i=0;i<2;i++)
			{
				for(int j=0;j<18;j++)
				{
					if(m_daxiaos[i][j] == index)
						return i;
				}
			}
		}
		break;
	case TYPE_LIEZHU:
		{
			int i = 0;
			for(i=0;i<3;i++)
			{
				for(int j=0;j<12;j++)
				{
					if(m_liezhus[i][j] == index)
						return i;
				}
			}
		}
		break;
	case TYPE_XIAZHUYIDASHU:
		{
			int i = 0;
			for(i=0;i<3;i++)
			{
				for(int j=0;j<12;j++)
				{
					if(m_xiazhuyidashu[i][j] == index)
						return i;
				}
			}
		}
		break;
	default:
		break;
	}

	return -1;
}

/**
 * 根据下注类型，下注类型值返回下注值
 */
int getXiaZhuNumbers(YanZhuType type,int index,int *aarray)
{
	if(aarray == NULL) return 0;

	switch(type)
	{
	case TYPE_ZHIJIEZHU:
		{
			aarray[0] = index;
			return 1;
		}
		break;
	case TYPE_FENZHU:
		{
			aarray[0] = m_fenzhus[index][0];
			aarray[1] = m_fenzhus[index][1];
			return 2;
		}
		break;
	case TYPE_JIEZHU:
		{
			aarray[0] = m_jiezhus[index][0];
			aarray[1] = m_jiezhus[index][1];
			aarray[2] = m_jiezhus[index][2];
			return 3;
		}
		break;
	case TYPE_SANSHU:
		{
			aarray[0] = m_sanshu[index][0];
			aarray[1] = m_sanshu[index][1];
			aarray[2] = m_sanshu[index][2];
			return 3;
		}
		break;
	case TYPE_JIAOZHU:
		{
			aarray[0] = m_jiaozhus[index][0];
			aarray[1] = m_jiaozhus[index][1];
			aarray[2] = m_jiaozhus[index][2];
			aarray[3] = m_jiaozhus[index][3];
			return 4;
		}
		break;
	case TYPE_SIGEHAOMA:
		{
			aarray[0] = m_sigehaomas[index][0];
			aarray[1] = m_sigehaomas[index][1];
			aarray[2] = m_sigehaomas[index][2];
			aarray[3] = m_sigehaomas[index][3];
			return 4;
		}
		break;
	case TYPE_XIANZHU:
		{
			aarray[0] = m_xianzhus[index][0];
			aarray[1] = m_xianzhus[index][1];
			aarray[2] = m_xianzhus[index][2];
			aarray[3] = m_xianzhus[index][3];
			aarray[4] = m_xianzhus[index][4];
			aarray[5] = m_xianzhus[index][5];
			return 6;
		}
		break;
	case TYPE_LIEZHU:
		{
			for(int i=0;i<12;i++)
				aarray[i] = m_liezhus[index][i];
			return 12;
		}
		break;
	case TYPE_XIAZHUYIDASHU:
		{
			for(int i=0;i<12;i++)
				aarray[i] = m_xiazhuyidashu[index][i];
			return 12;
		}
		break;
	case TYPE_HONGHEI:
		{
			for(int i=0;i<18;i++)
				aarray[i] = m_hongheis[index][i];
			return 18;
		}
		break;
	case TYPE_DANSHUANG:
		{
			for(int i=0;i<18;i++)
				aarray[i] = m_danshuangs[index][i];
			return 18;
		}
		break;
	case TYPE_DAXIAO:
		{
			for(int i=0;i<18;i++)
				aarray[i] = m_daxiaos[index][i];
			return 18;
		}
		break;
	default:
		break;
	}

	return 0;
}

/*
 * 拆解筹码
 */
void chaijiechouma(int64 money,std::map<int,int> &pchouma)
{
	if(money <= 0) return;

	pchouma[50000] = (int)(money / 50000);
	int64 tmpNum = money % 50000;
	pchouma[10000] = (int)(tmpNum / 10000);
	tmpNum = money % 10000;
	pchouma[5000] = (int)(tmpNum / 5000);
	tmpNum = money % 5000;
	pchouma[1000] = (int)(tmpNum / 1000);
	tmpNum = money % 1000;
	pchouma[500] = (int)(tmpNum / 500);
	tmpNum = money % 500;
	pchouma[100] = (int)(tmpNum / 100);
}
