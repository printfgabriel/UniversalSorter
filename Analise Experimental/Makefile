# Configurações do compilador
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Iinclude -lm
DEFINES = -DKEYSZ=$(KEYSZ) -DPLSZ=$(PLSZ) -DVETSZ=$(VETSZ)

# Estrutura de diretórios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = include

# Alvo principal
TARGET = $(BIN_DIR)/tp1.out

# Encontra TODOS os arquivos fonte e headers
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
HEADERS = $(wildcard $(INC_DIR)/*.hpp) $(wildcard $(INC_DIR)/*.h)

# Valores padrão
KEYSZ ?= 4
PLSZ ?= 20
VETSZ ?= 20

# Regra principal
all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(DEFINES) -o $@ $(OBJS)

# Regra para objetos (dependem dos headers)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(DEFINES) -c $< -o $@

# Regras auxiliares
run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean run