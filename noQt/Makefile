# Select compiler.
CXX = g++

# Set flags
CXXFLAGS := -std=c++11 -std=gnu++11

# Get current directory.
SELF_DIR = $(dir $(lastword $(MAKEFILE_LIST)))

# Set output name.
BINNAME = main.exe

# Set output directory.
BINDIR = bin/

# Specify files to compile.
SRCDIR = $(wildcard src/*.cpp)

# Set lib directory.
LIBDIR = -L"$(SELF_DIR)lib"

# Set include directory
INCDIR = -I"$(SELF_DIR)include"

# Specify libraries.
LIBRARIES = -lglfw3 -lglew32 -lOpenGL32 -lGdi32 -lglu32

compile: $(SRCDIR)
	$(CXX) $(CXXFLAGS) $(SRCDIR) -o $(BINDIR)$(BINNAME) $(LIBDIR) $(INCDIR) $(LIBRARIES)