# makefile

ROOT := /home/haerxile/ColdField/ParallelismClassProject/InputH

USE_MPI := no

# ownset path
SUBDIR := $(ROOT)
SUBDIR += $(ROOT)/TestFile

# target
TARGET := main

ifeq ($(USE_MPI),yes)
	CXX := mpic++
	OPTION := -O3 -D__MPI__
else
	CXX := g++
	OPTION := -O3
endif

OUTPUT := $(ROOT)/TestFile/output

# file paths and depends

INCS := $(foreach DIR,$(SUBDIR),-I$(DIR))
SRCS := $(foreach DIR,$(SUBDIR),$(wildcard $(DIR)/*.cpp))
OBJS := $(patsubst $(ROOT)/%.cpp,$(OUTPUT)/%.o,$(SRCS))
DEPS := $(patsubst %.o,%.d,$(OBJS))

# linking

$(TARGET) : $(OBJS)
	@echo Linking...
	$(CXX) $(OBJS) -o $@
	@echo Done!

# compiling

$(OUTPUT)/%.o : $(ROOT)/%.cpp
	@echo Compiling $<...
	@mkdir -p $(dir $@)
	$(CXX) -MMD -MP -c $(OPTION) $(INCS) $< -o $@
	@echo Done!

# clean
.PHONY : clean

clean:
	@echo Trying to clean...
	@rm -r $(OUTPUT)
	@echo Completed!

# include depends
-include $(DEPS)
