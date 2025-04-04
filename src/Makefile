cc = gcc
CCFLAGS := -g -Wall
LIBS := 

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    CCFLAGS += -DLINUX_BUILD
		LIBS += ../libs/* -lm -lglfw 
endif
ifeq ($(UNAME_S),Darwin)
    CCFLAGS += -D OSX -framework IOKit -framework Cocoa -framework OpenGL $(pkg-config --libs --cflags raylib)
endif
UNAME_P := $(shell uname -p)
ifeq ($(UNAME_P),x86_64)
    CCFLAGS += -D AMD64
endif
ifneq ($(filter %86,$(UNAME_P)),)
    CCFLAGS += -D IA32
endif
ifneq ($(filter arm%,$(UNAME_P)),)
    CCFLAGS += -D ARM
endif

all:
	@echo "$(LIBS)"
	$(cc) $(CCFLAGS) main.c life.c $(LIBS) -o ../bin/life

clean:
	rm -f *.o
