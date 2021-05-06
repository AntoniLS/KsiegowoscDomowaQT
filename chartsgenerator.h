/** @file */
#ifndef CHARTSGENERATOR_H
#define CHARTSGENERATOR_H

#include <QDialog>
#include <QTableView>

namespace Ui {
class ChartsGenerator;
}

/** Klasa zajmujaca się generowaniem wykresów wizualizujacych wydatki */
class ChartsGenerator : public QDialog
{
    Q_OBJECT

public:
    explicit ChartsGenerator(QWidget *parent = nullptr, QTableView* ptv = nullptr, int opt = 0, QString acc = "");
    ~ChartsGenerator();
    /**
    *   Metoda wywołująca inne generujące wykresy dla konkretnego typu profilu
    *   @param option -> 0 - Child, 1 - Parent -> everyaccount
    */
    void generateChart(int option);

private:
    Ui::ChartsGenerator *ui;
    int opt; ///< typ profilu
    QString accnum; ///< numer konta
    QTableView* tmpTV; /// wskaźnik na tabele zawierającą model

    /**
    *   Metoda generujaca wykres kołowy dla wszystkich transakcji aktualnie zawartych w modelu na podstawie ich kategorii.
    */
    void generatePie();
    /**
    *   Metoda generujaca wykres liniowy w zależności od czasu dla wszystkich transakcji aktualnie zawartych w modelu.
    */
    void generateLine();
    /**
    *   Metoda generujaca wykres kołowy dla profilu aktualnie zalogowanego w systemie księgowości na podstawie kategorii transakcji.
    */
    void generatePieForOne();
    /**
    *   Metoda generujaca wykres liniowy w zależności od czasu dla aktualnie zalogowanego profilu.
    */
    void generateLineForOne();
};

#endif // CHARTSGENERATOR_H
