#include <QDebug>
#include <QPalette>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "profilelistselection.h"
#include "additionalTypes.h"
#include "data.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupBaseUi();
    this->setWindowOpacity(0);
    data.prof = nullptr;
    if(db.permission()){ //jesli pomyślnie wczytaliśmy baze danych
        completeProfileSelection();
    }
    copt.onlyProfOfAcc = "";
}

MainWindow::~MainWindow()
{
    delete ui;
    if(data.prof){
        delete data.prof;
    }
}

void MainWindow::consolelog(const char *log)
{
    qDebug() <<"CONSOLE LOG: "<< log;
}

void MainWindow::completeProfileSelection()
{
    ProfileListSelection* profileSelection = new ProfileListSelection(this, &db); //zabezpieczamy przed memory leak

    connect(profileSelection ,&ProfileListSelection::accepted, [=](){
        consolelog("profile selection accepted!");
        //wywołanie głównej czesci programu
        this->setWindowOpacity(1);
        QString chosenOne = profileSelection->getSelectedProfile();
        profileDataCapsuleLite temp = db.getStartDataOfChosenProfile(chosenOne);
        typeOfProfileDecission(temp);
        uiSmallUpgradeAfterProfileSelection();

        setUpMainTableView();
    });
    connect(profileSelection ,&ProfileListSelection::rejected, [=](){
        consolelog("profile selection rejected!");
        QApplication::quit(); // wyjscie z apikacji po odmowie zalogowania się
    });
    profileListSelectionSetUpUI(profileSelection);

    //przygotowanie okna profilów do działania
    db.queryProfiles(profileSelection->getPtrListOfProfilesFromDb()); //wczytanie z bazy danych
    profileSelection->loadList(); // zaladowanie do list widget
}

void MainWindow::profileListSelectionSetUpUI(ProfileListSelection* profileSelection){
    profileSelection->move(600,300);
    profileSelection->setWindowTitle("Profile Selection");
    profileSelection->setModal(true);
    profileSelection->show();
    profileSelection->raise();
    profileSelection->activateWindow();
}

void MainWindow::typeOfProfileDecission(profileDataCapsuleLite& tmp){
    if(tmp.profileType == 0){
        data.prof = new HighPrivillageProfile(tmp.name, tmp.accnum, ui->tableViewMain);
        consolelog("Parent type of profile loaded!");
    }else{
        data.prof = new BaseProfile(tmp.name, tmp.accnum, ui->tableViewMain);
        consolelog("Child type of profile loaded!");
    }
}

void MainWindow::uiSmallUpgradeAfterProfileSelection()
{
    this->ui->profileLabel->setText(data.prof->getName());
    this->setWindowTitle(QString("Bills of %1").arg(data.prof->getName()));

    if(data.prof->retPrivillageLevel() == 0){  //jesli zostal wybrany parent to przycisk only profile of jest aktywny
         consolelog("ui for parent");
        //setup
    }else if(data.prof->retPrivillageLevel() == 1){ //jesli zostal wybrany child to przycisk only/combine zostaje dezaktywowany
        ui->profileButton->setEnabled(false);
        ui->combineButton->setEnabled(false);
        consolelog("ui for child");

    }else {
        //todo throw
        consolelog("Profile returned wrong privillage level");
    }

}

void MainWindow::updateColumnNames()
{
    this->ui->tableViewMain->model()->setHeaderData(0,Qt::Orientation::Horizontal,tr("Amount"));
    this->ui->tableViewMain->model()->setHeaderData(1,Qt::Orientation::Horizontal,tr("Account Number"));
    this->ui->tableViewMain->model()->setHeaderData(2,Qt::Orientation::Horizontal,tr("Date"));
    this->ui->tableViewMain->model()->setHeaderData(3,Qt::Orientation::Horizontal,tr("Hour"));
    this->ui->tableViewMain->model()->setHeaderData(4,Qt::Orientation::Horizontal,tr("Bill Category"));
}

void MainWindow::setUpMainTableView()
{
    copt.income = false;
    copt.outcome = false;
    copt.onlyProfOf = false;
    copt.combined = false;
    data.prof->loadAllBills(ui->tableViewMain,&db); //wykonuje jedna z metod wirtualnych wypelniajacych tablice danymi
    updateColumnNames();
    ui->tableViewMain->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewMain->setSelectionBehavior(QAbstractItemView::SelectRows);
}





void MainWindow::on_incomeButton_clicked()
{
    int flag = 0;
    data.prof->showOnlyIncome(&copt, &flag);
    buttonPressedVisuals(ui->incomeButton, copt.income);
    if(flag){
        buttonPressedVisuals(ui->outcomeButton, copt.outcome);
    }
}

void MainWindow::on_outcomeButton_clicked()
{
    int flag = 0;
    data.prof->showOnlyOutcome(&copt, &flag);
    buttonPressedVisuals(ui->outcomeButton, copt.outcome);
    if(flag){
        buttonPressedVisuals(ui->incomeButton, copt.income);
    }
}

void MainWindow::on_showEverythingButton_clicked()
{
    data.prof->showEverything(&copt);
    buttonPressedVisuals(ui->outcomeButton, copt.outcome);
    buttonPressedVisuals(ui->incomeButton, copt.income);

    if(dynamic_cast<HighPrivillageProfile*>(data.prof) != nullptr){ //odblokowanie tylko dla HighPrivillageProfile
        ui->combineButton->setEnabled(true);
        ui->profileButton->setEnabled(true);
        buttonPressedVisuals(ui->profileButton, copt.onlyProfOf);
        buttonPressedVisuals(ui->combineButton, copt.combined);
    }
}

void MainWindow::buttonPressedVisuals(QPushButton* bt, bool tf){
    if(tf){
        bt->setStyleSheet("background-color: rgb(0, 90, 0)");
    }else{
        bt->setStyleSheet("background-color: rgb(60, 60, 60)");
    }
}

void MainWindow::setupBaseUi()
{
    ui->chartButton->setStyleSheet("background-color: rgb(60, 60, 60)");
    ui->exportButton->setStyleSheet("background-color: rgb(60, 60, 60)");
    ui->incomeButton->setStyleSheet("background-color: rgb(60, 60, 60)");
    ui->outcomeButton->setStyleSheet("background-color: rgb(60, 60, 60)");
    ui->showEverythingButton->setStyleSheet("background-color: rgb(60, 60, 60)");
    ui->profileButton->setStyleSheet("background-color: rgb(60, 60, 60)");
    ui->combineButton->setStyleSheet("background-color: rgb(60, 60, 60)");
    ui->sortButton->setStyleSheet("background-color: rgb(60, 60, 60)");
    ui->plusButton->setStyleSheet("background-color: rgb(60, 60, 60)");
    ui->minusButton->setStyleSheet("background-color: rgb(60, 60, 60)");
}

void MainWindow::on_sortButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Sort condition", "Sorting will reset your current settings\n are you sure you want to do it?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        this->on_showEverythingButton_clicked();
        data.prof->sort();
     } else {
        qDebug()<<"Selected: no";
     }
}

void MainWindow::on_profileButton_clicked()
{
    bool ok = data.prof->selectOnlyOneProfile(&db, &copt);
    if(ok){
        buttonPressedVisuals(ui->profileButton, copt.onlyProfOf);
        ui->combineButton->setEnabled(false);
    }else if(!copt.onlyProfOf){
        buttonPressedVisuals(ui->profileButton, copt.onlyProfOf);
        ui->combineButton->setEnabled(true);
    }
}

void MainWindow::on_combineButton_clicked()
{
    bool ok = data.prof->combineProfiles(&db, &copt);
    if(ok){
        buttonPressedVisuals(ui->combineButton, copt.combined);
        ui->profileButton->setEnabled(false);
    }else if(!copt.combined){
        buttonPressedVisuals(ui->combineButton, copt.combined);
        ui->profileButton->setEnabled(true);
    }
}

void MainWindow::on_plusButton_clicked()
{
    data.prof->addNewTransaction(&db, &copt);
}

void MainWindow::on_minusButton_clicked()
{

    QModelIndexList indexes =  ui->tableViewMain->selectionModel()->selectedRows();
    int countRow = indexes.count();
    for( int i = countRow; i > 0; i--)
        ui->tableViewMain->model()->removeRow( indexes.at(i-1).row(), QModelIndex());
    db.getModel()->select();

}

void MainWindow::on_exportButton_clicked()
{
    data.prof->exportData(ui->tableViewMain);
}

void MainWindow::on_chartButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Sort condition", "By generating chart your table will be sorted by date\n are you sure you want to do it?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        this->on_showEverythingButton_clicked();
        data.prof->generateChart(ui->tableViewMain);
     } else {
        qDebug()<<"Selected: no";
     }
}
