# makefile
ROOT := /home/haerxile/ColdField/ParallelismClassProject

# ownset path
SUBDIR := $(ROOT)/GenRandomMatrix
SUBDIR += $(ROOT)/MatrixH
SUBDIR += $(ROOT)/InputH
SUBDIR += /opt/OpenBLAS/lib
SUBDIR += /usr/local/lib

# target
TARGET := main
CXX := g++
OUTPUT := $(ROOT)/GenRandomMatrix/output

# file paths and depends
INCS := $(foreach DIR,$(SUBDIR),-I$(DIR))
SICS := $(foreach DIR,$(SUBDIR),$(wildcard $(DIR)/libopenblas.a))
SICS += $(foreach DIR,$(SUBDIR),$(wildcard $(DIR)/liblapacke.a))
SRCS := $(foreach DIR,$(SUBDIR),$(wildcard $(DIR)/*.cpp))
OBJS := $(patsubst $(ROOT)/%.cpp,$(OUTPUT)/%.o,$(SRCS))
DEPS := $(patsubst %.o,%.d,$(OBJS))

# linking
$(TARGET) : $(OBJS)
	@echo Linking...
	@$(CXX) $(OBJS) -o $@ $(SICS) -lgfortran
	@echo Done!

# compiling
$(OUTPUT)/%.o : $(ROOT)/%.cpp
	@echo Compiling $<...
	@mkdir -p $(dir $@)
	@$(CXX) -MMD -MP -c $(INCS) $< -o $@
	@echo Done!

# clean
.PHONY : clean

clean:
	@echo Trying to clean...
	@rm -r $(OUTPUT)
	@echo Completed!

# include depends
-include $(DEPS)
