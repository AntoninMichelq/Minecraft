CC     = gcc
SRC    = main.c
RLDIR  = raylib/src
# ne garde que les fichiers qui existent dans ta version de raylib
RLSRC  = $(notdir $(wildcard $(addprefix $(RLDIR)/,rcore.c rshapes.c rtextures.c rtext.c rmodels.c raudio.c utils.c rglfw.c)))

ifeq ($(OS),Windows_NT)
    PLAT    = windows
    TARGET  = jeu.exe
    LIBS    = -lopengl32 -lgdi32 -lwinmm
    RLEXTRA =
else
    PLAT    = linux
    TARGET  = jeu
    LIBS    = -lGL -lm -lpthread -ldl -lrt -lX11
    RLEXTRA = -D_GNU_SOURCE
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
	ar rcs $@ $^

$(BUILD)/raylib/%.o: $(RLDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(RLFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

cleanall:
	rm -rf build jeu jeu.exe

.PHONY: all run clean cleanall