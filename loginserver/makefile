CC = gcc
CPP = g++
VPATH = ./
RM = rm -f

ifeq ($(debug),1)
	CFLAGS = -I ../include/molnet -O0 -g -Wall -ldl -lstdc++ -lpthread -L ../libs -lmolnet -lmysqlclient -ljsoncpp
else
	CFLAGS = -I ../include/molnet -O2 -ldl -lstdc++ -lpthread -L ../libs -lmolnet -lmysqlclient -ljsoncpp
endif

OBJDIR = objs
BIN = ../bin
OBJS = $(OBJDIR)/login_server.o \
       $(OBJDIR)/DBOperator.o \
	   $(OBJDIR)/GameFrameManager.o \


TARGET = $(BIN)/login_server
$(TARGET):$(OBJDIR) $(BIN) $(OBJS)
	$(CPP) -o $@ $(OBJDIR)/*.o $(CFLAGS)

$(OBJDIR):
	mkdir -p ./$@

$(BIN):
	mkdir -p ./$@

$(OBJS):$(OBJDIR)/%.o:%.cpp
	$(CPP) -c -g $(CFLAGS) $< -o $@

.PHONY : clearn
clean:
	-$(RM) $(TARGET)
	-$(RM) $(OBJDIR)/*.o -r
