#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
    class PreferencesDialog;
}

class PreferencesController;

/*!
 * Does as the name implies. Contains the ui and the ui-logic of a dialog to select
 * different settings of the application. Business logic is encapsulated in 
 * `SettingsController` which creates this dialog.
 */
class PreferencesDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
     * Create a new dialog instance.
     * 
     * \param p_controller
     * The controller that manages this dialog.
     * 
     * \param p_parent
     * The dialog's parent widget.
     */
    PreferencesDialog(PreferencesController* p_controller, QWidget* p_parent = nullptr);
    
    /*!
     * Releases the ui resources.
     */
    ~PreferencesDialog();
    
protected:
    /*!
     * Acts on the `QEvent::LanguageChange` event and retranslates the ui.
     *
     * \param p_event
     * The event that happened.
     */
    void changeEvent(QEvent* p_event);
    
private:
    Ui::PreferencesDialog* ui;
    PreferencesController* m_controller;
};

#endif // SETTINGSDIALOG_H
