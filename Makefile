CXX := g++
SOURCES := main.cpp
SOURCES += Texture.cpp MainWindow.cpp PendulumProvider.cpp
SOURCES += Rectangle.cpp Menu.cpp OptionsMenu.cpp Text.cpp Icon.cpp
SOURCES += Timer.cpp FpsCounter.cpp Playback.cpp Navigator.cpp
SOURCES += Window.cpp Renderer.cpp Hotkey.cpp
SOURCES += Clickable.cpp DragHandler.cpp Slider.cpp Grid.cpp
SOURCE_DIR := pendui

# Linker flags
LDFLAGS := -lSDL2 -lSDL2_image -lSDL2_ttf

# C++ flags. Add -g for debugging
CXXFLAGS := -std=c++17 -Wall -Werror -pedantic -g

# Where to build to
OBJDIR := build

# Main executable
MAIN := nlinkpendulum

# Adds a search path for where make looks for source files
vpath %.cpp $(SOURCE_DIR)

# Makes a <file.ext>.o file name in the build directory for every source file.
OBJS := $(SOURCES:%=$(OBJDIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Header files are located next to the source files in the source directory
# If we need recursion later, can do: $(shell find $(SOURCE_DIR) -type d)
INC_DIRS := $(SOURCE_DIR)

# The flag for the compiler is in the form -I<module>
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Preprocessor stuff.
# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -MMD -MP

# Link-er up.
$(MAIN): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C++ source
$(OBJDIR)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Clean build directory
.PHONY: clean
clean:
	rm -rf $(OBJDIR)
	rm -f $(MAIN)

# Run executable (building if necessary)
.PHONY: run
run: $(MAIN)
	./$(MAIN)

# Format all .h and .cpp files.
# To create format file:
# clang-format -style=Mozilla -dump-config > .clang-format
.PHONY: format
format:
	clang-format -i $(SOURCE_DIR)/*.cpp $(SOURCE_DIR)/*.h

# Check the format on all .h and .cpp files and display errors.
.PHONY: check-format
check-format:
	clang-format -i -Werror --dry-run $(SOURCE_DIR)/*.cpp $(SOURCE_DIR)/*.h

# Check for memory leaks
.PHONY: valgrind
valgrind: $(MAIN)
	valgrind --leak-check=yes ./$(MAIN)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, or after a clean, all the .d files will be missing. We
# don't want to see these errors.
-include $(DEPS)
