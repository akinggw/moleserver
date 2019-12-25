CC = gcc
CPP = g++
RM = rm -f

ifeq ($(debug),1)
	CFLAGS = -I ../../include/molnet -O0 -shared -g -Wall -fexceptions -fPIC -DLINUX -lpthread
else
	CFLAGS = -I ../../include/molnet -O2 -shared -fexceptions -fPIC -DLINUX -lpthread
endif

OBJDIR = objs
BIN = ../../libs
OBJS = $(OBJDIR)/MahJongAlg.o \
       $(OBJDIR)/MahJongLogic.o \

TARGET = $(BIN)/libmajiang.a
$(TARGET):$(OBJDIR) $(BIN) $(OBJS)
#	$(CPP) -o $@ $(OBJDIR)/*.o $(CFLAGS)
	ar -cr $(TARGET) $(OBJDIR)/*.o

$(OBJDIR):
	mkdir -p ./$@

$(BIN):
	mkdir -p ./$@

$(OBJS):$(OBJDIR)/%.o:$(SRCS)%.cpp
	$(CPP) -c $(CFLAGS) $< -o $@

.PHONY : clearn
clean:
	-$(RM) $(TARGET)
	-$(RM) $(OBJDIR)/*.o -r
