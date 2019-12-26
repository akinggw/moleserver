#ifndef _DATA_PROVIDER_FACTORY_H_INCLUDE
#define _DATA_PROVIDER_FACTORY_H_INCLUDE

/**
* MolNet网络引擎
*
* 描述:用于建立mysql引擎
* 作者:akinggw
* 日期:2010.3.2
*/

#include "common.h"

class DataProvider;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated"

/**
 * 用于建立数据库管理的一个工厂类
 */
class DataProviderFactory
{
public:
	/// 建立一个新的数据库管理器
	static DataProvider* createDataProvider(void);

private:
	/// 构造函数
	DataProviderFactory(void)
		throw();
	/// 析构函数
	~DataProviderFactory(void)
		throw();
};

#pragma clang diagnostic pop

#endif
