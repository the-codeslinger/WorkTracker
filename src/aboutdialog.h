#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
    class AboutDialog;
}

/*!
 * \brief Simple about dialog with the version number and some links to related websites.
 */
class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
     * Creates a new WorkTracker about dialog.
     *
     * \param parent
     * Parent of the dialog.
     */
    explicit AboutDialog(QWidget* parent = nullptr);

    /*!
     * Deletes the ui resources.
     */
    ~AboutDialog();

private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
