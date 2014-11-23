#include "editorwizard.h"

#include <QPushButton>

EditorWizard::EditorWizard(EditorController* p_controller, QWidget* p_parent)
    : QWizard(p_parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint
                        | Qt::WindowCloseButtonHint)
    , m_controller(p_controller)
{
    setWindowTitle(tr("Edit Work Tasks"));
}

void
EditorWizard::validationError()
{
    button(QWizard::BackButton)->setDisabled(true);
    button(QWizard::FinishButton)->setDisabled(true);
}

void 
EditorWizard::validationSuccess()
{
    button(QWizard::BackButton)->setDisabled(false);
    button(QWizard::FinishButton)->setDisabled(false);
}
