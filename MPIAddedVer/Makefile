# makefile

ROOT := /home/haerxile/ColdField/ParallelismClassProject

USE_MPI := yes

# ownset path
SUBDIR += $(ROOT)/InputH
SUBDIR += $(ROOT)/MatrixH
SUBDIR += $(ROOT)/TimerH
SUBDIR += $(ROOT)/TabulateH
SUBDIR += $(ROOT)/MPIAddedVer
SUBDIR += /opt/OpenBLAS/lib
SUBDIR += /usr/local/lib
ifeq ($(USE_MPI),yes)
	SUBDIR += /home/haerxile/MPI/openmpi-5.0.3/lib
endif

# target
TARGET := main

ifeq ($(USE_MPI),yes)
	CXX := mpic++
	OPTION := -O3 -D__MPI__
else
	CXX := g++
	OPTION := -O3
endif

OUTPUT := $(ROOT)/MPIAddedVer/output

# file paths and depends

INCS := $(foreach DIR,$(SUBDIR),-I$(DIR))
SICS := $(foreach DIR,$(SUBDIR),$(wildcard $(DIR)/libopenblas.a))
SICS += $(foreach DIR,$(SUBDIR),$(wildcard $(DIR)/liblapacke.a))
ifeq ($(USE_MPI),yes)
	DLLS := $(foreach DIR,$(SUBDIR),$(wildcard $(DIR)/libmpi.so))
endif
SRCS := $(foreach DIR,$(SUBDIR),$(wildcard $(DIR)/*.cpp))
OBJS := $(patsubst $(ROOT)/%.cpp,$(OUTPUT)/%.o,$(SRCS))
DEPS := $(patsubst %.o,%.d,$(OBJS))

# linking

$(TARGET) : $(OBJS)
	@echo Linking...
	@echo SUBDIR: $(SUBDIR)
	@$(CXX) $(OBJS) -o $@ $(SICS) $(DLLS) -lgfortran
	@echo Done!

# compiling

$(OUTPUT)/%.o : $(ROOT)/%.cpp
	@echo Compiling $<...
	@mkdir -p $(dir $@)
	@$(CXX) -MMD -MP -c $(OPTION) $(INCS) $< -o $@
	@echo Done!

# clean
.PHONY : clean

clean:
	@echo Trying to clean...
	@rm -r $(OUTPUT)
	@echo Completed!

# include depends
-include $(DEPS)
