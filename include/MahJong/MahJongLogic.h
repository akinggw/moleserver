#ifndef MAHJONG_LOGIC_HEAD_FILE
#define MAHJONG_LOGIC_HEAD_FILE

#include "MahJongAlg.h"

//!  麻将通用逻辑
/*!
	麻将通用的判定逻辑和操作逻辑集合，有其他限制的需扩展该类。
	\n 如血战麻将的算法类：CXZMahJongLogic
	\n class CXZMahJongLogic : public CMahJongLogic
	\n {
	\n	//额外的变量...
	\n
	\n	//扩展的方法...
	\n }
	\n 在使用时，即可使用基类的通用方法和已扩展的方法。
	\n CXZMahJongLogic* xzLogic = new CXZMahJongLogic();
	\n xzLogic->SetMahJongAlg(new CXZMahJongAlg(MahJongColor_Wan|MahJongColor_Tiao|MahJongColor_Bing));//设置算法
	\n xzLogic->基类的方法();
	\n xzLogic->扩展的方法();
	\note 使用时需要设置具体的麻将算法
*/
class CMahJongLogic
{
private:
	CMahJongAlg				*m_PMJAlgorithm;							/*!< 逻辑所需的算法类,可以是其派生类 */
	bool					m_bRequiredCheck;							/*!< 调用操作逻辑时是否需要检测是否合法,当对隐藏牌操作时有效 */

public:
	//! 构造函数
	/*!
		初始化成员变量,设置默认值
		\sa ~CMahJongLogic()
	*/
	CMahJongLogic() {m_PMJAlgorithm = 0; m_bRequiredCheck = false;}

	//! 析构函数
	/*!
		释放资源
		\sa ~CMahJongLogic(), Release()
	*/
	~CMahJongLogic() { Release(); }

	//! 释放资源函数
	/*!
		释放资源
	*/
	void Release();

	//! 设置指定的算法
	/*!
		设置指定的算法,可以是其派生类
		\param PMJAlgorithm 麻将算法指针
		\sa GetMahJongAlg()
	*/
	void SetMahJongAlg(CMahJongAlg *PMJAlgorithm);

	//! 获取麻将算法
	/*!
		\return 返回上次设置的麻将算法指针
		\sa SetMahJongAlg()
	*/
	CMahJongAlg *GetMahJongAlg();

	//! 设置是否需要操作检测
	/*!
		设置后在调用操作函数时将对其操作进行合法性检查
		\note 对隐藏牌操作可能失效
		\sa GetRequiredCheck()
	*/
	inline void SetRequiredCheck(bool bRequiredCheck = true){ m_bRequiredCheck = bRequiredCheck; }

	//! 获取是否需要操作检测
	/*!
		\sa SetRequiredCheck()
	*/
	inline bool GetRequiredCheck(){ return m_bRequiredCheck; }

	//操作函数
public:
	//! 出牌操作
	/*!
		出牌:将牌从手牌数组中删除,加到出牌数组中
		\param bSelfHandMJ 手牌数组
		\param iSelfHandMJCount 手牌数组大小
		\param bSelfOutMJ 出牌数组
		\param iSelfOutMJCount 出牌数组大小
		\param bOutMJ 要出的牌
		\return 操作是否成功
		\note 如果是隐藏牌,手牌数量直接减1,将要出的牌加到出牌数组中,需要设置是否执行检查
		\sa UnDoOutMJ(),SetRequiredCheck(),GetRequiredCheck()
	*/
	virtual bool OutMJ(MJBYTE bSelfHandMJ[], int &iSelfHandMJCount, MJBYTE bSelfOutMJ[], int &iSelfOutMJCount, MJBYTE bOutMJ);


	//! 撤销出牌操作
	/*!
		出牌操作判定失败,撤销出牌操作:将牌从出牌数组中删除,加到手牌中.撤销后手牌应该是3N*2张
		\param bSelfHandMJ 手牌数组
		\param iSelfHandMJCount 手牌数组大小
		\param bSelfOutMJ 出牌数组
		\param iSelfOutMJCount 出牌数组大小
		\param bOutMJ 出的牌
		\return 操作是否成功
		\note 撤销操作仅限于对当前自己的牌,不能对隐藏牌操作
		\sa OutMJ(),SetRequiredCheck(),GetRequiredCheck()
	*/
	virtual bool UnDoOutMJ(MJBYTE bSelfHandMJ[], int &iSelfHandMJCount, MJBYTE bSelfOutMJ[], int &iSelfOutMJCount, MJBYTE bOutMJ);


	//! 吃牌操作
	/*!
		根据吃牌的类型,将删除相关手牌,添加吃牌到胡牌,删除出的最后一张牌
		\param bSelfHuMJ 胡牌数组
		\param iSelfHuCount 胡牌数组大小
		\param bSelfHandMJ 手牌数组
		\param iSelfHandCount 手牌数组大小
		\param bLastOutMJ 最后出牌玩家的出牌数组
		\param iLastOutMJCount 最后出牌玩家的出牌数组大小
		\param bChi 要吃的牌
		\param bChiType 吃牌的类型
		\return 操作是否成功
		\note 如果是隐藏牌,手牌数量直接减2,将结构加到胡牌数组中.需要设置是否执行检查
		\sa UnDoChi(), IsChi(), SetRequiredCheck(), GetRequiredCheck()
	*/
	virtual bool Chi(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bChi, enOperationChiType bChiType);


	//! 撤销吃牌操作
	/*!
		吃牌操作判定失败,撤销吃牌操作:根据吃牌的类型,添加相关手牌,删除胡牌中吃的牌,添加出的最后一张牌
		\param bSelfHuMJ 胡牌数组
		\param iSelfHuCount 胡牌数组大小
		\param bSelfHandMJ 手牌数组
		\param iSelfHandCount 手牌数组大小
		\param bLastOutMJ 最后出牌玩家的出牌数组
		\param iLastOutMJCount 最后出牌玩家的出牌数组大小
		\param bChi 吃的牌
		\param bChiType 吃牌的类型
		\return 操作是否成功
		\note 撤销操作仅限于对当前自己的牌,不能对隐藏牌操作
		\sa Chi(), IsChi(), SetRequiredCheck(), GetRequiredCheck()
	*/
	virtual bool UnDoChi(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bChi, enOperationChiType bChiType);


	//! 碰牌操作
	/*!
		碰牌:将上次出牌玩家出的最后一张牌删除,添加到操作玩家的胡牌中,同时删除操作玩家碰掉的牌
		\param bSelfHuMJ 胡牌数组
		\param iSelfHuCount 胡牌数组大小
		\param bSelfHandMJ 手牌数组
		\param iSelfHandCount 手牌数组大小
		\param bLastOutMJ 最后出牌玩家的出牌数组
		\param iLastOutMJCount 最后出牌玩家的出牌数组大小
		\param bPeng 要碰的牌
		\return 操作是否成功
		\note 如果是隐藏牌,手牌数量直接减2,将结构加到胡牌数组中.需要设置是否执行检查
		\sa UnDoPeng(), IsPeng(), SetRequiredCheck(), GetRequiredCheck()
	*/
	virtual bool Peng(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bPeng);


	//! 撤销碰牌操作
	/*!
		碰牌操作判定失败,撤销碰牌操作::补回上次出牌玩家出的最后一张牌,删除胡牌中碰的牌,补回手牌中碰掉的牌
		\param bSelfHuMJ 胡牌数组
		\param iSelfHuCount 胡牌数组大小
		\param bSelfHandMJ 手牌数组
		\param iSelfHandCount 手牌数组大小
		\param bLastOutMJ 最后出牌玩家的出牌数组
		\param iLastOutMJCount 最后出牌玩家的出牌数组大小
		\param bPeng 要碰的牌
		\return 操作是否成功
		\note 撤销操作仅限于对当前自己的牌,不能对隐藏牌操作
		\sa Peng(), IsPeng(), SetRequiredCheck(), GetRequiredCheck()
	*/
	virtual bool UnDoPeng(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bPeng);


	//! 杠操作
	/*!
		杠牌:分为明杠、自摸明杠、暗杠。
		明杠:将最后出牌玩家的最后一张牌和自己3张指定手牌删除,添加明杠
		自摸明杠:将1张指定手牌删除,把碰牌改为明杠
		暗杠:删除4张指定手牌,添加暗杠
		\param bSelfHuMJ 胡牌数组
		\param iSelfHuCount 胡牌数组大小
		\param bSelfHandMJ 手牌数组
		\param iSelfHandCount 手牌数组大小
		\param bLastOutMJ 最后出牌玩家的出牌数组
		\param iLastOutMJCount 最后出牌玩家的出牌数组大小
		\param bGang 要杠的牌
		\param bGangType 杠的类型
		\return 操作是否成功
		\note 需要设置是否执行检查
		\sa UnDoGang(), IsOutMingGang(), IsSelfMingGang(), IsAnGang(), SetRequiredCheck(), GetRequiredCheck()
	*/
	virtual bool Gang(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bGang, enOperationGangType bGangType);


	//! 撤销杠操作
	/*!
		杠牌:分为明杠、自摸明杠、暗杠。
		明杠:补回出牌玩家的最后一张牌和自己3张指定手牌,删除明杠
		自摸明杠:补回1张指定手牌,把明杠改为碰牌
		暗杠:补回4张指定手牌,删除暗杠
		\param bSelfHuMJ 胡牌数组
		\param iSelfHuCount 胡牌数组大小
		\param bSelfHandMJ 手牌数组
		\param iSelfHandCount 手牌数组大小
		\param bLastOutMJ 最后出牌玩家的出牌数组
		\param iLastOutMJCount 最后出牌玩家的出牌数组大小
		\param bGang 要杠的牌
		\param bGangType 杠的类型
		\return 操作是否成功
		\note 需要设置是否执行检查
		\sa Gang(), IsOutMingGang(), IsSelfMingGang(), IsAnGang(), SetRequiredCheck(), GetRequiredCheck()
	*/
	virtual bool UnDoGang(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bGang, enOperationGangType bGangType);


	// 普通明杠操作
	/*
		杠牌:别人打出的牌,普通明杠,将最后出牌玩家的最后一张牌删除
		如果是手牌则删除且添加到操作玩家胡牌中,如果是胡牌则改变类型（由碰改为杠）
		\param bSelfHuMJ 胡牌数组
		\param bSelfHuCount 胡牌数组大小
		\param bSelfHandMJ 手牌数组
		\param bSelfHandCount 手牌数组大小
		\param bLastOutMJ 最后出牌玩家的出牌数组
		\param bLastOutMJCount 最后出牌玩家的出牌数组大小
		\param bGang 要杠的牌
		\return 操作是否成功
		\note 如果是隐藏牌,手牌数量直接减2,将结构加到胡牌数组中.需要设置是否执行检查
		\sa UnDoNormalGang(), IsOutMingGang(), IsSelfMingGang(), IsAnGang(), ZMGang(), SetRequiredCheck(), GetRequiredCheck()
	*/
	//virtual bool NormalGang(MJBlock bSelfHuMJ[], MJBYTE &bSelfHuCount, MJBYTE bSelfHandMJ[], MJBYTE &bSelfHandCount, MJBYTE bLastOutMJ[], MJBYTE &bLastOutMJCount, MJBYTE bGang);

	// 自摸明杠和暗杠操作
	/*
		杠牌:自己摸的牌,分自摸明杠和暗杠,将摸得牌从手牌中删除
		自摸明杠:改变胡牌区的类型（由碰改为杠）
		暗杠:删除手牌
		\param bSelfHuMJ 胡牌数组
		\param bSelfHuCount 胡牌数组大小
		\param bSelfHandMJ 手牌数组
		\param bSelfHandCount 手牌数组大小
		\param bTouch 要杠的牌,即摸的牌
		\return 操作是否成功
		\note 如果是隐藏牌,手牌数量直接减2,将结构加到胡牌数组中.需要设置是否执行检查
		\sa IsOutMingGang(), IsSelfMingGang(), IsAnGang(), NormalGang(), SetRequiredCheck(), GetRequiredCheck()
	*/
	//virtual bool ZMGang(MJBlock bSelfHuMJ[], MJBYTE &bSelfHuCount, MJBYTE bSelfHandMJ[], MJBYTE &bSelfHandCount, MJBYTE bTouch);

	//辅助函数
public:
	//! 获取随机数
	/*!
		获取指定范围的随机数
		\param iMin 整数,随机范围左边界
		\param iMax 整数,随机范围右边界
		\return iMin到iMax之间的随机数
	*/
	virtual int GetRandomNum(int iMin, int iMax);

	//! 打乱排序
	/*!
		将传入的数组打乱顺序,打乱的次数在RANDOM_TIMES定义
		\param bAllMJ 牌数组
		\param iAllMJCount 牌数组大小
		\sa RANDOM_TIMES
	*/
	virtual void RandomMJ(MJBYTE bAllMJ[], int iAllMJCount);

	//! 将牌数据排序
	/*!
		万饼条按从小到大排序
		\param bAllMJ 牌数组
		\param iAllMJCount 牌数组大小
	*/
	virtual void SortHandMJ(MJBYTE bAllMJ[], int iAllMJCount);

	//! 校验牌是否存在
	/*!
		校验牌在指定牌数组中是否存在
		\param bAllMJ 牌数组
		\param iAllMJCount 牌数组大小
		\param bMJ 指定麻将牌
		\return true-存在;false-不存在
	*/
	virtual bool CheckMJExist(const MJBYTE bAllMJ[], int iAllMJCount, MJBYTE bMJ);

	//! 删除指定麻将
	/*!
		从牌数组中删除指定麻将,如果牌时隐藏的(牌数据为0),牌数量减1
		\param bAllMJ 牌数组
		\param iAllMJCount 牌数组大小
		\param bMJ 指定麻将牌
		\return true-删除成功;false-删除失败
	*/
	virtual bool DeleteMJ(MJBYTE bAllMJ[], int &iAllMJCount, MJBYTE bMJ);

	//! 获得指定麻将的数量
	/*!
		\param bAllMJ 牌数组
		\param iAllMJCount 牌数组大小
		\param bMJ 指定麻将牌
		\return 指定麻将在麻将数组中的数量
	*/
	virtual int GetMJCount(MJBYTE bAllMJ[], int &iAllMJCount, MJBYTE bMJ);

	//////////////////////////////////////////////////////////////////////////

	//判定函数
public:
	//! 吃牌判定
	/*!
		判定能否吃,手牌与打出的牌能否组成123的牌型
		\param bSelfHUMJ 胡牌数组
		\param iSelfHUMJCount 胡牌数组大小
		\param bSelfMJ 手牌数组
		\param iSelfMJCount 手牌数组大小
		\param bMJOut 别人出的牌
		\param bChiType 如果能吃,返回能吃的类型
		\return 是否能吃
		\\note 上家打的牌才能吃
		\sa Chi()
	*/
	virtual bool IsChi(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfMJ[], int iSelfMJCount, MJBYTE bMJOut, MJFLAG &bChiType);

	//! 碰牌判定
	/*!
		判定能否碰,手牌与打出的牌能否组成111的牌型.除出牌者以外才能碰
		\param bSelfHUMJ 胡牌数组
		\param iSelfHUMJCount 胡牌数组大小
		\param bSelfMJ 手牌数组
		\param iSelfMJCount 手牌数组大小
		\param bMJOut 别人出的牌
		\return 是否能碰
		\sa Peng()
	*/
	virtual bool IsPeng(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfMJ[], int iSelfMJCount, MJBYTE bMJOut);

	//! 判定能否明杠
	/*!
		判定能否明杠:别人出牌手牌中是否有3张相同的
		\param bSelfHUMJ 胡牌数组
		\param iSelfHUMJCount 胡牌数组大小
		\param bSelfMJ 手牌数组
		\param iSelfMJCount 手牌数组大小
		\param bMJOut 别人出的牌
		\return 是否能明杠
		\sa IsSelfMingGang(), IsAnGang(), NormalGang(), ZMGang()
	*/
	virtual bool IsOutMingGang(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfMJ[], int iSelfMJCount, MJBYTE bMJOut);

	//! 判定是否自摸明杠
	/*!
		判定能否自摸明杠:自己摸的牌是否与碰的牌相同
		\param bSelfHUMJ 胡牌数组
		\param iSelfHUMJCount 胡牌数组大小
		\param bMJTouch 摸的牌
		\return 是否能自摸明杠
		\sa IsOutMingGang(), IsAnGang(), NormalGang(), ZMGang()
	*/
	virtual bool IsSelfMingGang(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bMJTouch);

	//! 判定能否暗杠
	/*!
		判定能否暗杠:自己摸的牌才能暗杠,可能同时存在多个暗杠,将可杠的牌放入bGang
		\param bSelfHUMJ 胡牌数组
		\param iSelfHUMJCount 胡牌数组大小
		\param bSelfMJ 手牌数组
		\param iSelfMJCount 手牌数组大小
		\param bGang 杠牌数组
		\param iGangCount 杠牌数组大小
		\return 是否能暗杠
		\note 摸的牌已放入手牌
		\sa IsOutMingGang(), IsSelfMingGang(), NormalGang(), ZMGang()
	*/
	virtual bool IsAnGang(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfMJ[], int iSelfMJCount, MJBYTE bGang[], int &iGangCount);

		//! 判定能否暗杠
	/*!
		判定能否暗杠:自己摸的牌才能暗杠,可能同时存在多个暗杠,将可杠的牌放入bGang
		\param bSelfHUMJ 胡牌数组
		\param iSelfHUMJCount 胡牌数组大小
		\param bSelfMJ 手牌数组
		\param iSelfMJCount 手牌数组大小
		\param bMJTouch 摸的第14张牌
		\param bGang 杠牌数组
		\param iGangCount 杠牌数组大小
		\return 是否能暗杠
		\note 摸的牌还未放入手牌
		\sa IsOutMingGang(), IsSelfMingGang(), NormalGang(), ZMGang()
	*/
	virtual bool IsAnGang(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfMJ[], int iSelfMJCount, MJBYTE bMJTouch, MJBYTE bGang[], int &iGangCount);


	//! 判定能否旋风杠
	/*!
	判定能否旋风杠:自己摸的牌才能旋风杠,可能同时存在多个旋风杠,将可杠的牌放入bGang
	\param bSelfHUMJ 胡牌数组
	\param iSelfHUMJCount 胡牌数组大小
	\param bSelfMJ 手牌数组
	\param iSelfMJCount 手牌数组大小
	\param bMJTouch 摸的第14张牌
	\param bGang 杠牌数组
	\param iGangCount 杠牌数组大小
	\return 是否能旋风杠
	\note 摸的牌还未放入手牌
	\sa IsOutMingGang(), IsSelfMingGang(), NormalGang(), ZMGang()
	*/
	virtual bool IsXFGang(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfMJ[], int iSelfMJCount, MJBYTE bMJTouch, MJBYTE bGang[], int &iGangCount);


	//! 判定能否胡牌
	/*!
		判定能否胡牌:只判断是否能组成111 111 111 111 11一般胡法,特殊胡法需要在特定的派生类中实现逻辑
		\param bSelfHUMJ 胡牌数组
		\param iSelfHUMJCount 胡牌数组大小
		\param bSelfHandMJ 手牌数组
		\param iSelfHandMJCount 手牌数组大小
		\param bMJ 判定胡牌的第14张牌
		\return 是否能胡牌
		\sa IsOutMingGang(), IsSelfMingGang(), NormalGang(), ZMGang()
	*/
	virtual bool IsHu(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfHandMJ[], int iSelfHandMJCount, MJBYTE bMJ);
};

#endif
