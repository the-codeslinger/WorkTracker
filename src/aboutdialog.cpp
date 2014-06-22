#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget* parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint
                    | Qt::WindowCloseButtonHint)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->licenseLabel->setOpenExternalLinks(true);
    ui->githubLabel->setOpenExternalLinks(true);
    ui->blogLabel->setOpenExternalLinks(true);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
