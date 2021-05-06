/** @file */
#ifndef ADDNEWPROFILEDIALOG_H
#define ADDNEWPROFILEDIALOG_H

#include <QDialog>
#include "additionalTypes.h"
namespace Ui {
class AddNewProfileDialog;
}

/** Dialog dodawania nowego profilu */
class AddNewProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewProfileDialog(QWidget *parent = nullptr, QList<QString>* ptrList = nullptr);
    ~AddNewProfileDialog();
    /**
    * Metoda pobierajaca nowowprowadzone dane z formularza
    * @return Dane dotyczące nowego profilu
    */
    profileDataCapsule getNewProfileData();

private slots:
    void on_CancelButton_clicked();
    void on_SaveButton_clicked();

private:
    Ui::AddNewProfileDialog *ui;
    //dane dotyczące nowego profilu
    QString newProfileName;
    QString newPin;
    QString newAccNumber;
    int newProfileType;
    QList<QString>* ptrProfileList; //<lista profili wczytanych z bazy danych
    /**
    *  Metoda sprawdzajaca wypełnienie komórek w formularzu
    */
    void checkIfNotEmpty();
};

#endif // ADDNEWPROFILEDIALOG_H
