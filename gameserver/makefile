CC = gcc
CPP = g++
VPATH = ./
RM = rm -f

ifeq ($(debug),1)
	CFLAGS = -I ../include/molnet -O0 -g -Wall  -ldl -lstdc++ -lpthread -L ../libs -lmolnet -lmysqlclient -ljsoncpp
else
	CFLAGS = -I ../include/molnet -O2  -ldl -lstdc++ -lpthread -L ../libs -lmolnet -lmysqlclient -ljsoncpp
endif

OBJDIR = objs
BIN = ../bin
OBJS_GF = $(OBJDIR)/DBOperator.o \
	   $(OBJDIR)/CPlayer.o \
	   $(OBJDIR)/PlayerManager.o \
	   $(OBJDIR)/CRoom.o \
	   $(OBJDIR)/RoomManager.o \
	   $(OBJDIR)/CRobotManager.o \

OBJS_VPM = $(OBJDIR)/CVideoPlaybackManager.o \

OBJS_MATCH = $(OBJDIR)/CBasicMatchingMode.o \
	   $(OBJDIR)/CDieOutMatchingMode.o \

OBJS = $(OBJDIR)/GameFrameManager.o \
	   $(OBJDIR)/gameparamconfig.o \
	   $(OBJDIR)/game_server.o \

TARGET = $(BIN)/game_server
$(TARGET):$(OBJDIR) $(BIN) $(OBJS) $(OBJS_GF) $(OBJS_VPM) $(OBJS_MATCH)
	$(CPP) -o $@ $(OBJDIR)/*.o $(CFLAGS)

$(OBJDIR):
	mkdir -p ./$@

$(BIN):
	mkdir -p ./$@

$(OBJS_GF):$(OBJDIR)/%.o:gameframe/%.cpp
	$(CPP) -c -g $(CFLAGS) $< -o $@

$(OBJS_VPM):$(OBJDIR)/%.o:videoPlayback/%.cpp
	$(CPP) -c -g $(CFLAGS) $< -o $@

$(OBJS_MATCH):$(OBJDIR)/%.o:matching/%.cpp
	$(CPP) -c -g $(CFLAGS) $< -o $@

$(OBJS):$(OBJDIR)/%.o:%.cpp
	$(CPP) -c -g $(CFLAGS) $< -o $@

.PHONY : clearn
clean:
	-$(RM) $(TARGET)
	-$(RM) $(OBJDIR)/*.o -r
