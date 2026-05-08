#include "controller.h"


namespace Project {

    Controller::Controller(Application& app, Model& m) : pApplication(app), model(m)
    {

    }

    /*
     * Unregister an observer (important to prevent crashes when views are deleted).
     */
    void Controller::removeObserver(Observer* observer)
    {
        auto it = std::find(observers.begin(), observers.end(), observer);

        if (it != observers.end()) {
            observers.erase(it);
        }
    }

    void Controller::notify(ProjectEvent event, int index)
    {
        for (auto* observer : observers) {
            observer->onEvent(event, index);  
        }
    }

    void Controller::onAddTrack()
    {
        model.addTrack();
        std::cout << "Controller => onAddTrack()" << std::endl;
        notify(ProjectEvent::ADD_TRACK, 0);
    }
}
