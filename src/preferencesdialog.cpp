#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "controller/preferencescontroller.h"

PreferencesDialog::PreferencesDialog(PreferencesController* p_controller, QWidget* p_parent)
    : QDialog(p_parent)
    , ui(new Ui::PreferencesDialog)
    , m_controller(p_controller)
{
    ui->setupUi(this);
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}
