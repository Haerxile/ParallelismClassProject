# Makefile
SUBDIR := .
SUBDIR += ./func

OUTPUT := ./output

INCS := $(foreach DIR,$(SUBDIR),-I$(DIR))
SRCS := $(foreach DIR,$(SUBDIR),$(wildcard $(DIR)/*.cpp))
OBJS := $(patsubst %.cpp,$(OUTPUT)/%.o,$(SRCS))
DEPS := $(patsubst %.o,%.d,$(OBJS))

main : $(OBJS)
	@echo Linking...
	@g++ $(OBJS) -o main
	@echo Done!
	./main

$(OUTPUT)/%.o : %.cpp
	@echo Compiling...
	@mkdir -p $(dir $@)
	@g++ -MMD -MP -c $(INCS) $< -o $@
	@echo Done!

.PHONY : clean

clean:
	@echo Trying to clean ...
	@rm -r $(OUTPUT)
	@echo Completed!

-include $(DEPS)
