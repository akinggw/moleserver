#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#include "cdefines.h"

//////////////////////////////////////////////////////////////////////////

//游戏逻辑
class CGameLogic
{
	//变量定义
private:
	tagCarType				m_cbCardListData[32];				//扑克定义

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	/// 根据区域类型得到位置
	int GetPostionByCarType(tagCarType pCarType);
	/// 根据位置得到类型
	inline tagCarType GetCarTypeByIndex(int index) { return m_cbCardListData[index]; }
	/// 得到押租类型的名称
	std::string GetCarTypeName(tagCarType pCarType);
};

//////////////////////////////////////////////////////////////////////////

#endif
