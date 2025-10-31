TARGET = main
CXX = g++


IMGUI_SRCS = \
../imgui/imgui.cpp \
../imgui/imgui_draw.cpp \
../imgui/imgui_tables.cpp \
../imgui/imgui_widgets.cpp \
../imgui/imgui_demo.cpp \
../imgui/backends/imgui_impl_glfw.cpp \
../imgui/backends/imgui_impl_opengl3.cpp


INC_DIRS = -I../include -I../glm 

LIB_DIRS = -L/usr/local/lib


LIBS = -lglfw -ldl -lpthread



RM = rm -f

# === Build mode: release or debug ===
MODE ?= release
SRCS = main.cpp ../include/glad/glad.c

ifeq ($(MODE),debug)
	CXXFLAGS += -g -DDEBUG  -Wall -Wextra -std=c++17
	SRCS += $(IMGUI_SRCS)
        INC_DIRS += -I../imgui
else ifeq ($(MODE),release)
	CXXFLAGS += -O2  -Wall -Wextra -std=c++17
else
    $(error Invalid MODE: '$(MODE)'. Use MODE=debug or MODE=release)
endif



all:$(TARGET)

$(TARGET): $(SRCS)
	@echo "Compilazione di $(TARGET)..."
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(INC_DIRS) $(LIB_DIRS) $(LIBS)
	@echo "Compilazione completata con successo. Eseguibile: $(TARGET)"



run: $(TARGET)
	@echo "Esecuzione di $(TARGET)..."
	./$(TARGET)



clean:
	@echo "Pulizia in corso..."
	$(RM) $(TARGET)
	$(RM) *~
	@echo "Pulizia completata."


.PHONY:
	all run clean
