# makefile
ROOT := /home/haerxile/ColdField/ParallelismClassProject
# ownset path
SUBDIR := $(ROOT)/MeVsBLAS
SUBDIR += $(ROOT)/InputH
SUBDIR += $(ROOT)/MatrixH
SUBDIR += $(ROOT)/TimerH
SUBDIR += $(ROOT)/TabulateH
SUBDIR += /opt/OpenBLAS/lib

# target
TARGET := main
CXX := g++
OUTPUT := $(ROOT)/MeVsBLAS/output

# file paths and depends

INCS := $(foreach DIR,$(SUBDIR),-I$(DIR))
SICS := $(foreach DIR,$(SUBDIR),$(wildcard $(DIR)/libopenblas.a))
SRCS := $(foreach DIR,$(SUBDIR),$(wildcard $(DIR)/*.cpp))
OBJS := $(patsubst $(ROOT)/%.cpp,$(OUTPUT)/%.o,$(SRCS))
DEPS := $(patsubst %.o,%.d,$(OBJS))

# linking

$(TARGET) : $(OBJS)
	@echo Linking...
	$(CXX) $(OBJS) -o $@ $(SICS) -lgfortran
	@echo Done!

# compiling

$(OUTPUT)/%.o : $(ROOT)/%.cpp
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
