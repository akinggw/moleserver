#include "../../include/molnet/dataproviderfactory.h"

#include "../../include/molnet/mysqldataprovider.h"

/**
 * 构造函数
 */
DataProviderFactory::DataProviderFactory(void)
	throw()
{

}

/**
 * 析构函数
 */
DataProviderFactory::~DataProviderFactory(void)
	throw()
{

}

/**
 * 建立一个新的数据库管理器
 *
 * @return 返回我们建立的数据库管理器
 */
DataProvider* DataProviderFactory::createDataProvider(void)
{
	MySqlDataProvider *provider = new MySqlDataProvider();
	return provider;
}
