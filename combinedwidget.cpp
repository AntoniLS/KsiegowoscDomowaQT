#include "combinedwidget.h"
#include "ui_combinedwidget.h"
#include <QCheckBox>
#include <QVBoxLayout>
#include <QDebug>
#include <QMessageBox>

combinedWidget::combinedWidget(QWidget *parent, QList<QString>*lst) :
    QDialog(parent),
    ui(new Ui::combinedWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("Combine differnt profiles bills");
    QVBoxLayout* qvbl = new QVBoxLayout(this);
    ui->groupBox->setLayout(qvbl); //some problem
    for(const auto&prof : *lst){
        QCheckBox* tempCheck = new QCheckBox(prof,this);
        qvbl->addWidget(tempCheck);
        tmp.append(tempCheck);
    }
}

combinedWidget::~combinedWidget()
{
    delete ui;
}

QList<QString> combinedWidget::getCheckedProfiles()
{
    return checkedProfiles;
}



void combinedWidget::on_cancelButton_clicked()
{
    reject();
}

void combinedWidget::on_okButton_clicked()
{
    for( const auto&checkBox : tmp){
        if(checkBox->isChecked()){
            checkedProfiles.append(checkBox->text());
        }
    }
    if(!checkedProfiles.empty()){
     accept();
    }
    else{
        QMessageBox empt;
        empt.setIcon(QMessageBox::Critical);
        empt.setStandardButtons(QMessageBox::Ok);
        empt.setText("You must pick at least one profile!");
        empt.setWindowTitle("Nothing was selected!");
        empt.exec();
    }
}
