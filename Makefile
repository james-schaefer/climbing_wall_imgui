EXE = climbing_wall_gui
SOURCES = main.cpp
SOURCES += ./dear_imgui/imgui_impl_sdl.cpp ./dear_imgui/imgui_impl_opengl3.cpp
SOURCES += ./libshm_vars/Shm_vars.cpp
SOURCES += ./dear_imgui/imgui.cpp ./dear_imgui/imgui_demo.cpp \
           ./dear_imgui/imgui_draw.cpp ./dear_imgui/imgui_widgets.cpp
SOURCES += ./libshm_vars/Shm_vars.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)

LIBSHMDIR = ./libshm_vars/
LIBSHM = $(LIBSHMDIR)libshm_vars.a

CFLAGS = -I./dear_imgui 
CFLAGS += -g -Wall -Wformat 

CXXFLAGS = -I./dear_imgui -I./libshm_vars
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

CXXFLAGS += `sdl2-config --cflags`
LIBS += -lGL -ldl `sdl2-config --libs`
LDFLAGS = -L./libshm_vars -lshm_vars -lpthread -lrt 


##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------
all: $(EXE)
	@echo Build complete for $(EXE)

$(LIBSHM): $(LIBSHMDIR)Shm.h $(LIBSHMDIR)Shm_vars.h  \
           $(LIBSHMDIR)Shm_vars.cpp $(LIBSHMDIR)Command.cpp
	
	cd $(LIBSHMDIR); \
	g++ -c Command.cpp; \
	g++ -c Shm_vars.cpp; \
	ar -cq libshm_vars.a Shm_vars.o Command.o; \
	rm -f  Shm_vars.o Command.o 

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:./dear_imgui/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
%.o:./libshm_vars/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:./dear_imgui/libs/gl3w/GL/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o:./dear_imgui/libs/glad/src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<


$(EXE): $(OBJS) $(LIBSHM)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS) $(LDFLAGS)

clean:
	rm -f $(EXE) $(OBJS)
