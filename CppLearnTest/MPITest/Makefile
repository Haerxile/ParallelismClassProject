# makefile

ROOT := $(shell pwd)

# ownset path
SUBDIR := $(ROOT)
SUBDIR += /home/haerxile/MPI/openmpi-5.0.3/lib

# target
TARGET := helloworld
CXX := mpic++
OUTPUT := $(ROOT)/output

# file paths and depends

INCS := $(foreach DIR,$(SUBDIR),-I$(DIR))
DLLS := $(foreach DIR,$(SUBDIR),$(wildcard $(DIR)/libmpi.so))
SRCS := $(foreach DIR,$(SUBDIR),$(wildcard $(DIR)/*.cpp))
OBJS := $(patsubst $(ROOT)/%.cpp,$(OUTPUT)/%.o,$(SRCS))
DEPS := $(patsubst %.o,%.d,$(OBJS))

# linking

$(TARGET) : $(OBJS)
	@echo Linking...
	$(CXX) $(OBJS) -o $@ $(DLLS)
	@echo Done!

# compiling

$(OUTPUT)/%.o : %.cpp
	@echo Compiling $<...
	@mkdir -p $(dir $@)
	$(CXX) -MMD -MP -c $(INCS) $< -o $@
	@echo Done!

# clean
.PHONY : clean

clean:
	@echo Trying to clean...
	@rm -r $(OUTPUT)
	@echo Completed!

# include depends
-include $(DEPS)
