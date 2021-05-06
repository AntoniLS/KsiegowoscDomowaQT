#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "databasemanagment.h"
#include "profilelistselection.h"
#include "data.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/** Klasa będąca głównym oknem programu */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_incomeButton_clicked();

    void on_outcomeButton_clicked();

    void on_showEverythingButton_clicked();

    void on_sortButton_clicked();

    void on_profileButton_clicked();

    void on_combineButton_clicked();

    void on_plusButton_clicked();

    void on_minusButton_clicked();

    void on_exportButton_clicked();

    void on_chartButton_clicked();

private:
    Ui::MainWindow *ui;
    DataBaseManagment db; ///< obiekt klasy zarządzajacej bazą danych
    CurrentOptions copt; ///< aktualnie wybrane opcje programu
    Data data; ///< obiekt zarządzający przebiegiem programu, pod zarządzania oraz wyświetlania dancyh

    /**
    *   metoda ułatwiająca kontrolę poprawności programu.
    */
    void consolelog(const char* log);
    /**
    *   metoda wywołująca okno wyboru profilu.
    */
    void completeProfileSelection();
    /**
    *   metoda ustawiająca wygląd oraz dodatki w ui.
    */
    void profileListSelectionSetUpUI(ProfileListSelection*);
    /**
    *   metoda ustawiająca opcje programu, tabelę oraz jej zawartość.
    */
    void setUpMainTableView();
    /**
    *   metoda tworzy dynamicznie obiekt odpowiedniej klasy zarządzającej przebiegiem programu w zależności od typu profilu.
    */
    void typeOfProfileDecission(profileDataCapsuleLite&);
    /**
    *   ulepszenie ui zaraz po wyborze profilu.
    */
    void uiSmallUpgradeAfterProfileSelection();
    /**
    *   aktualizacja nazw kolumn, w celu łatwiejszej analizy danych.
    */
    void updateColumnNames();
    /**
    *   zmiana wyglądu włączonego przycisku.
    */
    void buttonPressedVisuals(QPushButton*, bool);
    /**
    *   podstawowe ustawnie potrzebych elementów w ui.
    */
    void setupBaseUi();


};
#endif // MAINWINDOW_H
