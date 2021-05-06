#include "databasemanagment.h"
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <iostream>

DataBaseManagment::DataBaseManagment()
{
   connectionStatus = false; //domyslnie
   setupDataBase();
}


void DataBaseManagment::setupDataBase()
{
    this->database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("/Users/antonils/Desktop/KsiegowoscDB/bills.db");
    mbox(database.open());
}

void DataBaseManagment::mbox(bool tf){ //Message box pop up
    QMessageBox infoMessage;
    infoMessage.setMinimumSize(200,100);
    infoMessage.setWindowTitle("Database status");
    if(tf){
        infoMessage.setIcon(QMessageBox::Information);
        infoMessage.setText("Database connected successfully");
        connectionStatus = true; // udalo sie, umożliwia późniejszy wybór profilów
    }else{
        infoMessage.setIcon(QMessageBox::Critical);
        infoMessage.setText("Couldn't connect database!");
    }
    infoMessage.setStandardButtons(QMessageBox::Ok);
    infoMessage.exec();
}


void DataBaseManagment::queryProfiles(QList<QString>* profileListNames) //Poczatkowe wczytanie nazw profilow
{
    QSqlQuery queryProfileNames(this->database);
    if(queryProfileNames.prepare("SELECT PROFILE_NAME FROM HOMEUSERS")){
       queryProfileNames.exec();
       while(queryProfileNames.next()){
           profileListNames->append(queryProfileNames.value(0).toString());
       }
    }else{
       dataBaseManagerLog("Couldn't prepare query");
    }
}

bool DataBaseManagment::addToDataBase(QString& name, QString& pin, int type, QString& accnum) //type 0 parent, 1 child
{
    //qDebug()<< name << pin << type;
    QString queryNewProfileString = QString("INSERT INTO HOMEUSERS(PROFILE_NAME, PIN, PROFILE_TYPE, ACC_NUMBER) ");
    queryNewProfileString.append(QString("VALUES ('%1', '%2', '%3', '%4')").arg(name).arg(pin).arg(type).arg(accnum));
    //qDebug() << queryNewProfile;
    QSqlQuery queryNewProfile(this->database);
    if(queryNewProfile.prepare(queryNewProfileString)){
        queryNewProfile.exec();
        return true;
    }
    return false;
}

bool DataBaseManagment::checkingPin(QString & pinTry, QString & selection)
{
    QString queryString = QString("SELECT PIN FROM HOMEUSERS WHERE PROFILE_NAME = '%1'").arg(selection);
    qDebug() << queryString;
    QSqlQuery queryPinToProfile(this->database);
    if(queryPinToProfile.prepare(queryString)){
        queryPinToProfile.exec();
        queryPinToProfile.next();
        if(pinTry == queryPinToProfile.value(0).toString()){
            return true;
        }
    }else{
        dataBaseManagerLog("Couldn't chcek for pin!");
        return false;
    }
    return false;
}

bool DataBaseManagment::deleteSelectedProfile(QString & selection)
{
    QString deleteString = QString("DELETE FROM HOMEUSERS WHERE PROFILE_NAME = '%1'").arg(selection);
    qDebug()<< deleteString;
    QSqlQuery deletingProfileQuery(this->database);
    if(deletingProfileQuery.prepare(deleteString)){
        deletingProfileQuery.exec();
        return true;
    }else{
        dataBaseManagerLog("Couldn't delete profile due to database problem");
        return false;
    }
}

profileDataCapsuleLite DataBaseManagment::getStartDataOfChosenProfile(QString & chosen)
{
    QString queryProfile = QString("SELECT ACC_NUMBER, PROFILE_TYPE FROM HOMEUSERS WHERE PROFILE_NAME = '%1'").arg(chosen);
    qDebug() << queryProfile;
    QSqlQuery queryThisOne(this->database);
    if(queryThisOne.prepare(queryProfile)){
        queryThisOne.exec();
        queryThisOne.next();
        profileDataCapsuleLite temp = {chosen, queryThisOne.value(0).toString(), queryThisOne.value(1).toInt()};
        qDebug()<<"GOT THIS FROM DB" <<temp.profileType;
        return temp;
    }
    profileDataCapsuleLite empty = {"","",999};
    return empty;
}

bool DataBaseManagment::addNewPlusTransactionToDataBase(QList<QString> *x)
{
    QList<QString> tranParts = *x;
    QString queryNewTransactionString = QString("INSERT INTO BILLS(AMOUNT, ACC_NUMBER, DATE_OF_TR, TIME_OF_TR, CATEGORY) ");
    queryNewTransactionString.append(QString("VALUES (%1, %2, '%3', '%4', '%5')").arg(tranParts[0], tranParts[1], tranParts[2],tranParts[3], tranParts[4]));
    QSqlQuery queryNewTransaction(this->database);
    if(queryNewTransaction.prepare(queryNewTransactionString)){
        queryNewTransaction.exec();
        qDebug()<<"Added Sucessfully!";
        return true;
    }
    qDebug()<<"Error ocurred while adding to database!";
    return false;
}

bool DataBaseManagment::addFromImport(QList<QList<QString>> *tranParts)
{
    for(const auto & tp : *tranParts){
        QString queryNewTransactionString = QString("INSERT INTO BILLS(AMOUNT, ACC_NUMBER, DATE_OF_TR, TIME_OF_TR, CATEGORY) ");
        queryNewTransactionString.append(QString("VALUES (%1, %2, '%3', '%4', '%5')").arg(tp[0], tp[1], tp[2], tp[3], tp[4]));

        QSqlQuery queryNewTransaction(this->database);
        if(queryNewTransaction.prepare(queryNewTransactionString)){
            queryNewTransaction.exec();
            qDebug()<<"Added Sucessfully!";
        }else{
            qDebug()<<"Error occured, skipping rest of transactions";
            return false;
        }
    }
    return true;
}

void DataBaseManagment::dataBaseManagerLog(const char* log){
    qDebug()<< "DATABASE MANAGER LOG: "<< log;
}

bool DataBaseManagment::getAllBills(QTableView* tv)
{
    QSqlQuery queryThisOne(this->database);
    QSqlTableModel *model = new QSqlTableModel(tv);
    model->setTable(QString("BILLS"));
    model->select();
    tv->setModel(model);
    mod = model;
}

bool DataBaseManagment::getAvailableProfiles(QList<QString> * tmpList)
{
   QString queryProfile =
   QString("SELECT DISTINCT H.PROFILE_NAME FROM HOMEUSERS AS H, BILLS AS B WHERE H.ACC_NUMBER = B.ACC_NUMBER");
    QSqlQuery queryProfileNames(this->database);
   if(queryProfileNames.prepare(queryProfile)){
      qDebug() << "profiles queried!";
      queryProfileNames.exec();
      while(queryProfileNames.next()){
          tmpList->append(queryProfileNames.value(0).toString());
      }
   }else{
      dataBaseManagerLog("Couldn't prepare query");
   }


}

bool DataBaseManagment::getBillsOfSeclectedAccNum(QTableView* tv,QString &accnum)
{
    QSqlQuery queryThisOne(this->database);
    QSqlTableModel *model = new QSqlTableModel(tv);
    model->setTable(QString("BILLS"));
    model->setFilter(QString("ACC_NUMBER = %1").arg(accnum));
    model->select();
    tv->setModel(model);
    mod = model;
}

QList<QString> DataBaseManagment::getAccountnumbersContainer(QList<QString> *names)
{
    bool sucessfull = true;
    QList<QString> accnums;
    for(const auto& name: *names){
        QString queryProfile = QString("SELECT ACC_NUMBER FROM HOMEUSERS WHERE PROFILE_NAME = '%1'").arg(name);
        QSqlQuery queryThisOne(this->database);
        if(queryThisOne.prepare(queryProfile)){
            queryThisOne.exec();
            queryThisOne.next();
            accnums.append(queryThisOne.value(0).toString());
        }else{
            sucessfull = false;
            break;
        }
    }
    if(!sucessfull){
        accnums.clear();
        return accnums;
    }
    if(accnums.count() == names->count()){
        dataBaseManagerLog("conversion to accnums, successfull");
        return accnums;
    }
}

bool DataBaseManagment::permission()
{
    return connectionStatus;
}

QSqlTableModel *DataBaseManagment::getModel()
{
    return mod;
}

