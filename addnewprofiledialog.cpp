#include "addnewprofiledialog.h"
#include "ui_addnewprofiledialog.h"
#include <QDebug>
#include <QMessageBox>
#include "additionalTypes.h"
AddNewProfileDialog::AddNewProfileDialog(QWidget *parent, QList<QString>*ptrList) :
    QDialog(parent),
    ui(new Ui::AddNewProfileDialog)
{
    this->ptrProfileList = ptrList;
    ui->setupUi(this);
    ui->BoxForProfTypes->addItem("Parent");
    ui->BoxForProfTypes->addItem("Child");
    this->setWindowTitle("Add new profile");
    ui->EditPin->setEchoMode(QLineEdit::EchoMode::Password);
    ui->SaveButton->setEnabled(false);
    ui->EditAccountNumber->setMaxLength(12);
    ui->EditPin->setMaxLength(8);
    ui->EditName->setMaxLength(15);

    connect(ui->EditName,&QLineEdit::textChanged,this,&AddNewProfileDialog::checkIfNotEmpty);
    connect(ui->EditPin,&QLineEdit::textChanged,this,&AddNewProfileDialog::checkIfNotEmpty);
    connect(ui->EditAccountNumber, &QLineEdit::textChanged,this,&AddNewProfileDialog::checkIfNotEmpty);
}

AddNewProfileDialog::~AddNewProfileDialog()
{
    delete ui;
}

profileDataCapsule AddNewProfileDialog::getNewProfileData()
{
    profileDataCapsule data;
    data.name = newProfileName;
    data.pin = newPin;
    data.profileType = newProfileType;
    data.accnum = newAccNumber;
    return data;
}

void AddNewProfileDialog::on_CancelButton_clicked()
{
    reject();
}

void AddNewProfileDialog::on_SaveButton_clicked()
{
    //TODO SPRAWDZIC CZY NAZWA PROFILU NIE JEST TAKA SAMA JAK ISTNIEJACA
    if(ptrProfileList->contains(ui->EditName->text())){
       QMessageBox::critical(this, "Warning", "This profile already exisits!");
       ui->EditName->setText("");
    }else{
        newPin = ui->EditPin->text();
        newProfileName = ui->EditName->text();
        newAccNumber = ui->EditAccountNumber->text();
        newProfileType = ui->BoxForProfTypes->currentIndex(); // 0 - Parent, 1 - Child
        qDebug()<<"selectionbox index 0-parent, 1-child >>>" <<newProfileType;
        accept();
    }
}

void AddNewProfileDialog::checkIfNotEmpty()
{
    if(ui->EditName->text().isEmpty() || ui->EditPin->text().isEmpty()){ //nie pozwalamy przejsc dalej jesli pola nie sa wypelnione
        ui->SaveButton->setEnabled(false);
    }else{
        ui->SaveButton->setEnabled(true);
    }
}
