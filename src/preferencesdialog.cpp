#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "controller/preferencescontroller.h"

#include <QMapIterator>
#include <QApplication>
#include <QComboBox>

PreferencesDialog::PreferencesDialog(PreferencesController* p_controller, QWidget* p_parent)
    : QDialog(p_parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint
                        | Qt::WindowCloseButtonHint)
    , ui(new Ui::PreferencesDialog)
    , m_controller(p_controller)
{
    ui->setupUi(this);
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void
PreferencesDialog::changeEvent(QEvent* p_event)
{
    if (QEvent::LanguageChange == p_event->type()) {
        ui->retranslateUi(this);
    }

    QDialog::changeEvent(p_event);
}
