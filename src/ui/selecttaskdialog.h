#ifndef SELECTTASKDIALOG_H
#define SELECTTASKDIALOG_H

#include "../model/datasource.h"

#include <QDialog>
#include <QDomDocument>

namespace Ui {
    class SelectTaskDialog;
}

/*!
 * A simple dialog with just one line-edit in which the user can search for existing 
 * tasks or enter a new one.
 */
class SelectTaskDialog : public QDialog
{
    Q_OBJECT
    
public:
    /*!
     * Create a new dialog. The data-source is needed to create the model that the 
     * completer uses.
     */
    SelectTaskDialog(DataSource dataSource, QWidget* parent = nullptr);
    
    /*!
     * Releases the ui resources.
     */
    ~SelectTaskDialog();
    
    /*!
     * \return 
     * Returns the name of the task the user selected or entered.
     */
    QString taskName() const;
    
protected slots:
    /*!
     * Evaluates the current content of the line-edit and enables or disables the OK
     * button accordingly.
     */
    void textChanged(const QString& text);
    
private:
    Ui::SelectTaskDialog* ui;
};

#endif // SELECTTASKDIALOG_H
