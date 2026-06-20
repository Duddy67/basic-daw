#include "transport_bar.h"
#include "../main.h"



TransportBar::TransportBar(int x, int y, int w, int h, Application& app) :
  Fl_Group(x, y, w, h), application(app)
{
    box(FL_DOWN_BOX);
    // Create transport buttons.
    play = new Fl_Button(TINY_SPACE, SMALL_SPACE + TINY_SPACE, BUTTON_WIDTH, BUTTON_HEIGHT, "@>");
    stop = new Fl_Button((TINY_SPACE * 2) + MEDIUM_SPACE, SMALL_SPACE + TINY_SPACE, BUTTON_WIDTH, BUTTON_HEIGHT, "@square");
    record = new Fl_Button((TINY_SPACE * 3) + (MEDIUM_SPACE * 2), SMALL_SPACE + TINY_SPACE, BUTTON_WIDTH, BUTTON_HEIGHT, "@circle");
    loop = new Fl_Light_Button((TINY_SPACE * 4) + (MEDIUM_SPACE * 3), SMALL_SPACE + TINY_SPACE, BUTTON_WIDTH, BUTTON_HEIGHT, "@reload");
    loop->selection_color(FL_GREEN);
    // Set the loop button shortcut to the L key (ie: numeric code = 108).
    loop->shortcut(108);

    // Assign a callback function to buttons.
    play->callback([](Fl_Widget* w, void* userData) {
                          TransportBar* tb = static_cast<TransportBar*>(userData);
                          // Make sure a project exists.
                          if (tb->application.getProject()) {
                              tb->onPlay();
                          }
                      }, (void*) this);
    stop->callback([](Fl_Widget* w, void* userData) {
                          TransportBar* tb = static_cast<TransportBar*>(userData);
                          if (tb->application.getProject()) {
                              tb->onStop();
                          }
                      }, (void*) this);
    record->callback([](Fl_Widget* w, void* userData) {
                          TransportBar* tb = static_cast<TransportBar*>(userData);
                          if (tb->application.getProject()) {
                              tb->onRecord();
                          }
                      }, (void*) this);
    loop->callback([](Fl_Widget* w, void* userData) {
                          TransportBar* tb = static_cast<TransportBar*>(userData);
                          if (tb->application.getProject()) {
                              tb->onLoop();
                          }
                      }, (void*) this);

    // Disable keyboard focus on buttons
    play->clear_visible_focus();
    stop->clear_visible_focus();
    record->clear_visible_focus();
    loop->clear_visible_focus();
    end();

}

TransportBar::~TransportBar()
{

}


int TransportBar::handle(int event)
{
    switch (event) {

    }

    // Default - Let Fl_Group handle any not processed events.
    return Fl_Group::handle(event);
}

void TransportBar::onPlay()
{
    // Make sure there is at least one track before playing.
    if (application.getProject()->trackCount()) {
        //std::cout << "onPlay()." << std::endl;
        application.getTransport().play();
    }
}

void TransportBar::onStop()
{
    application.getTransport().stop();
}

void TransportBar::onRecord()
{
    if (application.getProject()->trackCount()) {
        application.getTransport().record();
    }
}

void TransportBar::onLoop()
{

}
