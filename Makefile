# Compiler ####################################################################
STD			:= -std=c18		# -ansi, -std=c99, -std=c11, -std=c18, -std=c2x
CC			:= gcc $(STD)

# Application #################################################################
APPNAME		:= pong
UAPPNAME	:= $(shell echo '$(APPNAME)' | tr '[:lower:]' '[:upper:]')
ifeq ($(_WIN32),)
EXEC		:= $(APPNAME).exe
else
EXEC		:= $(APPNAME)
endif

# Directories #################################################################
SRCDIR		:= src
ASMDIR		:= asm
OBJDIR		:= obj
BINDIR		:= bin
DBGDIR		:= $(BINDIR)/debug
RLSDIR		:= $(BINDIR)/release

# Flags #######################################################################
CFLAGS		:= -O2
WFLAGS		:= -Wall -Wextra -Wpedantic
  GCOV		:= -fprofile-arcs -ftest-coverage
  GPROF		:= -pg
PFLAGS		:= -Og $(GCOV)
DFLAGS		:= -Og -g3 -ggdb3

# Includes ####################################################################
ISDL2		:= `sdl2-config --cflags`

INCLUDES	:= $(ISDL2)

# Libraries ###################################################################
LNCURSES	:= -lncurses 

LSDL2		:= `sdl2-config --static-libs`

  JPEG		:= `pkg-config libjpeg --libs --static`
  TIFF		:= `pkg-config libtiff-4 --libs --static`
LSDL2_IMAGE	:= `pkg-config SDL2_image --libs --static` $(JPEG) $(TIFF)

  FREETYPE	:= `pkg-config freetype2 --libs --static`
  HARFBUZZ	:= `pkg-config harfbuzz --libs --static`
LSDL2_TTF	:= `pkg-config SDL2_ttf --libs --static` $(FREETYPE) $(HARFBUZZ) -lstdc++

  FLAC		:= `pkg-config flac --libs --static`
  MPG123	:= `pkg-config libmpg123 --libs --static`
  OGG		:= `pkg-config ogg --libs --static`
  VORBIS	:= `pkg-config vorbis vorbisenc  vorbisfile --libs --static`
  OPUS		:= `pkg-config opus opusfile opusurl --libs --static`
LSDL2_MIXER	:= `pkg-config SDL2_mixer --libs --static` $(FLAC) $(MPG123) $(OGG) $(VORBIS) $(OPUS)

LSDL2_NET	:= `pkg-config SDL2_net --libs --static` -lws2_32 -liphlpapi


LIBS		:= $(LSDL2)
LIBS		+= -static-libgcc -static

# Sources and objects #########################################################
SRCS		:= $(wildcard $(SRCDIR)/*.c)
ASMS		:= $(patsubst $(SRCDIR)/%, $(ASMDIR)/%, $(SRCS:.c=.s))
OBJS		:= $(patsubst $(SRCDIR)/%, $(OBJDIR)/%, $(SRCS:.c=_r.o))
DOBJS		:= $(patsubst $(SRCDIR)/%, $(OBJDIR)/%, $(SRCS:.c=_d.o))

# Compilation #################################################################
all:		release debug

release:	FLAGS := $(WFLAGS) $(CFLAGS)
release:	$(RLSDIR)/$(EXEC)

debug:		FLAGS := $(WFLAGS) $(DFLAGS) $(PFLAGS)
debug:		LIBS  +=  -lgcov
debug:		CC += -DDEBUG
debug:		$(DBGDIR)/$(EXEC)

run:		release
	@$(RLSDIR)/$(EXEC)

rund:		debug
	@$(DBGDIR)/$(EXEC)

# Release
$(RLSDIR)/$(EXEC):	$(OBJS)
	@echo "[ L ] $@"
	@$(CC) $^ -o $@ -s $(LIBS)

$(OBJDIR)/%_r.o:	$(ASMDIR)/%_r.s
	@echo "[ A ] $@"
	@$(CC) $< -c -o $@

$(ASMDIR)/%_r.s:	$(SRCDIR)/%.c
	@echo "[ C ] $@"
	@$(CC) $< -S -o $@ $(FLAGS)

# Debug
$(DBGDIR)/$(EXEC):	$(DOBJS)
	@echo "[ L ] $@"
	@$(CC) $(GPROF) $^ -o $@ $(LIBS)

$(OBJDIR)/%_d.o:	$(ASMDIR)/%_d.s
	@echo "[ A ] $@"
	@$(CC) $< -c -o $@

$(ASMDIR)/%_d.s:	$(SRCDIR)/%.c
	@echo "[ C ] $@"
	@$(CC) $< -S -o $@ $(FLAGS)

# Files dependancies ##########################################################

$(ASMDIR)/main_r.s:		$(SRCDIR)/main.c $(SRCDIR)/$(APPNAME).h
#$(ASMDIR)/game_r.s:		$(SRCDIR)/game.c $(SRCDIR)/game.h $(SRCDIR)/display.h $(SRCDIR)/event.h
#$(ASMDIR)/display_r.s:	$(SRCDIR)/display.c $(SRCDIR)/display.h
#$(ASMDIR)/event_r.s:	$(SRCDIR)/event.c $(SRCDIR)/event.h

$(ASMDIR)/main_d.s:		$(SRCDIR)/main.c $(SRCDIR)/$(APPNAME).h
#$(ASMDIR)/game_d.s:		$(SRCDIR)/game.c $(SRCDIR)/game.h $(SRCDIR)/display.h $(SRCDIR)/event.h
#$(ASMDIR)/display_d.s:	$(SRCDIR)/display.c $(SRCDIR)/display.h
#$(ASMDIR)/event_d.s:	$(SRCDIR)/event.c $(SRCDIR)/event.h

# #############################################################################
init:
	@mkdir -pv $(SRCDIR)
	@mkdir -pv $(ASMDIR)
	@mkdir -pv $(OBJDIR)
	@mkdir -pv $(BINDIR)
	@mkdir -pv $(DBGDIR)
	@mkdir -pv $(RLSDIR)
ifeq (,$(wildcard $(SRCDIR)/main.c))
	@echo -e "#include \"$(APPNAME).h\"\n\nint main(void) {\n\treturn 0;\n}" > $(SRCDIR)/main.c
else
	@touch $(SRCDIR)/main.c
endif
ifeq (,$(wildcard $(SRCDIR)/$(APPNAME).h))
	@echo -e "#ifndef HAVE_$(UAPPNAME)_H\n#define HAVE_$(UAPPNAME)_H\n\n\n#endif /* HAVE_$(UAPPNAME)_H */" > $(SRCDIR)/$(APPNAME).h
else
	@touch $(SRCDIR)/$(APPNAME).h
endif

# keep the intermediate .s files
.SECONDARY: 

clean:
	@rm -vf bin/release/$(EXEC)
	@rm -vf bin/debug/$(EXEC)
	@rm -vf asm/*.s
	@rm -vf obj/*.o
