CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude -fvisibility=hidden
SHARED_CXXFLAGS := $(CXXFLAGS) -fPIC
LDFLAGS := -shared -Wl,--export-dynamic
LDLIBS := -lm -ldl

TARGET := rgr
BIN_DIR := bin
OBJ_DIR := obj
LIB_DIR := lib
SRC_DIR := src

GROSFELD_SRCS := $(wildcard $(SRC_DIR)/grosfeld/*.cpp)
HILL_SRCS := $(wildcard $(SRC_DIR)/hill/*.cpp)
POLYBIUS_SRCS := $(wildcard $(SRC_DIR)/polybius/*.cpp)
UTILS_SRC := $(SRC_DIR)/utils.cpp
MAIN_SRC := $(SRC_DIR)/main.cpp

GROSFELD_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(GROSFELD_SRCS))
HILL_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(HILL_SRCS))
POLYBIUS_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(POLYBIUS_SRCS))
UTILS_OBJ := $(OBJ_DIR)/utils.o
MAIN_OBJ := $(OBJ_DIR)/main.o

LIBS := \
    $(LIB_DIR)/libgrosfeld.so \
    $(LIB_DIR)/libhill.so \
    $(LIB_DIR)/libpolybius.so

.PHONY: all clean install uninstall libs test

all: create_dirs $(BIN_DIR)/$(TARGET)

libs: create_dirs $(LIBS)

create_dirs:
	@mkdir -p $(BIN_DIR) $(LIB_DIR)
	@mkdir -p $(OBJ_DIR)/{grosfeld,hill,polybius}

$(BIN_DIR)/$(TARGET): $(MAIN_OBJ) $(UTILS_OBJ) $(LIBS)
	$(CXX) $(CXXFLAGS) -L$(LIB_DIR) -Wl,-rpath,'\$$ORIGIN/..' $^ -o $@ -lgrosfeld -lhill -lpolybius $(LDLIBS)

# Правила для библиотек
$(LIB_DIR)/libgrosfeld.so: $(GROSFELD_OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(LIB_DIR)/libhill.so: $(HILL_OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(LIB_DIR)/libpolybius.so: $(POLYBIUS_OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

# Правила компиляции
$(OBJ_DIR)/grosfeld/%.o: $(SRC_DIR)/grosfeld/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(SHARED_CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/hill/%.o: $(SRC_DIR)/hill/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(SHARED_CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/polybius/%.o: $(SRC_DIR)/polybius/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(SHARED_CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)

install: $(BIN_DIR)/$(TARGET) $(LIBS)
	@echo "Installing $(TARGET) to /usr/local/bin"
	@sudo install -m 755 $(BIN_DIR)/$(TARGET) /usr/local/bin/$(TARGET)
	@echo "Installing libraries to /usr/local/lib"
	@sudo install -m 644 $(LIBS) /usr/local/lib/
	@sudo ldconfig
	@echo "Installation complete"

uninstall:
	@echo "Removing $(TARGET) and libraries"
	@sudo rm -f /usr/local/bin/$(TARGET)
	@sudo rm -f /usr/local/lib/libgrosfeld.so /usr/local/lib/libhill.so /usr/local/lib/libpolybius.so
	@sudo ldconfig
	@echo "Uninstallation complete"

test: all
	@echo "Testing program..."
	@cd $(BIN_DIR) && ./$(TARGET)