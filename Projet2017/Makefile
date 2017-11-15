
LIB_MODE=

PROGRAM	:= game
MAPUTIL := maputil

# Must be the first rule
.PHONY: default
default: $(PROGRAM) $(MAPUTIL)

SOURCES := $(wildcard src/*.c)
OBJECTS := $(SOURCES:src/%.c=obj/%.o)
DEPENDS := $(SOURCES:src/%.c=deps/%.d)
MDEPENDS := util/src/maputil.c

MAKEFILES := Makefile

CUSTOM_OBJ := obj/mapio.o obj/tempo.o
LIB	:= lib/libgame.a

CFLAGS := -O3 -g -Wall -Wno-unused-function
CFLAGS += -DPADAWAN
CFLAGS += -I./include

CFLAGS += $(shell pkg-config SDL2_image SDL2_mixer --cflags)
LDLIBS := $(shell pkg-config SDL2_image SDL2_mixer --libs)

LDLIBS += -lm -lpthread

$(OBJECTS): $(MAKEFILES)

ifndef LIB_MODE
$(LIB): $(filter-out $(CUSTOM_OBJ),$(OBJECTS))
	ar -crs $(LIB) $^
endif

$(PROGRAM): $(CUSTOM_OBJ) $(LIB)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(MAPUTIL): 	
	$(CC) $(MDEPENDS) -o maputil

$(OBJECTS): obj/%.o: src/%.c
	$(CC) -o $@ $(CFLAGS) -c $<

.PHONY: depend
depend: $(DEPENDS)

$(DEPENDS): $(MAKEFILES)

$(DEPENDS): deps/%.d: src/%.c
	$(CC) $(CFLAGS) -MM $< | \
		sed -e 's|\(.*\)\.o:|deps/\1.d obj/\1.o:|g' > $@

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif

.PHONY: clean
clean: 
	rm -f maputil game obj/*.o deps/*.d 
