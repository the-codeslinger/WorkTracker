#include "preferencescontroller.h"
#include "../ui/preferencesdialog.h"

#include <QSettings>

PreferencesController::PreferencesController(QWidget* p_parentWidget, QObject* p_parent)
    : QObject(p_parent)
    , m_dialog(new PreferencesDialog(this, p_parentWidget))
{
    connect(m_dialog, &PreferencesDialog::accepted, 
            this,     &PreferencesController::saveSettings);
    connect(m_dialog, &PreferencesDialog::rejected, 
            this,     &PreferencesController::restoreSettings);
}

void
PreferencesController::run()
{
    m_dialog->show();
}

void 
PreferencesController::saveSettings()
{
    emit finished(true);
}

void 
PreferencesController::restoreSettings()
{
    emit finished(false);
}
