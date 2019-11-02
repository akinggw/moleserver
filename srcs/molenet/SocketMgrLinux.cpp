/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * SocketMgr - epoll manager for Linux.
 *
 */

#include "../../include/molnet/Network.h"
#ifdef CONFIG_USE_EPOLL

//#define ENABLE_ANTI_DOS

initialiseSingleton(SocketMgr);
void SocketMgr::AddSocket(Socket* s)
{
    _socketLock.Acquire();

#ifdef ENABLE_ANTI_DOS
	uint32 saddr;
	int i, count;

	// Check how many connections we already have from that ip
	saddr = s->GetRemoteAddress().s_addr;
	__gnu_cxx::hash_map<uint32,Socket*>::iterator iter = fds.begin();
	for(; iter != fds.end(); ++iter)
	{
		if((*iter).second)
		{
			if((*iter).second->GetRemoteAddress().s_addr == saddr) count++;
		}
	}

	// More than 16 connections from the same ip? enough! xD
	if(count > 16)
	{
		s->Disconnect(false);
		_socketLock.Release();
		return;
	}
#endif

	if(fds[s->GetFd()] != NULL)
	{
		//fds[s->GetFd()]->Delete();
		//fds[s->GetFd()] = NULL;
		s->Delete();
		_socketLock.Release();
		return;
	}

	if(max_fd < s->GetFd()) max_fd = s->GetFd();
	fds[s->GetFd()] = s;
	s->isRealRemovedFromSet.SetVal(false);
	++socket_count;

	// Add epoll event based on socket activity.
	struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
	ev.events = (s->writeBuffer.GetSize()) ? EPOLLOUT : EPOLLIN;
	ev.events |= EPOLLET;			/* use edge-triggered instead of level-triggered because we're using nonblocking sockets */
	ev.data.fd = s->GetFd();

	if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, ev.data.fd, &ev))
		Log.Error("epoll", "Could not add event to epoll set on fd %u", ev.data.fd);
    _socketLock.Release();
}

void SocketMgr::ClearTasks(void)
{
	if(_ThreadBases.empty()) return;

	std::list<ThreadBase*>::iterator iter = _ThreadBases.begin();
	for(;iter!=_ThreadBases.end();++iter)
		if((*iter)) delete (*iter);
	_ThreadBases.clear();
}

void SocketMgr::Update(void)
{
    try
    {
        _socketLock.Acquire();
        __gnu_cxx::hash_map<uint32,Socket*>::iterator itr = fds.begin();
        Socket * s=NULL;
        time_t t = time(NULL);

        for(itr = fds.begin();itr != fds.end();)
        {
            s = (*itr).second;

            if(s != NULL)
            {
                time_t diff = 0;

                diff = t - s->GetHeartCount();

                //char strTmp[256];
                //sprintf(strTmp,"logger:Is running??%lld %lld %ld %d",s->GetHeartCount(),diff,s->m_eventCount.GetVal(),(int)_sockets.size());
                //::OutputDebugString(strTmp);

                if(diff > 4 && s->m_eventCount.GetVal() <= 0)		   // More than 2mins
                {
                    s->Disconnect();
                    fds.erase(itr++);
					listenfds[s->GetFd()]=NULL;
                    --socket_count;

                    SocketOps::CloseSocket(s->GetFd());

                    // add free socket list
                    AddFreeSocket(s);
                 }
                else
                {
                    //if(diff > 5)
                    //{
                    //	CMolMessageOut out(IDD_MESSAGE_HEART_BEAT);
                    //	s->Send(out);
                    //}

                    ++itr;
                }
            }
            else
            {
                ++itr;
            }
        }
        _socketLock.Release();
    }
    catch (std::exception e)
    {
        _socketLock.Release();

        char str[256];
        sprintf(str,"socket更新异常:%s\n",e.what());
        LOG_DEBUG(str);
    }
}

Socket* SocketMgr::GetSocket(uint32 fd)
{
    Socket *pSocket = NULL;

    __gnu_cxx::hash_map<uint32,Socket*>::iterator iterfind = fds.find(fd);
	if(iterfind != fds.end())
        pSocket = (*iterfind).second;

    return pSocket;
}

void SocketMgr::AddListenSocket(ListenSocketBase* s)
{
	assert(listenfds[s->GetFd()] == 0);
	listenfds[s->GetFd()] = s;

	// Add epoll event based on socket activity.
	struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
	ev.events = EPOLLIN;
	ev.events |= EPOLLET;			/* use edge-triggered instead of level-triggered because we're using nonblocking sockets */
	ev.data.fd = s->GetFd();

	if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, ev.data.fd, &ev))
		Log.Error("epoll", "Could not add event to epoll set on fd %u", ev.data.fd);
}

void SocketMgr::RemoveSocket(Socket* s)
{
    _socketLock.Acquire();
	if(fds[s->GetFd()] != s)
	{
		Log.Error("epoll", "Could not remove fd %u from the set due to it not existing?", s->GetFd());
		_socketLock.Release();
		return;
	}
	fds[s->GetFd()] = NULL;
	listenfds[s->GetFd()]=NULL;
	--socket_count;

    __gnu_cxx::hash_map<uint32,Socket*>::iterator iterfind = fds.find(s->GetFd());
	if(iterfind != fds.end())
        fds.erase(iterfind);

	// Remove from epoll list.
	struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
	ev.data.fd = s->GetFd();
	ev.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP | EPOLLONESHOT;

	if(epoll_ctl(epoll_fd, EPOLL_CTL_DEL, ev.data.fd, &ev))
		Log.Error("epoll", "Could not remove fd %u from epoll set, errno %u", s->GetFd(), errno);
    _socketLock.Release();
}

void SocketMgr::CloseAll()
{
    _socketLock.Acquire();
	__gnu_cxx::hash_map<uint32,Socket*>::iterator iter = fds.begin();
	for(; iter != fds.end(); ++iter)
    {
		if((*iter).second != NULL)
		{
            SocketOps::CloseSocket( (*iter).second->GetFd() );
            sSocketGarbageCollector.QueueSocket((*iter).second);
		}
    }
	_socketLock.Release();

    _freesocketLock.Acquire();
	std::list<Socket*>::iterator iterfree = _freesockets.begin();
	for(; iterfree != _freesockets.end();++iterfree)
	{
	    if((*iterfree) != NULL)
	    {
            SocketOps::CloseSocket( (*iterfree)->GetFd() );
            sSocketGarbageCollector.QueueSocket((*iterfree));
	    }
	}
	_freesocketLock.Release();
}

void SocketMgr::AddFreeSocket(Socket *s)
{
    if(!s) return;

    try
    {
        _freesocketLock.Acquire();

        if((int)_freesockets.size() > IDD_MAX_FREE_SOCKET)
        {
            s->isRealRemovedFromSet.SetVal(true);
            sSocketGarbageCollector.QueueSocket(s);

            _freesocketLock.Release();
            return;
        }

        _freesockets.push_back(s);
        _freesocketLock.Release();
    }
    catch (std::exception e)
    {
        _freesocketLock.Release();

        char str[256];
        sprintf(str,"加入空闲socket异常:%s\n",e.what());
        LOG_DEBUG(str);
    }
}

Socket* SocketMgr::GetFreeSocket(void)
{
    Socket *pSocket = NULL;

    if(_freesockets.empty()) return NULL;

    _freesocketLock.Acquire();
    pSocket = (*_freesockets.begin());
    _freesockets.erase(_freesockets.begin());
    _freesocketLock.Release();

    pSocket->Clear();

    return pSocket;
}

void SocketMgr::SpawnWorkerThreads()
{
	uint32 count = 1/*sysconf(_SC_NPROCESSORS_ONLN)*/;
	for(uint32 i = 0; i < count; ++i)
		ThreadPool.ExecuteTask(new SocketWorkerThread());
}

void SocketMgr::ShowStatus()
{
	sLog.outString("sockets count = %u", socket_count.GetVal());
}

/// clear meslist
void SocketMgr::ClearMesList(void)
{
	if(_MesList.empty()) return;

	_mesLock.Acquire();
	std::list<MessageStru>::iterator iter = _MesList.begin();
	for(;iter!=_MesList.end();++iter)
	{
		if((*iter).mes)
			SafeDelete((*iter).mes);
	}
	_MesList.clear();
	_mesLock.Release();
}

bool SocketWorkerThread::run()
{
	int fd_count;
	Socket* ptr;
	int i;
	running = true;
	SocketMgr* mgr = SocketMgr::getSingletonPtr();

	while(network::IsConnected())
	{
		fd_count = epoll_wait(mgr->epoll_fd, events, THREAD_EVENT_SIZE, 5000);

		for(i = 0; i < fd_count; ++i)
		{
			if(events[i].data.fd >= SOCKET_HOLDER_SIZE)
			{
				Log.Error("epoll", "Requested FD that is too high (%u)", events[i].data.fd);
				continue;
			}

            mgr->LockSocketList();
            ptr = sSocketMgr.GetSocket(events[i].data.fd);
			//ptr = mgr->fds[events[i].data.fd];
			mgr->UnlockSocketList();

			if(events[i].data.fd == m_ServerSocket->GetFd() || ptr == NULL)
			{
				if((ptr = ((Socket*)mgr->listenfds[events[i].data.fd])) != NULL)
					((ListenSocketBase*)ptr)->OnAccept();
				else
					Log.Error("epoll", "Returned invalid fd (no pointer) of FD %u", events[i].data.fd);

                continue;
			}

			if(events[i].events & EPOLLHUP || events[i].events & EPOLLERR)
			{
				ptr->Disconnect();
				continue;
			}
			else if(events[i].events & EPOLLIN)
			{
				ptr->ReadCallback(0);               // Len is unknown at this point.

				/* changing to written state? */
				if(ptr->writeBuffer.GetSize() && !ptr->HasSendLock() && ptr->IsConnected())
					ptr->PostEvent(EPOLLOUT);
			}
			else if(events[i].events & EPOLLOUT)
			{
				ptr->BurstBegin();          // Lock receive mutex
				ptr->WriteCallback();       // Perform actual send()
				if(ptr->writeBuffer.GetSize() > 0)
				{
					/* we don't have to do anything here. no more oneshots :) */
				}
				else
				{
					/* change back to a read event */
					ptr->DecSendLock();
					ptr->PostEvent(EPOLLIN);
				}
				ptr->BurstEnd();            // Unlock

				if(ptr->m_eventCount.GetVal() > 0)
                    --(ptr->m_eventCount);
			}
		}
	}
	return true;
}

#endif
