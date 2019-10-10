#include "ServerLogicModule.h"

#include "CServerServiceManager.h"

//全局变量
static CServerServiceManager			g_ServerServiceManager;				//管理变量

//建立对象函数
extern "C"
{
    void * CreateServerServiceManager(void)
    {
        return &g_ServerServiceManager;
    }
}
