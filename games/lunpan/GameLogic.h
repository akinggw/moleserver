#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#include "cdefines.h"
#include <map>
#include <string>

/*
 * 押注类型
 */
enum YanZhuType
{
	TYPE_ZHIJIEZHU = 0,                      // 直接注
	TYPE_FENZHU,                             // 分注
	TYPE_JIEZHU,                             // 街注
	TYPE_SANSHU,                             // 三数
	TYPE_JIAOZHU,                            // 角注
	TYPE_SIGEHAOMA,                          // 4个号码
	TYPE_XIANZHU,                            // 线注
	TYPE_LIEZHU,                             // 列注
	TYPE_XIAZHUYIDASHU,                      // 下注一打数
	TYPE_HONGHEI,                            // 红黑
	TYPE_DANSHUANG,                          // 单双
	TYPE_DAXIAO,                             // 大小
	TYPE_NULL
};

/// 根据类型得到名字
std::string GetNameByTypeMe(YanZhuType type);

/**
 * 根据下注类型，下注类型值返回下注值
 */
int getXiaZhuNumbers(YanZhuType type,int index,int *aarray);

/*
 * 拆解筹码
 */
void chaijiechouma(int64 money,std::map<int,int> &pchouma);

/**
 * 检测指定值是否在那个区域
 */
int GetXiazhuType(YanZhuType type,int index);
int GetXiaZhuAreaCount(YanZhuType type);
int GetXiaZhuArea(YanZhuType type,int index);

/*
 * 管理所有的下注
 */
class CXiaZhuManager
{
public:
	/// 构造函数
	CXiaZhuManager() { }
	/// 析构函数
	~CXiaZhuManager() { }

	/// 设置指定区域的下注额
	inline void setXiaZhu(YanZhuType pYanZhuType,int index,int64 money)
	{
		m_xiazhus[(int)pYanZhuType][index] = money;
	}
	/// 累加指定区域的下注额
	inline void addXiaZhu(YanZhuType pYanZhuType,int index,int64 money)
	{
		m_xiazhus[(int)pYanZhuType][index] += money;
	}
	/// 减指定区域的下注额
	inline void decXiaZhu(YanZhuType pYanZhuType,int index,int64 money)
	{
		if(m_xiazhus[(int)pYanZhuType][index] - money >= 0)
			m_xiazhus[(int)pYanZhuType][index] -= money;
	}
	/// 设置指定区域下注额为0
	inline void zeroXiaZhu(YanZhuType pYanZhuType,int index)
	{
		m_xiazhus[(int)pYanZhuType][index] = 0;
	}
	/// 得到指定区域的下注额
	inline int64 getXiaZhu(YanZhuType pYanZhuType,int index)
	{
		return m_xiazhus[(int)pYanZhuType][index];
	}
	/// 得到指定大区域的下注额
	inline int64 getXiaZhuBigarea(YanZhuType pYanZhuType)
	{
		int64 ptempMoney = 0;

		std::map<int,int64>::iterator iter = m_xiazhus[(int)pYanZhuType].begin();
		for(;iter != m_xiazhus[(int)pYanZhuType].end();++iter)
			ptempMoney+=(*iter).second;

		return ptempMoney;
	}
	/// 得到所有的下注
	inline std::map<int,std::map<int,int64> >& getXiaZhus(void)
	{
		return m_xiazhus;
	}
	/// 得到下注个数
	inline int getXiaZhuCount(void)
	{
		int pTotal = 0;

		std::map<int,std::map<int,int64> >::iterator iter = m_xiazhus.begin();
		for(;iter != m_xiazhus.end();++iter)
		{
			pTotal+=(int)(*iter).second.size();
		}

		return pTotal;
	}
	/// 得到指定类型的下注
	inline std::map<int,int64>& getXiaZhuByType(YanZhuType ptype)
	{
		return m_xiazhus[(int)ptype];
	}
	/// 清除
	inline void clear(void)
	{
		m_xiazhus.clear();
		m_xiazhubeilv.clear();
	}
	/// 计算当前玩家总的下注
	inline int64 getTotalXiaZhu(void)
	{
		int64 pTotal = 0;

		std::map<int,std::map<int,int64> >::iterator iter = m_xiazhus.begin();
		for(;iter != m_xiazhus.end();++iter)
		{
			std::map<int,int64>::iterator iter2 = (*iter).second.begin();
			for(;iter2 != (*iter).second.end();++iter2)
				pTotal+=(*iter2).second;
		}

		return pTotal;
	}

	//设置下注倍率
	inline void setXiaZhuBeilv(YanZhuType pYanZhuType,int beilv)
	{
		m_xiazhubeilv[(int)pYanZhuType] = beilv;
	}
	/// 得到指定区域的倍率
	inline int getXiaZhuBeilv(YanZhuType pYanZhuType)
	{
		return m_xiazhubeilv[(int)pYanZhuType];
	}

private:
	std::map<int,std::map<int,int64> > m_xiazhus;
	std::map<int,int > m_xiazhubeilv;
};

//////////////////////////////////////////////////////////////////////////

#endif
