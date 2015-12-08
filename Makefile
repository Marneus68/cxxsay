# Takes the name of the current directory as a the executable and "pr" name
TARGET  := $(shell basename $$PWD | tr '[:upper:]' '[:lower:]')
# Warning levels
WARN    :=
# Optimisation
OFLAGS  := -std=c++11
# Link Options
LDFLAGS := 
# Link Libraries
LDLIBS 	:= 

# Source directory
SRCDIR	:= src
# Include directory
INCDIR	:= include
# Object directory
OBJDIR	:= obj

CFLAGS	:= -I $(INCDIR)

# setting the vpath (additionnal search path for make)
VPATH	:= $(INCDIR) $(SRCDIR) $(OBJDIR)

CPP_SRCS    = $(wildcard src/*.cpp)
OBJ_FILES   = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(CPP_SRCS))

# Colors
black	= `tput setaf 0`
red		= `tput setaf 1`
green	= `tput setaf 2`
yellow	= `tput setaf 3`
blue	= `tput setaf 4`
magenta = `tput setaf 5`
cyan	= `tput setaf 2`
white	= `tput setaf 7`
reset	= `tput sgr0`

all: cxxsay cxxthink

cxxsay: obj/funs.o obj/cxxsay.o
	@echo "$(blue)Linking $(yellow)"$@"$(blue)...$(reset)"
	$(CXX) $(WARN) $(OFLAGS) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS) 
	@echo "$(green)Done.$(reset)"

cxxthink: obj/funs.o obj/cxxthink.o
	@echo "$(blue)Linking $(yellow)"$@"$(blue)...$(reset)"
	$(CXX) $(WARN) $(OFLAGS) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS) 
	@echo "$(green)Done.$(reset)"

obj/%.o: %.cpp $(OBJDIR)
	@echo "$(blue)Compiling $(yellow)"$<"$(blue)...$(reset)"
	$(CXX) -c $(WARN) $(OFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@

$(OBJDIR):
	@echo "$(blue)Creating object directory..."$(reset)
	mkdir -p $(OBJDIR)

clean: 
	@echo "$(blue)Cleaning executables...$(reset)"
	rm -rf $(OBJDIR)/*.o

mrproper: clean
	@echo "$(blue)Cleaning executables...$(reset)"
	rm -rf cxxsay
	rm -rf cxxthink

install:
	@echo "$(blue)Installing...$(reset)"

doc:
	@echo "Building documentation..."

