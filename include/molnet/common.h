#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <cstdlib>
#include <cstdio>

#include <cstdarg>
#include <ctime>
#include <cmath>
#include <cerrno>

#define CONFIG_USE_EPOLL

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#  define WIN32_LEAN_AND_MEAN
//#  define _WIN32_WINNT 0x0500
#  define NOMINMAX
#  include <windows.h>
#  undef NOMINMAX
#else
#  include <cstring>
#  define MAX_PATH 1024
#endif

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <pthread.h>
#endif

#include <cassert>
#include <cstdlib>
#include <set>
#include <list>
#include <string>
#include <map>
#include <queue>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <climits>
#include <cstdlib>
#include <ext/hash_map>

#if COMPILER != COMPILER_GNU
typedef signed __int64 int64;
typedef signed __int32 int32;
typedef signed __int16 int16;
typedef signed __int8 int8;

typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned __int16 uint16;
typedef unsigned __int8 uint8;
#else

typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;
typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;
typedef unsigned long int DWORD;
typedef uint8_t BYTE;
typedef uint16_t WORD;
#endif

#include "Singleton.h"
#include "Log.h"
#include "Mutex.h"

#include "AtomicULong.h"
#include "AtomicCounter.h"
#include "AtomicBoolean.h"
#include "ThreadStarter.h"
#include "ThreadPool.h"

#define THREAD_RESERVE 10
#define MOL_NETWORK_VERSION 100                     // 网络消息协议版本
//#define MOL_REV_BUFFER_SIZE 20480
#define MOL_REV_BUFFER_SIZE_TWO 10240
#define MOL_STR_BUFFER_SIZE 5000
#define MOL_CONN_POOL_MAX 5                        // 数据库连接池中最大连接

#include "netallocator/NedAllocatedObject.h"
#include "rc4.h"
#include "MolNetMessage.h"

#define IDD_SECOND_MSG_MAX_COUNT      50               // 每秒最多处理的消息条数
#define IDD_CLIENT_CONNECT_MAX_COUNT  10               // 客户端最大连接次数
#define IDD_MESSAGE_HEART_BEAT        100              // 心跳消息
#define IDD_MAX_FREE_SOCKET           2000             // 空闲连接

//删除指针
#define  SafeDelete(pData) { try { delete pData; } catch (...) { assert(false); } pData=NULL; }
//计算数组维数
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

#ifndef WIN32
     #define _atoi64(val)     strtoll(val, NULL, 10)
#endif

#endif // COMMON_H_INCLUDED
