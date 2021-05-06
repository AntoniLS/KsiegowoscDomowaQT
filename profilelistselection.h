/** @file */
#ifndef PROFILELISTSELECTION_H
#define PROFILELISTSELECTION_H

#include <QDialog>
#include "databasemanagment.h"

namespace Ui {
class ProfileListSelection;
}

/**Klasa pełniąca rolę okna logowania do wybranego profilu*/
class ProfileListSelection : public QDialog // tylko do wyboru
{
    Q_OBJECT

public:
    explicit ProfileListSelection(QWidget *parent = nullptr);
    //overload konstruktora, aby miec dostep do tego samego obiektu menadzera bazy danych co w mainwindow
    explicit ProfileListSelection(QWidget *parent = nullptr, DataBaseManagment* ptrDb = nullptr);
    ~ProfileListSelection();

    /**
    *   metoda zwraca wybrany przez użytkownika profil
    */
    QString getSelectedProfile();
    /**
    *   metoda zwraca wskaźnik na listę profilów z bazy danych
    */
    QList<QString>* getPtrListOfProfilesFromDb();
    /**
    *   metoda ładująca listę profilów
    */
    void loadList();
private slots:
    void on_Continue_clicked();

    void on_Add_clicked();

    void on_Delete_clicked();
private:
    //pola
    Ui::ProfileListSelection *ui;
    DataBaseManagment* db; ///< wskaznik do obiektu zarządzajacego bazą danych
    QString selectedProfile; ///< wybrany profil
    QList<QString> listOfProfilesFromDb; ///< lista wszystkich profilow

};

#endif // PROFILELISTSELECTION_H
