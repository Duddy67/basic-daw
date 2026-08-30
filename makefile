# Compiler
CXX = g++

# Executable
EXE = basicDAW

# Object directory
OBJDIR = obj

# Dependency directory
DEPDIR = $(OBJDIR)/.deps

# Source files
LOCAL_SRC = \
	main.cpp \
	application/menu.cpp \
	application/application.cpp \
	application/callbacks.cpp \
	core/transport.cpp \
	core/engine.cpp \
	core/tempo_map.cpp \
	core/time_converter.cpp \
	dialogs/dialog.cpp \
	dialogs/new.cpp \
	application/menu/edit.cpp \
	project/model.cpp \
	project/view.cpp \
	application/menu/file.cpp \
	widgets/track.cpp \
	views/track_list.cpp \
	application/menu/track.cpp \
	application/menu/settings.cpp \
	audio/processor.cpp \
	audio/track.cpp \
	views/channel_strip.cpp \
	views/timeline.cpp \
	dialogs/add_track.cpp \
	project/controller.cpp \
	widgets/strip.cpp \
	widgets/transport_bar.cpp \
	midi/scheduler.cpp \
	midi/track.cpp \
	config/config.cpp  \
        dialogs/audio_settings.cpp \
        dialogs/midi_settings.cpp

#EXT_SRC = \
          ../libraries/RtMidi.cpp

# Object files
LOCAL_OBJ = $(addprefix $(OBJDIR)/,$(LOCAL_SRC:.cpp=.o))
EXT_OBJ = $(addprefix $(OBJDIR)/libraries/,$(notdir $(EXT_SRC:.cpp=.o)))

OBJS = $(LOCAL_OBJ) $(EXT_OBJ)

# Dependency files
LOCAL_DEP = $(addprefix $(DEPDIR)/,$(LOCAL_SRC:.cpp=.d))
EXT_DEP = $(addprefix $(DEPDIR)/libraries/,$(notdir $(EXT_SRC:.cpp=.d)))
DEPS = $(LOCAL_DEP) $(EXT_DEP)

# Compiler flags
CXXFLAGS = \
	-Wall \
	-I../libraries \
        -g -O0 \
	$(shell fltk-config --cxxflags) \
        -fsanitize=address \
        -MMD -MP -MF $(DEPDIR)/$*.d

# Linker flags
LFLAGS = \
	$(shell fltk-config --ldflags) \
	-lasound \
	-lpthread \
        -ljack \
        -fsanitize=address

# Detect if JACK is available
JACK_TEST := $(shell pkg-config --exists jack 2>/dev/null && echo "yes")
ifeq ($(JACK_TEST),yes)
    CXXFLAGS += -D__LINUX_ALSA__ -D__UNIX_JACK__
    LFLAGS += `pkg-config --libs jack`
endif

# Default target
all: $(EXE)

# Link executable
$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(LFLAGS)

# Compile source files
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@) $(DEPDIR)/$(dir $*)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/libraries/%.o: ../libraries/%.cpp
	@mkdir -p $(dir $@) $(DEPDIR)/libraries
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Include dependency files
-include $(DEPS)

# Clean
clean:
	rm -rf $(OBJDIR)
	rm -f $(EXE)

# Strip binary
strip: $(EXE)
	strip --strip-all $(EXE)

.PHONY: all clean strip

