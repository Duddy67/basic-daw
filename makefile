SRC = main.cpp application/menu.cpp application/application.cpp application/callbacks.cpp dialogs/dialog.cpp \
      dialogs/new.cpp application/menu/edit.cpp project/model.cpp project/view.cpp \
      application/menu/file.cpp widgets/track.cpp views/track_list.cpp application/menu/track.cpp \
      audio/engine.cpp audio/track.cpp views/channel_strip.cpp views/timeline.cpp dialogs/add_track.cpp \
      project/controller.cpp widgets/strip.cpp
CXX = g++
CXXFLAGS = -Wall $(shell fltk-config --cxxflags)

LFLAGS = $(shell fltk-config --ldflags)

OBJS = $(SRC:.cpp=.o)
DIR_OBJ = obj/
DIR_OBJS = $(addprefix $(DIR_OBJ), $(OBJS))

$(DIR_OBJ)%.o: %.cpp *.h
	$(CXX) $(CXXFLAGS) -c $(<) -o $(@)

EXE = basicDAW

all: $(EXE)

$(EXE): $(DIR_OBJS)
	$(CXX) -o $@ $^ $(LFLAGS)

depend:
	makedepend -- $(CXXFLAGS) -- $(SRC)

strip: $(EXE)
	strip --strip-all $(EXE)

clean:
	rm -f $(DIR_OBJS)
	rm -f $(EXE)
