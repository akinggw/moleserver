/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * SocketMgr - epoll manager for Linux.
 *
 */


#ifndef SOCKETMGR_LINUX_H
#define SOCKETMGR_LINUX_H

#include "SocketDefines.h"
#ifdef CONFIG_USE_EPOLL

#define SOCKET_HOLDER_SIZE 30000    // You don't want this number to be too big, otherwise you're gonna be eating
// memory. 65536 = 256KB, so thats no big issue for now, and I really can't
// see anyone wanting to have more than 65536 concurrent connections.

#define THREAD_EVENT_SIZE 4096      // This is the number of socket events each thread can receieve at once.
// This default value should be more than enough.

class Socket;
class SocketWorkerThread;
class ListenSocketBase;

class SocketMgr : public Singleton<SocketMgr>
{
		/// /dev/epoll instance handle
		int epoll_fd;

		// fd -> pointer binding.
		__gnu_cxx::hash_map<uint32,Socket*> fds;
		std::list<Socket*> _freesockets;
        Mutex _socketLock,_freesocketLock;

		ListenSocketBase* listenfds[SOCKET_HOLDER_SIZE];

		/// socket counter
		AtomicCounter socket_count;

		int max_fd;

        std::list<MessageStru> _MesList;
        Mutex _mesLock;

        std::list<ThreadBase*> _ThreadBases;

	public:

		/// friend class of the worker thread -> it has to access our private resources
		friend class SocketWorkerThread;

		/// constructor > create epoll device handle + initialize event set
		SocketMgr()
		{
			epoll_fd = epoll_create(SOCKET_HOLDER_SIZE);
			if(epoll_fd == -1)
			{
				sLog.outError("Could not create epoll fd (/dev/epoll).");
				exit(-1);
			}

			// null out the pointer array
			fds.clear();
			memset(listenfds, 0, sizeof(void*) * SOCKET_HOLDER_SIZE);
			max_fd = 0;
		}

		/// destructor > destroy epoll handle
		~SocketMgr()
		{
		    ClearMesList();

		    ClearTasks();

			// close epoll handle
			close(epoll_fd);
		}

		/// add a new socket to the epoll set and to the fd mapping
		void AddSocket(Socket* s);
		void AddListenSocket(ListenSocketBase* s);
		Socket* GetSocket(uint32 fd);
		inline void LockSocketList(void) { _socketLock.Acquire(); }
		inline void UnlockSocketList(void) { _socketLock.Release(); }

		/// remove a socket from epoll set/fd mapping
		void RemoveSocket(Socket* s);
		/// add a free socket
		void AddFreeSocket(Socket *s);
		/// get a free socket
		Socket* GetFreeSocket(void);

		/// returns epoll fd
		inline int GetEpollFd() { return epoll_fd; }

		/// closes all sockets
		void CloseAll();

		/// spawns worker threads
		void SpawnWorkerThreads();

		/// show status
		void ShowStatus();

		/// update
		void Update(void);

        /// add task
        inline void AddTask(ThreadBase *task)
        {
            if(task == NULL) return;

            _ThreadBases.push_back(task);
        }
		/// clear all tasks
		void ClearTasks(void);

		/// add one mes to meslist
		inline void PushMessage(MessageStru mes)
		{
		    _mesLock.Acquire();
		    _MesList.push_back(mes);
		    _mesLock.Release();
		}
		/// get cout of meslist
		inline int GetMesCount(void)
		{
		    return (int)_MesList.size();
		}
		/// attempt lock meslist
		inline bool LockMesList(void)
		{
		    return _mesLock.AttemptAcquire();
		}
		/// unlock meslist
		inline void UnlockMesList(void)
		{
		    _mesLock.Release();
		}
		/// get meslist
		inline std::list<MessageStru>* GetMesList(void)
		{
		    return &_MesList;
		}
		/// clear meslist
		void ClearMesList(void);
};

class SocketWorkerThread : public ThreadBase
{
		/// epoll event struct
		struct epoll_event events[THREAD_EVENT_SIZE];
		bool running;
	public:
		bool run();
		void OnShutdown()
		{
			running = false;
		}
};

#define sSocketMgr SocketMgr::getSingleton()

#endif

#endif
