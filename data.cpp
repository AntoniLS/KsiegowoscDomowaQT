#include <QDebug>
#include <QInputDialog>
#include <QString>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QFile>
#include <QFileDialog>
#include "addnewtransaction.h"
#include "combinedwidget.h"
#include "devcontroltools.h"
#include "data.h"
#include "chartsgenerator.h"

/////////////BASE PROFILE////////////////
QString BaseProfile::getAccnumber(){
    return accnumber;
}
QString BaseProfile::getName(){
    return name;
}

QTableView *BaseProfile::getPtv()
{
    return ptv;
}
void BaseProfile::loadAllBills(QTableView *table, DataBaseManagment* dataBase)
{
    //bierze tylko te wydatki skojarzone z jego numerem konta
    dataBase->getBillsOfSeclectedAccNum(table,this->accnumber);

}

void BaseProfile::showOnlyIncome(CurrentOptions* copt, int* flag)
{

   if(!copt->income){
     if(copt->outcome){ //aby wylaczyc mozliwosc income && outcome w tym samym momencie
         showOnlyOutcome(copt, nullptr);
         //wylaczenie outcome
         if(flag)
             (*flag)++;
     }
     for(int i = 0; i < ptv->model()->rowCount(); i++){
       if(ptv->model()->index(i,0).data().toDouble() < 0){
           ptv->hideRow(i);
       }
     }
     copt->income = true;
   }
   else{
       if(copt->combined){
           for(int i = 0; i < ptv->model()->rowCount(); i++){
                  if(ptv->model()->index(i,0).data().toDouble() < 0 && copt->combinedList.contains(getPtv()->model()->index(i,1).data().toString())){
                        ptv->showRow(i);
                  }
           }
       }
       if(copt->onlyProfOf){
           for(int i = 0; i < ptv->model()->rowCount(); i++){
             if(ptv->model()->index(i,0).data().toDouble() < 0 && ptv->model()->index(i,1).data().toString() == copt->onlyProfOfAcc){
                 ptv->showRow(i);
             }
           }
       }else{
           for(int i = 0; i < ptv->model()->rowCount(); i++){
             if(ptv->model()->index(i,0).data().toDouble() < 0){
                 ptv->showRow(i);
             }
           }
       }
       copt->income = false;
   }
}

void BaseProfile::showOnlyOutcome(CurrentOptions* copt, int* flag)
{
   if(!copt->outcome){
       if(copt->income){ //aby wylaczyc mozliwosc income && outcome w tym samym momencie
           showOnlyIncome(copt, nullptr);
           //wylaczenie income
           if(flag)
               (*flag)++;
       }
        for(int i = 0; i < ptv->model()->rowCount(); i++){
            if(ptv->model()->index(i,0).data().toDouble() > 0){
                ptv->hideRow(i);
            }
    }
    copt->outcome = true;
   }
   else{
       if(copt->combined){
           for(int i = 0; i < ptv->model()->rowCount(); i++){
                  if(ptv->model()->index(i,0).data().toDouble() > 0 && copt->combinedList.contains(getPtv()->model()->index(i,1).data().toString())){
                        ptv->showRow(i);
                  }
           }
       }
       if(copt->onlyProfOf){
           for(int i = 0; i < ptv->model()->rowCount(); i++){
               if(ptv->model()->index(i,0).data().toDouble() > 0 && ptv->model()->index(i,1).data().toString() == copt->onlyProfOfAcc){
                   ptv->showRow(i);
               }
           }
       }else{
           for(int i = 0; i < ptv->model()->rowCount(); i++){
               if(ptv->model()->index(i,0).data().toDouble() > 0){
                   ptv->showRow(i);
               }
           }
       }
       copt->outcome = false;
   }
}

void BaseProfile::showEverything(CurrentOptions *copt)
{
    if(copt->combined){
        showAll();
        copt->combined = false;
        copt->combinedList.clear();
    }
    if(copt->onlyProfOf){
        showAll();
        copt->onlyProfOf = false;
    }
    if(copt->outcome){
        for(int i = 0; i < ptv->model()->rowCount(); i++){
            if(ptv->model()->index(i,0).data().toDouble() > 0){
                ptv->showRow(i);
            }
        }
        copt->outcome = false;

    }if(copt->income){
        for(int i = 0; i < ptv->model()->rowCount(); i++){
            if(ptv->model()->index(i,0).data().toDouble() < 0){
                ptv->showRow(i);
            }
        }
        copt->income = false;
    }

}

bool BaseProfile::exportData(QTableView *tv)
{
    QString data;
    int rows = tv->model()->rowCount();
    int columns = tv->model()->columnCount();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
                data += tv->model()->data(tv->model()->index(i,j)).toString();
                if(j!=columns-1){
                    data += ";";
                }
        }
        data += " \n";
    }

    QFileDialog tmp(tv);
    QString title("Export as CSV and TXT");
    QString fileName = tmp.getSaveFileName(tv, title,
                        "Name of file");

    if(fileName != ""){

        QFile csv(QString("%1.csv").arg(fileName));
        if(csv.open(QIODevice::WriteOnly)) {
            qDebug()<<"okcsv";
            QTextStream out(&csv);
            out << data;
            csv.close();
        }else{
            QString errMsg;
            QFileDevice::FileError err = QFileDevice::NoError;
            errMsg = csv.errorString();
            err = csv.error();
            qDebug()<<errMsg;
            qDebug()<<err;
        }

        // .txt
        QFile txtFile(QString("%1.txt").arg(fileName));
        if(txtFile.open(QIODevice::WriteOnly)) {
            qDebug()<<"oktxt";
            QTextStream out(&txtFile);
            out << data;

            txtFile.close();
        }else{
            QString errMsg;
            QFileDevice::FileError err = QFileDevice::NoError;
            errMsg = csv.errorString();
            err = csv.error();
            qDebug()<<errMsg;
            qDebug()<<err;
        }
        qDebug()<<"Done";
    }

}

void BaseProfile::sort() // sorting by value ascending
{
    ptv->sortByColumn(0,Qt::AscendingOrder);
}

int BaseProfile::retPrivillageLevel() //type 0 parent, 1 child
{
    return 1;
}

bool BaseProfile::selectOnlyOneProfile(DataBaseManagment* dataBase,CurrentOptions* copt)
{
    return false; //obiekt klasy bazowej (profil typu child) nie ma uprawnień
}

bool BaseProfile::combineProfiles(DataBaseManagment *dataBase, CurrentOptions *copt)
{
    return false; //obiekt klasy bazowej (profil typu child) nie ma uprawnień
}

void BaseProfile::addNewTransaction(DataBaseManagment *dataBase, CurrentOptions *copt)
{
    //Dla child opcja zablokowana na pojedyńczym numerze konta
    AddNewTransaction* atran = new AddNewTransaction(nullptr, 1, getAccnumber(), dataBase); //child
    int ret = atran->exec();
    if(ret){
        dataBase->addNewPlusTransactionToDataBase(atran->getTransaction());
        dataBase->getModel()->select();
    }else{
        qDebug()<<"Canceled";
    }
    delete atran;
}

void BaseProfile::generateChart(QTableView *table)
{

   ChartsGenerator* chartObj = new ChartsGenerator(0, table, 0);
   chartObj->setWindowTitle("Charts");
   int ret = chartObj->exec();
   if(!ret){
       delete chartObj;
   }

}

void BaseProfile::showAll()
{
    for(int i = 0; i < ptv->model()->rowCount(); i++){
            ptv->showRow(i);
    }
}



/////////////////HIGH PRIVILLAGE PROFILE///////////////////////
void HighPrivillageProfile::loadAllBills(QTableView * table, DataBaseManagment* dataBase)
{
    //pobiera wszystkie transakcje
    dataBase->getAllBills(table);
}

int HighPrivillageProfile::retPrivillageLevel() //type 0 parent, 1 child
{
    return 0;
}

///Wybieramy sposrod profili, ktore sa powiazane z relacją BILLS
bool HighPrivillageProfile::selectOnlyOneProfile(DataBaseManagment* dataBase, CurrentOptions* copt)
{
    if(!copt->onlyProfOf){
        QList<QString> temp;
        dataBase->getAvailableProfiles(&temp);
        bool ok;
        QString chosen = QInputDialog::getItem(getPtv(),"Choose profile to show", "Profile",temp,0,false,&ok);
        if(ok){
            qDebug()<<chosen;
            profileDataCapsuleLite t = dataBase->getStartDataOfChosenProfile(chosen);
            for(int i = 0; i < getPtv()->model()->rowCount(); i++){
               if(getPtv()->model()->index(i,1).data().toString() != t.accnum){
                    getPtv()->hideRow(i);
               }
            }
            copt->onlyProfOf = true;
            copt->onlyProfOfAcc = t.accnum;
            copt->onlyProfOfAcc = t.accnum;
        }else{
            DevControlTools::consoleLog("Canceled chosing profile");
        }
        return ok;
    }else{
        //if it is set, when we want tu undo it
        if(copt->income or copt->outcome){
            if(copt->income){
                for(int i = 0; i < getPtv()->model()->rowCount(); i++){
                    if(getPtv()->model()->index(i,0).data().toDouble() > 0){
                        getPtv()->showRow(i);
                    }
                }
            }
            else if(copt->outcome){
                for(int i = 0; i < getPtv()->model()->rowCount(); i++){
                    if(getPtv()->model()->index(i,0).data().toDouble() < 0){
                        getPtv()->showRow(i);
                    }
                }
            }
        }else{
            showAll();
        }
        copt->onlyProfOf = false;
    }

}

bool HighPrivillageProfile::combineProfiles(DataBaseManagment *dataBase, CurrentOptions *copt)
{
    if(!copt->combined){
        QList<QString> temp;
        QList<QString> accnums;
        dataBase->getAvailableProfiles(&temp);
        combinedWidget* comb = new combinedWidget(nullptr,&temp);
        int ret = comb->exec();
        if(ret){
            //przejac wybrane
            temp = comb->getCheckedProfiles();
            //querry
            accnums = dataBase->getAccountnumbersContainer(&temp);
            if(!accnums.isEmpty()){
                copt->combinedList = accnums;
                for(int i = 0; i < getPtv()->model()->rowCount(); i++){
                       if(!copt->combinedList.contains(getPtv()->model()->index(i,1).data().toString())){
                             getPtv()->hideRow(i);
                       }
                }
                copt->combined = true;
                copt->onlyProfOf = true;
            }
            delete comb;
            return true;
        }else{
            qDebug()<<"canceled";
            delete comb;
            return false;
        }
    }else{
        //if it is set, when we want tu undo it
        if(copt->income or copt->outcome){
            if(copt->income){
                for(int i = 0; i < getPtv()->model()->rowCount(); i++){
                    if(getPtv()->model()->index(i,0).data().toDouble() > 0){
                        getPtv()->showRow(i);
                    }
                }
            }
            else if(copt->outcome){
                for(int i = 0; i < getPtv()->model()->rowCount(); i++){
                    if(getPtv()->model()->index(i,0).data().toDouble() < 0){
                        getPtv()->showRow(i);
                    }
                }
            }
        }else{
            showAll();
        }
        copt->combined = false;
        copt->onlyProfOf = false;
    }

}

void HighPrivillageProfile::addNewTransaction(DataBaseManagment *dataBase, CurrentOptions *copt)
{
    AddNewTransaction* atran = new AddNewTransaction(nullptr,0,"",dataBase); // parent
    int ret = atran->exec();
    if(ret){
        qDebug()<<"Adding...";
        //dodanie do bazy danych
        dataBase->addNewPlusTransactionToDataBase(atran->getTransaction());
        dataBase->getModel()->select();
    }else{
        qDebug()<<"Canceled";
    }
    delete atran;
}

void HighPrivillageProfile::generateChart(QTableView *table)
{
    bool ok;
    QString msg = "Which variant do you want to generate";
    QStringList tempList;
    tempList << "Your spendings" << "Family spendings overall";
    QString item = QInputDialog::getItem(table,QString("choose option"),msg,tempList,0,false,&ok);
    if(ok){
        if(item == tempList[0]){ //tylko dla profilu zalogowanego
            ChartsGenerator* chartObj = new ChartsGenerator(0, table, 1, this->getAccnumber());
            chartObj->setWindowTitle("Charts");
            int ret = chartObj->exec();
            if(!ret){
                delete chartObj;
            }

        }else{ //dla wszystkich
            ChartsGenerator* chartObj = new ChartsGenerator(0, table, 0);
            chartObj->setWindowTitle("Charts");
            int ret = chartObj->exec();
            if(!ret){
                delete chartObj;
            }
        }
    }else{
        qDebug()<<"Generating charts cancelled!";
    }



}
