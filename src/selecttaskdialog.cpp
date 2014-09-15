#include "selecttaskdialog.h"
#include "ui_selecttaskdialog.h"
#include "model/ui/tasklistmodel.h"

#include <QCompleter>
#include <QPushButton>

SelectTaskDialog::SelectTaskDialog(const QDomDocument& p_dataSource, QWidget* p_parent)
    : QDialog(p_parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint
                        | Qt::WindowCloseButtonHint)
    , ui(new Ui::SelectTaskDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);
    
    QCompleter* completer = new QCompleter(this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    completer->setModel(new TaskListModel(p_dataSource, this));
    ui->lineEdit->setCompleter(completer);
    
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &SelectTaskDialog::textChanged);
}

SelectTaskDialog::~SelectTaskDialog()
{
    delete ui;
}

QString 
SelectTaskDialog::taskName() const
{
    return ui->lineEdit->text();
}

void 
SelectTaskDialog::textChanged(const QString& p_text)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(p_text.isEmpty());
}
