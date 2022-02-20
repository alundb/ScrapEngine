VULKAN_SDK_PATH = ../../external/VulkanSDK
GLFW_PATH   = ../../external/glfw
GLM_PATH    = ../../external/glm
VKMEM_PATH  = ../../external/VulkanMemoryAllocator
STB_PATH    = ../../external/stb
IMGUI_DIR   = ../../external/imgui
ENKITS_PATH = ../../external/enkits
REACT_PATH  = ../../external/reactphysics
DR_PATH     = ../../external/dr_libs
ASSIMP_DIR  = ../../external/assimp
OPENAL_PATH = ../../external/openal-soft

GAME_SOURCE_DIR = ../SimpleGame/GameObjects
SOURCE_DIR = Engine
BIN_DIR = bin
OBJ_DIR = obj

# CXX = clang++
CXX = g++

# Game Source Dirs
GAME_SOURCE_DIRS = $(shell find $(GAME_SOURCE_DIR) -type d)

# Engine Source Dirs
SOURCE_DIRS = $(shell find $(SOURCE_DIR) -type d) $(GAME_SOURCE_DIRS) $(IMGUI_DIR)

# simple debug target
print:
	$(info $$var is [${GAME_SOURCE_DIRS}])


# Create a list of the *.cpp sources in SOURCE_DIRS
SOURCES = $(foreach dir,$(SOURCE_DIRS),$(wildcard $(dir)/*.cpp))

OBJS = $(addprefix $(OBJ_DIR)/,$(SOURCES:.cpp=.o))
DEPS = $(addprefix $(OBJ_DIR)/,$(SOURCES:.cpp=.d))

LDFLAGS = -L$(VULKAN_SDK_PATH)/lib -lvulkan $(GLFW_PATH)/src/libglfw3.a $(ENKITS_PATH)/libenkiTS.a \
	$(ASSIMP_DIR)/lib/libassimp.so $(REACT_PATH)/build/lib/libreactphysics3d.a \
	$(OPENAL_PATH)/libopenal.so \
	-lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -ldl

CFLAGS = -std=c++17 -fPIC -g -Wall -I$(VULKAN_SDK_PATH)/Vulkan-Headers/include/vulkan/ -I$(GLFW_PATH)/include -I$(IMGUI_DIR) -I "." -I $(VKMEM_PATH)/src -I $(GLM_PATH) -I $(STB_PATH) -I $(ENKITS_PATH)/src -I $(REACT_PATH)/src -I $(DR_PATH)

-include $(DEPS)

simplegame: $(BIN_DIR)/../SimpleGame/Game
	./$^

$(OBJ_DIR)/%.o : %.cpp
	@echo Howdy $$@
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -MMD -MP -c $< -o $@

$(BIN_DIR)/% : %.cpp $(OBJS)
	@echo bin Howdy $$@
	@mkdir -p $(@D)
	$(CXX)  $(CFLAGS) $^ -o $@ $(LDFLAGS)

.PHONY: clean simplegame print

# Avoid removal of intermediate object files.
# This gets rid of having to compile twice after make clean
.PRECIOUS: $(OBJ_DIR)/%.o

clean:
	rm -rf $(BIN_DIR)/*
	rm -rf $(OBJ_DIR)/*
