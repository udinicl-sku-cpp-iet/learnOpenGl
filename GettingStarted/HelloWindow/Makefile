CXX = g++
CXXFLAGS = -Wall -Wextra
INC_DIRS = -I../../include
LIB_DIRS = -L/usr/local/lib
LIBS = -lglfw -ldl -lpthread
TARGET = main
RM = rm -rf 


OBJ_DIR = ./obj

MAIN_SRC = main.cpp
GLAD_SRC = ../../include/glad/glad.c


MAIN_OBJ = $(OBJ_DIR)/main.o
GLAD_OBJ = $(OBJ_DIR)/glad.o


OBJS = $(MAIN_OBJ) $(GLAD_OBJ)


all: $(TARGET)


$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LIB_DIRS) $(LIBS)
	@echo "Build successful. Executable: $(TARGET)"


$(MAIN_OBJ): $(MAIN_SRC)
	@echo "Compiling $<..."
	@mkdir -p $(OBJ_DIR) # Assicura che la directory esista
	$(CXX) $(CXXFLAGS) -c $(MAIN_SRC) -o $(MAIN_OBJ) $(INC_DIRS)


$(GLAD_OBJ): $(GLAD_SRC)
	@echo "Compiling $<..."
	@mkdir -p $(OBJ_DIR) # Assicura che la directory esista
	$(CXX) $(CXXFLAGS) -c $(GLAD_SRC) -o $(GLAD_OBJ) $(INC_DIRS)


run: $(TARGET)
	@echo "Running $(TARGET)..."
	./$(TARGET)

clean:
	@echo "Cleaning up..."
	$(RM) $(TARGET)
	$(RM) $(OBJ_DIR)
	$(RM) *~
	@echo "Cleanup complete."

.PHONY: all run clean
