#include "profilelistselection.h"
#include "ui_profilelistselection.h"
#include "devcontroltools.h"
#include "addnewprofiledialog.h"
#include "additionalTypes.h"
#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>

ProfileListSelection::ProfileListSelection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProfileListSelection)
{
    ui->setupUi(this);
}
ProfileListSelection::ProfileListSelection(QWidget *parent, DataBaseManagment* ptrDb) :
    QDialog(parent),
    ui(new Ui::ProfileListSelection)
{
    ui->setupUi(this);
    this->db = ptrDb;
    QPixmap startPixmap("://budget.png");
    ui->ImagePlaceHolder->setPixmap(startPixmap);
};

ProfileListSelection::~ProfileListSelection()
{
    delete ui;
}
void ProfileListSelection::loadList()
{
    for(const auto&singleName : qAsConst(listOfProfilesFromDb)){ //iterowanie po kontenerze w celu utworzenia listy profilow
        ui->profileList->addItem(singleName);
    }
}

void ProfileListSelection::on_Continue_clicked()
{
    if(ui->profileList->selectedItems().size() != 0){ //sprawdzenie czy zostal zaznaczony jakikolwiek profil
        //przechwycenie co jest wybrane
        QList<QListWidgetItem*> selected = ui->profileList->selectedItems();
        QString singleSelection = selected[0]->text();
        //input widget, podanie pinu
        bool completedSuccessfully;
        QString PIN = QInputDialog::getText(this, tr("Pass your PIN"), tr("PIN: "), QLineEdit::Password, QString(), &completedSuccessfully);
        if(completedSuccessfully){
           DevControlTools::consoleLog("passed a pin");
           //jesli zgodny z pinem w krotce z nazwa wybranego proflu
           bool check = db->checkingPin(PIN, singleSelection);
           if(check){
               DevControlTools::consoleLog("Pin was right!");
               this->selectedProfile = singleSelection;
               accept(); //akceptujemy -> przejscie do glownej czesci programu z odpowiednia flaga
           }else{
               DevControlTools::consoleLog("Pin was WRONG!");
               QMessageBox::critical(this, "Warning", "Your PIN is incorrect");
           }
        }else{
            DevControlTools::consoleLog("canceled passing a pin");
        }
    }else{//jesli nic nie jest wybrane
        QMessageBox::warning(this, "Warning", "You must select profile to continue!");
    }
}

void ProfileListSelection::on_Add_clicked() //+ button
{
    AddNewProfileDialog* dialog = new AddNewProfileDialog(this, &listOfProfilesFromDb);
    int returnValue = dialog->exec();
    if(returnValue){
        profileDataCapsule temp = dialog->getNewProfileData();
        if(db->addToDataBase(temp.name, temp.pin, temp.profileType, temp.accnum)){
            this->listOfProfilesFromDb.clear();
            ui->profileList->clear(); //usuniecie profilow z listy
            db->queryProfiles(&this->listOfProfilesFromDb); // zaladowanie profilow z bazy z nowymi
            loadList(); //zaladowanie listy profilow do ui
        }


    }else{
        qDebug() << "canceled";
    }

}

void ProfileListSelection::on_Delete_clicked()
{
    //usuniecie zaznaczonego profilu, przekazanie informacji o nazwie profilu
    if(ui->profileList->selectedItems().size() != 0){ //sprawdzenie czy zostal zaznaczony jakikolwiek profil
        QList<QListWidgetItem*> selected = ui->profileList->selectedItems();
        QString selectedOne = selected[0]->text(); //wybrany moze byc tylko jeden
        db->deleteSelectedProfile(selectedOne);
        ui->profileList->takeItem(ui->profileList->currentRow());

        //usuniecie z kontenera
        QList<QString>::iterator iteration;
        for(iteration = listOfProfilesFromDb.begin(); iteration != listOfProfilesFromDb.end();){
            if(*iteration == selectedOne){
               iteration = this->listOfProfilesFromDb.erase(iteration);
            }else{
                ++iteration;
            }
        }

    }
    else{//jesli nic nie zostało wybrane
        QMessageBox::warning(this, "Warning", "You must select profile that you want to delete!");
    }
}

QString ProfileListSelection::getSelectedProfile()
{
    return selectedProfile;
}

QList<QString> *ProfileListSelection::getPtrListOfProfilesFromDb()
{
    return &listOfProfilesFromDb;
}

