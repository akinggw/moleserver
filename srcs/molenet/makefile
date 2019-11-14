CC = gcc
CPP = g++
RM = rm -f

ifeq ($(debug),1)
	CFLAGS = -I ../../include/molnet -O0 -shared -g -Wall -fexceptions -fPIC -DLINUX -lpthread -lmysqlclient -lhiredis
else
	CFLAGS = -I ../../include/molnet -O2 -shared -fexceptions -fPIC -DLINUX -lpthread -lmysqlclient -lhiredis
endif

OBJDIR = objs
BIN = ../../libs
SRCS = ./netallocator/
SRCS2 = ./html5/
SRCXREDIS = ./xredis/
OBJS = $(OBJDIR)/nedmalloc.o \
       $(OBJDIR)/NedAllocatorImpl.o \

CPPOBJS = $(OBJDIR)/NedAllocatedObject.o \

CPPOBJXREDIS = $(OBJDIR)/xRedisClient.o \
			   $(OBJDIR)/xRedisClient_connection.o \
			   $(OBJDIR)/xRedisClient_hashs.o \
			   $(OBJDIR)/xRedisClient_keys.o \
			   $(OBJDIR)/xRedisClient_lists.o \
			   $(OBJDIR)/xRedisClient_pubsub.o \
			   $(OBJDIR)/xRedisClient_sets.o \
			   $(OBJDIR)/xRedisClient_sortedsets.o \
			   $(OBJDIR)/xRedisClient_strings.o \
			   $(OBJDIR)/xRedisClusterClient.o \
			   $(OBJDIR)/xRedisPool.o \

CPPOBJS3 = $(OBJDIR)/base64.o \
					       $(OBJDIR)/sha.o \
						   $(OBJDIR)/sha1.o \
						   $(OBJDIR)/WebsocketHandshakeMessage.o \
						   $(OBJDIR)/shacmp.o \
						   $(OBJDIR)/WebsocketDataMessage.o \

CPPOBJS2 = $(OBJDIR)/AtomicBoolean.o \
		   $(OBJDIR)/AtomicCounter.o \
		   $(OBJDIR)/AtomicULong.o \
		   $(OBJDIR)/CircularBuffer.o \
		   $(OBJDIR)/connectPool.o \
		   $(OBJDIR)/dataprovider.o \
		   $(OBJDIR)/dataproviderfactory.o \
		   $(OBJDIR)/Log.o \
		   $(OBJDIR)/MolMessageIn.o \
		   $(OBJDIR)/MolMessageOut.o \
		   $(OBJDIR)/MolNetMessage.o \
		   $(OBJDIR)/Mutex.o \
		   $(OBJDIR)/mysqldataprovider.o \
		   $(OBJDIR)/network.o \
		   $(OBJDIR)/rc4.o \
		   $(OBJDIR)/recordset.o \
		   $(OBJDIR)/Socket.o \
		   $(OBJDIR)/SocketLinux.o \
		   $(OBJDIR)/SocketMgrLinux.o \
		   $(OBJDIR)/SocketOpsLinux.o \
		   $(OBJDIR)/ThreadPool.o \
		   $(OBJDIR)/CTcpSocketClient.o \

TARGET = $(BIN)/libmolnet.a
$(TARGET):$(OBJDIR) $(BIN) $(OBJS) $(CPPOBJS) $(CPPOBJS2) $(CPPOBJS3) $(CPPOBJXREDIS)
#	$(CPP) -o $@ $(OBJDIR)/*.o $(CFLAGS)
	ar -cr $(TARGET) $(OBJDIR)/*.o

$(OBJDIR):
	mkdir -p ./$@

$(BIN):
	mkdir -p ./$@

$(OBJS):$(OBJDIR)/%.o:$(SRCS)%.c
	$(CPP) -c $(CFLAGS) $< -o $@

$(CPPOBJS):$(OBJDIR)/%.o:$(SRCS)%.cpp
	$(CPP) -c $(CFLAGS) $< -o $@

$(CPPOBJS3):$(OBJDIR)/%.o:$(SRCS2)%.cpp
	$(CPP) -c $(CFLAGS) $< -o $@

$(CPPOBJXREDIS):$(OBJDIR)/%.o:$(SRCXREDIS)%.cpp
	$(CPP) -c $(CFLAGS) $< -o $@

$(CPPOBJS2):$(OBJDIR)/%.o:%.cpp
	$(CPP) -c $(CFLAGS) $< -o $@

.PHONY : clearn
clean:
	-$(RM) $(TARGET)
	-$(RM) $(OBJDIR)/*.o -r
