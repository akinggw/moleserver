CC = gcc
CPP = g++
RM = rm -f

ifeq ($(debug),1)
	CFLAGS = -I ../../include/molnet -O0 -shared -g -Wall -fexceptions -fPIC -DLINUX -lpthread -L ../../libs -lmolnet -lmysqlclient -ljsoncpp
else
	CFLAGS = -I ../../include/molnet -O2 -shared -fexceptions -fPIC -DLINUX -lpthread -L ../../libs -lmolnet -lmysqlclient -ljsoncpp
endif

OBJDIR = objs
BIN = ../../bin/games
CPPOBJS = $(OBJDIR)/ServerLogicModule.o \
       $(OBJDIR)/cserverlogicframe.o \
       $(OBJDIR)/GameLogic.o \
       $(OBJDIR)/crobotlogicframe.o \
       $(OBJDIR)/CServerServiceManager.o \

TARGET = $(BIN)/libgame_ddz.so
$(TARGET):$(OBJDIR) $(BIN) $(CPPOBJS)
	$(CPP) -o $@ $(OBJDIR)/*.o $(CFLAGS)
#	ar -cr $(TARGET) $(OBJDIR)/*.o

$(OBJDIR):
	mkdir -p ./$@

$(BIN):
	mkdir -p ./$@

$(CPPOBJS):$(OBJDIR)/%.o:$(SRCS)%.cpp
	$(CPP) -c $(CFLAGS) $< -o $@

.PHONY : clearn
clean:
	-$(RM) $(TARGET)
	-$(RM) $(OBJDIR)/*.o -r
