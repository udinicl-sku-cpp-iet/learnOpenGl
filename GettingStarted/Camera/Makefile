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



MAIN_SRC = main.cpp


MODE ?= standard
OBJS = $(GLAD_OBJ) 

ifeq ($(MODE),standard)
	CXXFLAGS += -DSTANDARD
	TARGET = main
	MAIN_OBJ_NAME = main.o
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


run: $(TARGET)
	@echo "Running $(TARGET)..."
	./$(TARGET)

clean:
	@echo "Cleaning up..."
	$(RM) main 
	$(RM) $(OBJ_DIR)
	$(RM) $(GLAD_OBJ_DIR)
	$(RM) *~
	@echo "Cleanup complete."

.PHONY: all run clean
