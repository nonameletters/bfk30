CC=gcc
#CFLAGS = -std=gnu99 -O2 -Wall -Wextra -g -I/usr/include/mysql
#LDFLAGS = -L/usr/lib/x86_64-linux-gnu -L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -ldl
CFLAGS = -std=gnu99 -O2 -Wall -Wextra -g
LDFLAGS = -lpthread -lz -lm -ldl

SRCS = $(wildcard *.c) ../fxp.c
OBJS = $(SRCS:%.c=%.o)
EXE = bmc_hid_tool

all: $(EXE)

$(EXE): $(OBJS)
	${CC} ${OBJS} ${LDFLAGS} -o ${EXE}

-include $(subst .c,.d,$(SRCS))

%.d: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -MM -MF $@ -MP -MT $(subst .c,.o,$<) $<

clean:
	rm -f $(EXE)
	rm -f $(OBJS)
	rm -f $(subst .c,.d,$(SRCS))
	rm -f *~

.PHONY: clean all
