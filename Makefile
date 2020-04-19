
EXE = climbing_wall_gui
SOURCES = main.cpp
SOURCES += ./dear_imgui/imgui_impl_sdl.cpp ./dear_imgui/imgui_impl_opengl3.cpp
SOURCES += ./dear_imgui/imgui.cpp ./dear_imgui/imgui_demo.cpp ./dear_imgui/imgui_draw.cpp ./dear_imgui/imgui_widgets.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)

CXXFLAGS = -I./dear_imgui 
CFLAGS += -g -Wall -Wformat 

CXXFLAGS = -I./dear_imgui 
CXXFLAGS += -g -Wall -Wformat 
CXXFLAGS += -std=c++17

LIBS =

##---------------------------------------------------------------------
## OPENGL LOADER
##---------------------------------------------------------------------

## Using OpenGL loader: gl3w [default]
SOURCES += ./dear_imgui/libs/gl3w/GL/gl3w.c
CXXFLAGS += -I./dear_imgui/libs/gl3w -DIMGUI_IMPL_OPENGL_LOADER_GL3W
CFLAGS += -I./dear_imgui/libs/gl3w -DIMGUI_IMPL_OPENGL_LOADER_GL3W


##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += -lGL -ldl `sdl2-config --libs`
	CXXFLAGS += `sdl2-config --cflags`
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo `sdl2-config --libs`
	LIBS += -L/usr/local/lib -L/opt/local/lib

	CXXFLAGS += `sdl2-config --cflags`
	CXXFLAGS += -I/usr/local/include -I/opt/local/include
endif

ifeq ($(findstring MINGW,$(UNAME_S)),MINGW)
   ECHO_MESSAGE = "MinGW"
   LIBS += -lgdi32 -lopengl32 -limm32 `pkg-config --static --libs sdl2`

   CXXFLAGS += `pkg-config --cflags sdl2`
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:./dear_imgui/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:./dear_imgui/libs/gl3w/GL/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o:./dear_imgui/libs/glad/src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)
