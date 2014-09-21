#ifndef EDITORWIZARD_H
#define EDITORWIZARD_H

#include <QWizard>

class EditorController;

class EditorWizard : public QWizard
{
    Q_OBJECT
public:
    EditorWizard(EditorController* p_controller, QWidget* p_parent = nullptr);
    
public slots:
    //void back();
    //void accept();
    //void reject();
    /*!
     * Disables the "back" and "finish" button.
     */
    void validationError();
    
    /*!
     * Eables the "back" and "finish" button.
     */
    void validationSuccess();
    
private:
    EditorController* m_controller;
    
};

#endif // EDITORWIZARD_H
