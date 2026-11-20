CC     = gcc
SRC    = main.c
RLDIR  = raylib/src
# ne garde que les fichiers qui existent dans ta version de raylib
RLSRC  = $(notdir $(wildcard $(addprefix $(RLDIR)/,rcore.c rshapes.c rtextures.c rtext.c rmodels.c raudio.c utils.c rglfw.c)))
ifeq ($(RLSRC),)
    $(error Aucun fichier source raylib trouve dans $(RLDIR) : le dossier raylib est vide ou mal place)
endif
# gcc-ar vient avec gcc : evite d'utiliser un "ar" d'une autre installation
AR     = gcc-ar

ifeq ($(OS),Windows_NT)
    # force cmd.exe : marche pareil avec mingw32-make seul ou avec w64devkit
    SHELL       := cmd.exe
    .SHELLFLAGS := /c
    PLAT    = windows
    TARGET  = jeu.exe
    LIBS    = -lopengl32 -lgdi32 -lwinmm
    RLEXTRA =
    MKDIR   = if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
    RMFILE  = if exist "$(1)" del /Q /F "$(1)"
    RMDIR   = if exist "$(1)" rmdir /S /Q "$(1)"
    RUN     = .\$(TARGET)
else
    PLAT    = linux
    TARGET  = jeu
    LIBS    = -lGL -lm -lpthread -ldl -lrt -lX11
    RLEXTRA = -D_GNU_SOURCE -D_GLFW_X11
    MKDIR   = mkdir -p $(1)
    RMFILE  = rm -f $(1)
    RMDIR   = rm -rf $(1)
    RUN     = ./$(TARGET)
endif

BUILD   = build/$(PLAT)
RLOBJ   = $(addprefix $(BUILD)/raylib/,$(RLSRC:.c=.o))
RLFLAGS = -O2 -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 \
          -I$(RLDIR) -I$(RLDIR)/external/glfw/include $(RLEXTRA)
CFLAGS  = -Wall -std=c99 -I$(RLDIR)

all: $(TARGET)

$(TARGET): $(SRC) $(BUILD)/libraylib.a
	$(CC) $(SRC) -o $@ $(CFLAGS) -L$(BUILD) -lraylib $(LIBS)

$(BUILD)/libraylib.a: $(RLOBJ)
	$(AR) rcs $@ $^
	gcc-ranlib $@

$(BUILD)/raylib/%.o: $(RLDIR)/%.c
	@$(call MKDIR,$(BUILD)/raylib)
	$(CC) -c $< -o $@ $(RLFLAGS)

run: $(TARGET)
	$(RUN)

clean:
	$(call RMFILE,$(TARGET))

cleanall:
	$(call RMDIR,build)
	$(call RMFILE,jeu)
	$(call RMFILE,jeu.exe)

.PHONY: all run clean cleanall