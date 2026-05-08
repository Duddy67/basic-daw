#include "../../main.h"


void Application::onAdd()
{
    if (addTrackDlg == nullptr) {
        addTrackDlg = new AddTrackDialog(x() + MODAL_WND_POS, y() + MODAL_WND_POS, XLARGE_SPACE, LARGE_SPACE, "Add track");
    }

    if (addTrackDlg->runModal() == DIALOG_OK) {
        // Add new file logic here...
    }
    // Add add track logic here...
    projectCtrl->onAddTrack();
}

void Application::onRemove()
{
    // Add remove track logic here...
}

