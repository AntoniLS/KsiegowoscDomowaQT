#include<QString>
#include <QTableView>
#include "databasemanagment.h"

#ifndef DATA_H
#define DATA_H

/** struktura zawiera dane na temat aktuanego stanu progrmau */
struct CurrentOptions{
    bool income;
    bool outcome;
    bool onlyProfOf;
    QString onlyProfOfAcc;
    bool combined;
    QList<QString> combinedList; ///< lista zawierająca tymczasową liste wybranych profilów w opcji "combine profiles"
};
/** Klasa bazowa odpowiadająca zalogowanemu profilowi typu "child" z ograniczonymi możliwościami*/
class BaseProfile{
    QTableView* ptv; ///< wskaźnik na tabelę zawierającą transakcje
    QString name; ///< nazwa zalogowanego profilu
    QString accnumber; ///< numer konta zalowanego profilu

public:
    BaseProfile(QString prof, QString acc, QTableView* ptr) : name(prof), accnumber(acc), ptv(ptr) {};
    /** Getter */
    QString getAccnumber();
    /** Getter */
    QString getName();
    /** Getter */
    QTableView* getPtv();
    /**
    *   Metoda filturje tablelę pokazując jedynie wydatki
    */
    void showOnlyOutcome(CurrentOptions*, int*);
    /**
    *   Metoda filturje tablelę pokazując jedynie dochód
    */
    void showOnlyIncome(CurrentOptions*, int*);
    /**
    *   Metoda filturje tablelę pokazując wszystkie transakcje (resetuje opcje)
    */
    void showEverything(CurrentOptions*);
    /**
    *   Metoda eksportująca transakcje do pliku csv i txt
    */
    bool exportData(QTableView*);
    /**
    *   Metoda sortuje tablelę pokazując transakcje w kolejności rosnącej
    */
    void sort();
    //metody wirtualne
    virtual ~BaseProfile() {};
    /**
    *   Metoda wczytująca wszystkie rachunki
    */
    virtual void loadAllBills(QTableView*, DataBaseManagment*);
    /**
    *   Metoda zwraca typ profilu
    */
    virtual int retPrivillageLevel();
    /**
    *   Metoda filtrująca tablelę pokazując jedynie wydatki konkretnego profilu z listy powiązanych z przechowywanymi transakcjami
    */
    virtual bool selectOnlyOneProfile(DataBaseManagment* dataBase,CurrentOptions* copt);
    /**
    *   Metoda filtrująca tablelę pokazując jedynie wydatki wybranych profilów z listy powiązanych z przechowywanymi transakcjami
    */
    virtual bool combineProfiles(DataBaseManagment* dataBase, CurrentOptions* copt);
    /**
    *   Metoda dodająca nową transakcje wprowadzoną przez użytkownika (dla obiektu klasy bazowej jedynie o tym samym numerze konta)
    */
    virtual void addNewTransaction(DataBaseManagment* dataBase, CurrentOptions* copt);
    /**
    *   Metoda generująca wykresy, ułatwiające analizę transakcji poprzez ich graficzną reprezentację
    */
    virtual void generateChart(QTableView*);
    // dodatkowe metody zmniejszające ilość kodu
    void showAll();

};
/** Klasa pochodna odpowiadająca zalogowanemu profilowi typu "parent" z pełnymi możliwościami*/
class HighPrivillageProfile : public BaseProfile{

public:
    HighPrivillageProfile(QString prof, QString acc, QTableView* ptr): BaseProfile(prof,acc,ptr) {};
    //metody wirtualne
    virtual ~HighPrivillageProfile() {};
    /**
    *   Metoda wczytująca wszystkie rachunki
    */
    virtual void loadAllBills(QTableView*, DataBaseManagment*);
    /**
    *   Metoda zwraca typ profilu
    */
    virtual int retPrivillageLevel();
    /**
    *   Metoda filtrująca tablelę pokazując jedynie wydatki konkretnego profilu z listy powiązanych z przechowywanymi transakcjami
    */
    virtual bool selectOnlyOneProfile(DataBaseManagment* dataBase,CurrentOptions* copt);
    /**
    *   Metoda filtrująca tablelę pokazując jedynie wydatki wybranych profilów z listy powiązanych z przechowywanymi transakcjami
    */
    virtual bool combineProfiles(DataBaseManagment* dataBase, CurrentOptions* copt);
    /**
    *   Metoda dodająca nową transakcje wprowadzoną przez użytkownika
    */
    virtual void addNewTransaction(DataBaseManagment* dataBase, CurrentOptions* copt);
    /**
    *   Metoda generująca wykresy, ułatwiające analizę transakcji poprzez ich graficzną reprezentację (dla obiektu klasy pochodnej istnieje możliwośc
    *   wyboru pomiędzy wygenerowaniem wykresu jedynie dla zalogowanego profilu lub dla wszystkich transakcji znajdujących się w bazie danych)
    */
    virtual void generateChart(QTableView*);
};

struct Data
{
    BaseProfile * prof; ///< Wskażnik na obiekt typu klasy bazowej
};


#endif // DATA_H
