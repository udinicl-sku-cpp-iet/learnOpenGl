CXX = g++
CXXFLAGS = -Wall -Wextra
INC_DIRS = -I../../include
LIB_DIRS = -L/usr/local/lib
LIBS = -lglfw -ldl -lpthread
RM = rm -rf


OBJ_DIR = ./obj


GLAD_SRC = ../../include/glad/glad.c
GLAD_OBJ_DIR = ../../glad/obj
GLAD_OBJ = $(GLAD_OBJ_DIR)/glad.o

IMGUI_SRCS = \
../../imgui/imgui.cpp \
../../imgui/imgui_draw.cpp \
../../imgui/imgui_tables.cpp \
../../imgui/imgui_widgets.cpp \
../../imgui/imgui_demo.cpp \
../../imgui/backends/imgui_impl_glfw.cpp \
../../imgui/backends/imgui_impl_opengl3.cpp
IMGUI_OBJ_DIR = ../../imgui/obj
IMGUI_OBJS = $(patsubst %.cpp,$(IMGUI_OBJ_DIR)/%.o,$(notdir $(IMGUI_SRCS)))
IMGUI_SRC_DIRS = $(sort $(dir $(IMGUI_SRCS)))
vpath %.cpp $(IMGUI_SRC_DIRS)


MAIN_SRC = main.cpp


MODE ?= standard
OBJS = $(GLAD_OBJ)
OBJS += $(IMGUI_OBJS) 
INC_DIRS += -I../../imgui -I../../imgui/backends	

ifeq ($(MODE),standard)
	CXXFLAGS += -DSTANDARD
	TARGET = main
	MAIN_OBJ_NAME = main.o
else ifeq ($(MODE),ex1)
	CXXFLAGS += -DEX1
	TARGET = main_ex1
	MAIN_OBJ_NAME = main_ex1.o	
else ifeq ($(MODE),ex2)
	CXXFLAGS += -DEX2
	TARGET = main_ex2
	MAIN_OBJ_NAME = main_ex2.o
else
	$(error Invalid MODE: '$(MODE)'. Use MODE=debug or MODE=release)
endif


MAIN_OBJ = $(OBJ_DIR)/$(MAIN_OBJ_NAME)
OBJS += $(MAIN_OBJ)




all: $(TARGET)


$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LIB_DIRS) $(LIBS)
	@echo "Build successful. Executable: $(TARGET)"


$(MAIN_OBJ): $(MAIN_SRC)
	@echo "Compiling $(MAIN_SRC) -> $(MAIN_OBJ) (Mode: $(MODE))..."
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(MAIN_SRC) -o $(MAIN_OBJ) $(INC_DIRS)


$(GLAD_OBJ): $(GLAD_SRC)
	@echo "Compiling $(GLAD_SRC) -> $(GLAD_OBJ)..."
	@mkdir -p $(GLAD_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(GLAD_SRC) -o $(GLAD_OBJ) $(INC_DIRS)


$(IMGUI_OBJ_DIR)/%.o: %.cpp
	@echo "Compiling ImGui source $<..."
	@mkdir -p $(IMGUI_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INC_DIRS)

run: $(TARGET)
	@echo "Running $(TARGET)..."
	./$(TARGET)

clean:
	@echo "Cleaning up..."
	$(RM) main main_ex1 main_ex2 main_ex3
	$(RM) $(OBJ_DIR)
	$(RM) $(GLAD_OBJ_DIR)
	$(RM) $(IMGUI_OBJ_DIR)
	$(RM) imgui.ini
	$(RM) *~
	$(RM) ./glsl_shader/*~
	@echo "Cleanup complete."

.PHONY: all run clean
