#include "addnewtransaction.h"
#include "ui_addnewtransaction.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QDebug>
#include <QFileDialog>
AddNewTransaction::AddNewTransaction(QWidget *parent, int priv, QString accnum, DataBaseManagment* db) :
    QDialog(parent), priv(priv), accnum(accnum), databaseptr(db),
    ui(new Ui::AddNewTransaction)
{
    ui->setupUi(this);
}

AddNewTransaction::~AddNewTransaction()
{
    delete ui;
}

QList<QString>* AddNewTransaction::getTransaction()
{
    return &transaction;
}

bool AddNewTransaction::higherPrivillageTransaction()
{
    QString input = ui->transactionBox->text();
    //regex
    //[+-]?[0-9]+(.[0-9]{2})?\s+[0-9]{12}\s+[1-9][0-9]{3}-[1-9][1-2]-([0-2][1-9]|30|31)\s+(0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9]\s+[A-Z]+
    QString pttrn = "[+-]?[0-9]+(.[0-9]{2})?\\s+[0-9]{12}\\s+[1-9][0-9]{3}-[1-9][1-2]-([0-2][1-9]|30|31)\\s+(0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9]\\s+[A-Z]+";

    QRegularExpression re(QRegularExpression::anchoredPattern(pttrn));

    QRegularExpressionMatch match = re.match(input);

    if(match.hasMatch()){

        QList<QString> transactionParts = input.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        transaction = transactionParts;
        ui->transactionBox->setText("");

        return true;
    }
    else
        return false;
}

bool AddNewTransaction::lowerPrivillageTransaction()
{
    QString input = ui->transactionBox->text();

    QString pttr = QString("[+-]?[0-9]+(.[0-9]{2})?\\s+%1\\s+[1-9][0-9]{3}-[1-9][1-2]-([0-2][1-9]|30|31)\\s+(0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9]\\s+[A-Z]+").arg(accnum);
    QRegularExpression re(QRegularExpression::anchoredPattern(pttr));
    QRegularExpressionMatch match = re.match(input);

    if(match.hasMatch()){
        QList<QString> transactionParts = input.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        transaction = transactionParts;
        ui->transactionBox->setText("");
        return true;
    }
    else
        return false;
}

void AddNewTransaction::wrongMessage(const char* msg)
{
    QMessageBox infoMessage;
    infoMessage.setMinimumSize(200,100);
    infoMessage.setWindowTitle("Adding new transaction status");
    infoMessage.setIcon(QMessageBox::Critical);
    infoMessage.setText(msg);
    infoMessage.setStandardButtons(QMessageBox::Ok);
    infoMessage.exec();
}

void AddNewTransaction::lowerPrivillageImport(QString& fileName)
{
    QFile csv(fileName);
    if(csv.open(QIODevice::ReadOnly)) {
        qDebug()<<"found, and opened file for import";
        QList<QList<QString>> transactionsInParts;
        QTextStream in(&csv);
        QString line = in.readLine();
        while(!line.isNull()){

            QString pttrn = QString("[+-]?[0-9]+(.[0-9]{2})?;%1;[1-9][0-9]{3}-[1-9][1-2]-([0-2][1-9]|30|31);(0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9];[A-Z]+").arg(accnum);
            line.replace( " ", "" );
            QRegularExpression re(QRegularExpression::anchoredPattern(pttrn));
            QRegularExpressionMatch match = re.match(line);
            if(match.hasMatch()){
                //rozłożenie na części
                QList<QString> tmp;
                tmp = line.split(QRegExp(";"), QString::SkipEmptyParts);
                transactionsInParts.append(tmp);
            }else{
                qDebug()<<"Wrong transaction";
            }
            line = in.readLine();
        }
        csv.close();
        //dodanie do bazy zaimportowanych transakcji
        if(databaseptr)
            if(databaseptr->addFromImport(&transactionsInParts))
                qDebug()<<"Transactions imported successfully";

    }else{
        QString errMsg;
        QFileDevice::FileError err = QFileDevice::NoError;
        errMsg = csv.errorString();
        err = csv.error();
        qDebug()<<errMsg;
        qDebug()<<err;
    }


}

void AddNewTransaction::higherPrivillageImport(QString& fileName)
{
    QFile csv(fileName);
    if(csv.open(QIODevice::ReadOnly)) {
        qDebug()<<"found, and opened file for import";
        QList<QList<QString>> transactionsInParts;
        QTextStream in(&csv);
        QString line = in.readLine();
        while(!line.isNull()){
            qDebug()<<"ok";
            QString pttrn = QString("[+-]?[0-9]+(.[0-9]{2})?;[0-9]{12};[1-9][0-9]{3}-[1-9][1-2]-([0-2][1-9]|30|31);(0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9];[A-Z]+");
            line.replace( " ", "" );
            QRegularExpression re(QRegularExpression::anchoredPattern(pttrn));
            QRegularExpressionMatch match = re.match(line);
            if(match.hasMatch()){
                //rozłożenie na części
                QList<QString> tmp;
                tmp = line.split(QRegExp(";"), QString::SkipEmptyParts);
                transactionsInParts.append(tmp);
            }else{
                qDebug()<<"Wrong transaction";
            }
            line = in.readLine();
        }
        csv.close();
        //dodanie do bazy zaimportowanych transakcji
        if(databaseptr)
            if(databaseptr->addFromImport(&transactionsInParts))
                qDebug()<<"Transactions imported successfully";

    }else{
        QString errMsg;
        QFileDevice::FileError err = QFileDevice::NoError;
        errMsg = csv.errorString();
        err = csv.error();
        qDebug()<<errMsg;
        qDebug()<<err;
    }

}


void AddNewTransaction::on_okButton_clicked()
{
    bool ok;
    if(priv) //child
        ok = lowerPrivillageTransaction();
    else //parent
        ok = higherPrivillageTransaction();

    if(ok)
        accept();
    else{
        if(ui->transactionBox->text() == ""){
            wrongMessage("Transaction can't be empty");
        }else
            wrongMessage("Input does not match the pattern");
    }
}

void AddNewTransaction::on_cancelButton_clicked()
{
    reject();
}

void AddNewTransaction::on_imortCSVButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open CSV"), "/home/", tr("CSV Files (*.csv)"));
    if(fileName != ""){

        if(priv) //child
            lowerPrivillageImport(fileName);
        else //parent
            higherPrivillageImport(fileName);
        qDebug()<<"finally";
        databaseptr->getModel()->select();
    }
}
