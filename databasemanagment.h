/** @file */
#ifndef DATABASEMANAGMENT_H
#define DATABASEMANAGMENT_H

#include <QSqlDatabase>
#include <QList>
#include <QTableView>
#include <QtSql>
#include "additionalTypes.h"

/** Klasa zarządzająca bazą danych, w której przechowywane są dane o profilach jak i transakcjach */
class DataBaseManagment
{
private:
    QSqlDatabase database; ///< Obiekt kontrolujący połączenie z bazą danych
    QSqlTableModel* mod; ///< wskaźnik do modelu danych
    bool connectionStatus;

    /**
    *   Metoda generujaca message box ostrzegający/informujacy w zależności od parametru
    */
    void mbox(bool);
    /**
    *   Metoda wyszukuje bazę danych oraz ustawia połączenie
    */
    void setupDataBase();
    /**
    *   Metoda ułatwiająca kontrolę poprawności danych związanych z bazą
    */
    void dataBaseManagerLog(const char*);
public:
    DataBaseManagment();

    /**
    *   Metoda uzupełaniająca listę profilów wycztując je z bazy danych
    */
    void queryProfiles(QList<QString>*);
    /**
    *   Metoda wykorzystywana w celu dodanie nowego profilu do bazy danych
    */
    bool addToDataBase(QString&, QString&, int, QString&);
    /**
    *   Metoda służąca do sprawdzenia poprawoności pinu
    */
    bool checkingPin(QString&, QString&);
    /**
    *   Metoda wykorzystywana do usunięcia wybranego profilu
    */
    bool deleteSelectedProfile(QString&);
    /**
    *   @returns dane dotyczące profilu (numer konta, typ profilu)
    */
    profileDataCapsuleLite getStartDataOfChosenProfile(QString&);
    /**
    *   Dodanie nowej transakcji pochodzącej z wypełnionego formularza przez użytkownika
    */
    bool addNewPlusTransactionToDataBase(QList<QString>*);
    /**
    *   Dodanie nowych transakcji z pochochodzących z importu z pliku csv
    */
    bool addFromImport(QList<QList<QString>>*);
    /**
    *   Wczytanie rachunków
    */
    bool getAllBills(QTableView*);
    /**
    *   Wczytanie profilów powiązanych z numerami konta znajdujących się w rachunkach
    */
    bool getAvailableProfiles(QList<QString>*);
    /**
    *   Wczytanie rachunków wskazaego numeru konta
    */
    bool getBillsOfSeclectedAccNum(QTableView*,QString& accnum);
    /**
    * @returns lista numerów konta odpowiadającm profilom znajdujących się na liście
    */
    QList<QString> getAccountnumbersContainer(QList<QString>* names);
    /** Getter */
    QSqlTableModel* getModel();
    /** Getter */
    bool permission();
};

#endif // DATABASEMANAGMENT_H
