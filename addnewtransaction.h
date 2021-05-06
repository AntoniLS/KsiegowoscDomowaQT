/** @file */
#ifndef ADDNEWTRANSACTION_H
#define ADDNEWTRANSACTION_H

#include <QDialog>
#include "databasemanagment.h"

namespace Ui {
class AddNewTransaction;
}

/** Klasa odpowiadajaca za proces dodawania nowych tranzakcji */
class AddNewTransaction : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewTransaction(QWidget *parent = nullptr, int priv=1, QString accnum = "none", DataBaseManagment* db = nullptr);
    ~AddNewTransaction();
    /**
    * @return metoda zwraca nowo dodaną transakcje
    */
    QList<QString>* getTransaction();
private slots:

    void on_okButton_clicked();

    void on_cancelButton_clicked();

    void on_imortCSVButton_clicked();

private:
    Ui::AddNewTransaction *ui;
    int priv; ///< typ profilu
    DataBaseManagment* databaseptr; ///< wskaznik do obiektu zarządzajacego bazą danych
    QString accnum; ///< numer konta
    QList<QString> transaction; ///< nowo dadana transakcja

    /**
    *   Tranakcja z dla konta z większymi uprawnieniami
    *   @return zwraca status zgodności podawanej transakci z oczekiwanym wzorcem
    */
    bool higherPrivillageTransaction();
    /**
    *   Tranakcja z dla konta z mniejszymi uprawnieniami
    *   @return zwraca status zgodności podawanej transakci z oczekiwanym wzorcem
    */
    bool lowerPrivillageTransaction();
    /**
    *   Metoda generująca MessageBox z informacja o powodzie błędu
    */
    void wrongMessage(const char *);
    /**
    *   Metoda importujaca transakcje z pliku csv do bazy danych (dla profilu z mniejszymi uprawnieniami -> dla konkretnego numeru konta)
    */
    void lowerPrivillageImport(QString&);
    /**
    *   Metoda importujaca transakcje z pliku csv do bazy danych (dla profilu z większymi uprawnieniami)
    */
    void higherPrivillageImport(QString&);

};

#endif // ADDNEWTRANSACTION_H
