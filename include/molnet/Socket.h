/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * Socket implementable class.
 *
 */

#ifndef SOCKET_H
#define SOCKET_H

#include "SocketDefines.h"
#include "SocketOps.h"

class CMolMessageOut;

class Socket : public NedAllocatedObject
{
	public:
		// Constructor. If fd = 0, it will be assigned
		Socket(SOCKET fd, uint32 sendbuffersize, uint32 recvbuffersize);

		// Destructor.
		virtual ~Socket();

		// Open a connection to another machine.
		bool Connect(const char* Address, uint32 Port);

		// Disconnect the socket.
		void Disconnect();

		// Accept from the already-set fd.
		void Accept(sockaddr_in* address);

		/* Implementable methods */

		// Called when data is received.
		virtual void OnRead(uint32 size) {}

		// Called when a connection is first successfully established.
		virtual void OnConnect() {}

		// Called when the socket is disconnected from the client (either forcibly or by the connection dropping)
		virtual void OnDisconnect() {}

		/* Sending Operations */

		// Locks sending mutex, adds bytes, unlocks mutex.
		bool Send(const uint8* Bytes, uint32 Size);

		bool Send(CMolMessageOut &out);

	    /// 发送html5数据
    	bool SendHtml5(const uint8 * Bytes,uint32 Size);

		// Burst system - Locks the sending mutex.
		inline void BurstBegin() { m_writeMutex.Acquire(); }

		// Burst system - Adds bytes to output buffer.
		bool BurstSend(const uint8* Bytes, uint32 Size);

		// Burst system - Pushes event to queue - do at the end of write events.
		void BurstPush();

		// Burst system - Unlocks the sending mutex.
		inline void BurstEnd() { m_writeMutex.Release(); }

		/* Client Operations */

		// Get the client's ip in numerical form.
		string GetRemoteIP();
		inline uint32 GetRemotePort() { return ntohs(m_client.sin_port); }
		inline SOCKET GetFd() { return m_fd; }
		inline void SetFd(SOCKET fd)
		{
            m_fd = fd;

            if(m_fd == 0)
            {
                m_fd = SocketOps::CreateTCPFileDescriptor();
            }
		}

        inline CircularBuffer & GetReadBuffer() { return readBuffer; }
        inline CircularBuffer & GetWriteBuffer() { return writeBuffer; }

		/* Platform-specific methods */

		void SetupReadEvent();
		void ReadCallback(uint32 len);
		void WriteCallback();

        inline void Clear(void)
        {
            m_connected.SetVal(false);
            m_deleted.SetVal(false);
			m_html5connected.SetVal(false);
            m_writeLock.SetVal(false);
            m_htmlMsgProcessed.SetVal(false);
            m_eventCount.SetVal(0);

           	m_readTimer.SetVal(0);
            m_eventCount.SetVal(0);
            m_readMsgCount.SetVal(0);
            m_readMsgBool.SetVal(true);
            isRealRemovedFromSet.SetVal(false);

            m_htmlMsgProcessed.SetVal(false);
            m_buffer_pos = 0;
            nMinExpectedSize = 6;
            masksOffset = 0;
            payloadSize = 0;

            readBuffer.Remove(readBuffer.GetSize());
            writeBuffer.Remove(writeBuffer.GetSize());

            m_BytesSent=0;
            m_BytesRecieved=0;
            m_heartJitter = time(NULL);

            remaining = 0;
            compress = 0;
            opcode = 0;
			mchecksum = 0;
        }
        //设置心跳计数
        inline void SetHeartCount(time_t count) { m_heartJitter = count; }
        //获得心跳计数
        inline time_t GetHeartCount(void) { return m_heartJitter; }
		inline bool IsDeleted()
		{
			return m_deleted.GetVal();
		}
		inline bool IsConnected()
		{
			return m_connected.GetVal();
		}
		inline sockaddr_in & GetRemoteStruct() { return m_client; }

		void Delete();

		inline in_addr GetRemoteAddress() { return m_client.sin_addr; }


		CircularBuffer readBuffer;
		CircularBuffer writeBuffer;

		AtomicCounter m_eventCount;
        AtomicBoolean isRealRemovedFromSet;

	protected:

		// Called when connection is opened.
		void _OnConnect();

		SOCKET m_fd;

		Mutex m_writeMutex;
		Mutex m_readMutex;

		// we are connected? stop from posting events.
		AtomicBoolean m_connected;

		// We are deleted? Stop us from posting events.
		AtomicBoolean m_deleted;

	    // html5端是否真正连接成功
	    AtomicBoolean m_html5connected;

	    char m_buffer[MOL_REV_BUFFER_SIZE_TWO];                /**< ÓÃÓÚ´æ´¢ÊÕµ½µÄÊý¾Ý */
	    unsigned long m_buffer_pos;
	    AtomicBoolean m_htmlMsgProcessed;
        int nMinExpectedSize;
        int masksOffset;
        int64 payloadSize;

		sockaddr_in m_client;

		unsigned long m_BytesSent;
		unsigned long m_BytesRecieved;

        time_t					m_heartJitter;

        AtomicULong             m_readTimer;
        AtomicCounter           m_readMsgCount;
        AtomicBoolean           m_readMsgBool;

        uint32 remaining;
        uint16 opcode;
        uint16 compress;
		uint32 mchecksum;

	public:
		// Atomic wrapper functions for increasing read/write locks
		inline void IncSendLock() { ++m_writeLock; }
		inline void DecSendLock() { --m_writeLock; }
		inline bool AcquireSendLock()
		{
			if(m_writeLock.SetVal(1) != 0)
				return false;
			else
				return true;
		}

	private:
		// Write lock, stops multiple write events from being posted.
		AtomicCounter m_writeLock;

		/* Win32 - IOCP Specific Calls */
#ifdef CONFIG_USE_IOCP

	public:

		// Set completion port that this socket will be assigned to.
		inline void SetCompletionPort(HANDLE cp) { m_completionPort = cp; }

		OverlappedStruct m_readEvent;
		OverlappedStruct m_writeEvent;

	private:
		// Completion port socket is assigned to
		HANDLE m_completionPort;

		// Assigns the socket to his completion port.
		void AssignToCompletionPort();

#endif

		/* Linux - EPOLL Specific Calls */
#ifdef CONFIG_USE_EPOLL
	public:
		// Posts a epoll event with the specifed arguments.
		void PostEvent(uint32 events);

		inline bool HasSendLock()
		{
			bool res;
			res = (m_writeLock.GetVal() != 0);
			return res;
		}
#endif

		/* FreeBSD - kqueue specific calls */
#ifdef CONFIG_USE_KQUEUE
	public:
		// Posts a epoll event with the specifed arguments.
		void PostEvent(int events, bool oneshot);
		ARCEMU_INLINE bool HasSendLock()
		{
			bool res;
			res = (m_writeLock.GetVal() != 0);
			return res;
		}
#endif

	public:
		// Polls and resets the traffic data
		void PollTraffic(unsigned long* sent, unsigned long* recieved)
		{

			m_writeMutex.Acquire();
			*sent = m_BytesSent;
			*recieved = m_BytesRecieved;
			m_BytesSent = 0;
			m_BytesRecieved = 0;

			m_writeMutex.Release();
		}
};

/** Connect to a server.
* @param hostname Hostname or IP address to connect to
* @param port Port to connect to
* @return templated type if successful, otherwise null
*/
template<class T>
T* ConnectTCPSocket(const char* hostname, u_short port)
{
	sockaddr_in conn;
	hostent* host;

	/* resolve the peer */
	host = gethostbyname(hostname);

	if(!host)
		return NULL;

	/* copy into our address struct */
	memcpy(&conn.sin_addr, host->h_addr_list[0], sizeof(in_addr));
	conn.sin_family = AF_INET;
	conn.sin_port = ntohs(port);

	T* s = new T(0);
	if(!s->Connect(hostname, port))
	{
		s->Delete();
		return 0;
	}
	return s;
}

/* Socket Garbage Collector */
#define SOCKET_GC_TIMEOUT 15

class SocketGarbageCollector : public Singleton<SocketGarbageCollector>
{
		map<Socket*, time_t> deletionQueue;
		Mutex lock;
	public:
		~SocketGarbageCollector()
		{
			map<Socket*, time_t>::iterator i;
			for(i = deletionQueue.begin(); i != deletionQueue.end(); ++i)
				delete i->first;
		}

		void Update()
		{
			map<Socket*, time_t>::iterator i, i2;
			time_t t = UNIXTIME;
			lock.Acquire();
			for(i = deletionQueue.begin(); i != deletionQueue.end();)
			{
				i2 = i++;
				if(i2->second <= t &&
                    i2->first->isRealRemovedFromSet.GetVal())
				{
					delete i2->first;
					deletionQueue.erase(i2);
				}
			}
			lock.Release();
		}

		void QueueSocket(Socket* s)
		{
			lock.Acquire();
			deletionQueue.insert(map<Socket*, time_t>::value_type(s, UNIXTIME + SOCKET_GC_TIMEOUT));
			lock.Release();
		}

        inline void DeleteSocket(void)
        {
            lock.Acquire();
            map<Socket*, time_t>::iterator i;
            for(i=deletionQueue.begin();i!=deletionQueue.end();++i)
            {
                Socket *pSocket = i->first;
                if(pSocket == NULL) continue;

                delete pSocket;
                pSocket = NULL;
            }
            deletionQueue.clear();
            lock.Release();
        }
};

#define sSocketGarbageCollector SocketGarbageCollector::getSingleton()

class NetClient : public Socket
{
public:
	/// 构造函数
	NetClient(SOCKET fd);
	/// 析构函数
	~NetClient();

	/// 当数据达到时调用
	virtual void OnRead(uint32 size);
	/// 当一个连接成功建立时调用
	virtual void OnConnect();
	/// 当一个连接断开时调用
	virtual void OnDisconnect();
};

class MolNetworkUpdate : public ThreadBase
{
public:
	/// 构造函数
	MolNetworkUpdate();
	/// 析构函数
	~MolNetworkUpdate();

	bool run();

private:
	unsigned long m_curTime,m_TimeSpeed;
	unsigned long m_threadTimer,m_threadTimeSpeed;
	unsigned long m_UpdateTime,m_UpdateTimeSpeed;
};

extern MolNetworkUpdate *m_NetworkUpdate;

#endif
