#include "chartsgenerator.h"
#include "ui_chartsgenerator.h"
#include <QtCharts>
#include <QChartView>
#include <QPair>
#include <QList>



ChartsGenerator::ChartsGenerator(QWidget *parent, QTableView* ptv, int opt, QString acc) :
    QDialog(parent), tmpTV(ptv), opt(opt), accnum(acc),
    ui(new Ui::ChartsGenerator)
{
    ui->setupUi(this);
    generateChart(opt);
}

ChartsGenerator::~ChartsGenerator()
{
    delete ui;
}

void ChartsGenerator::generateChart(int option)
{
    if(!option){ //0 - dla wszystkich transakcji zawartych w tabeli
        generatePie();
        generateLine();
    }else{ //1 - dla transakcji konkretnego numeru konta (zalogowanego profilu)
        qDebug()<<"Parent only my";
        generatePieForOne();
        generateLineForOne();
    }
}

void ChartsGenerator::generatePie()
{
    //Zbieranie danych dotyczących kategorii
    QList<QString> avCategories;
    for(int i = 0; i < tmpTV->model()->rowCount(); i++){
        if(!avCategories.contains(tmpTV->model()->index(i,4).data().toString())) //jesli nie ma takiej kategorii dodaje ją
            avCategories.append(tmpTV->model()->index(i,4).data().toString());
    }
    //Przeliczanie wydatków w konkretnej kategorii
    QVector<QPair<QString, double>> spendingsInCateogry;
    for(int i = 0; i < avCategories.length(); i++){
        const auto category = avCategories[i];
        spendingsInCateogry.append(QPair<QString, double>(category, 0.0));
        for(int j = 0; j < tmpTV->model()->rowCount(); j++){
            if(tmpTV->model()->index(j,4).data().toString() == category && tmpTV->model()->index(j,0).data().toDouble() < 0){
                spendingsInCateogry[i].second += tmpTV->model()->index(j,0).data().toDouble();
            }
        }
    }
    //Obliczanie wartości procentowych
    double full = 0;
    for(const auto& catSp : spendingsInCateogry)
        full += catSp.second;
    QVector<QPair<QString, double>> catWithPercentge;
    for(const auto& catSp : spendingsInCateogry)
        catWithPercentge.append(QPair<QString,double>(catSp.first,(catSp.second/full)));

    //Ustawianie wykresu kołowego
    QPieSeries * cateogrySeries = new QPieSeries(this);
    for(const auto& series : catWithPercentge){
        cateogrySeries->append(series.first, series.second);
    }
    QChart* pieChart = new QChart();
    pieChart->addSeries(cateogrySeries);
    pieChart->setTitle("Spendings by category");

    QChartView* pieChartView = new QChartView(pieChart, this);
    pieChartView->setRenderHint(QPainter::Antialiasing);
    ui->horizontalLayout->addWidget(pieChartView);


}

void ChartsGenerator::generateLine()
{
    tmpTV->sortByColumn(2,Qt::AscendingOrder);

    QLineSeries *series = new QLineSeries(this);
    for(int j = 0; j < tmpTV->model()->rowCount(); j++){
        if(tmpTV->model()->index(j,0).data().toDouble() < 0){
            QString tmp = tmpTV->model()->index(j,2).data().toString();
            QStringList values = tmp.split(QLatin1Char('-'), Qt::SkipEmptyParts);
            qDebug() << values[0]<<" "<<values[1]<<" "<<values[2];
            QDateTime someMoment;
            someMoment.setDate(QDate(values[0].toInt(),values[1].toInt(),values[2].toInt()));
            series->append(someMoment.toMSecsSinceEpoch(), abs(tmpTV->model()->index(j,0).data().toDouble()));
        }
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setTitle("Spendings over time");

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(5);
    axisX->setFormat("d-MM-yy");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Money spent");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->horizontalLayout->addWidget(chartView);
}

void ChartsGenerator::generatePieForOne()
{
    qDebug() << accnum;
    QList<QString> avCategories;
    for(int i = 0; i < tmpTV->model()->rowCount(); i++){
        if(tmpTV->model()->index(i,1).data().toString() == accnum)
            qDebug()<<"ok";
        if(!avCategories.contains(tmpTV->model()->index(i,4).data().toString()) && tmpTV->model()->index(i,1).data().toString() == accnum) //if it doesnt contains such category already, add it.
            avCategories.append(tmpTV->model()->index(i,4).data().toString());
    }
    //Zbieranie danych dotyczących kategorii
    QVector<QPair<QString, double>> spendingsInCateogry;
    for(int i = 0; i < avCategories.length(); i++){
        const auto category = avCategories[i];
        spendingsInCateogry.append(QPair<QString, double>(category, 0.0));
        for(int j = 0; j < tmpTV->model()->rowCount(); j++){
            if(tmpTV->model()->index(j,4).data().toString() == category && tmpTV->model()->index(j,0).data().toDouble() < 0  && tmpTV->model()->index(j,1).data().toString() == accnum){
                spendingsInCateogry[i].second += tmpTV->model()->index(j,0).data().toDouble();
            }
        }
    }
    //Obliczanie wartości procentowych
    double full = 0;
    for(const auto& catSp : spendingsInCateogry)
        full += catSp.second;
    QVector<QPair<QString, double>> catWithPercentge;
    for(const auto& catSp : spendingsInCateogry)
        catWithPercentge.append(QPair<QString,double>(catSp.first,(catSp.second/full)));

    //Ustawianie wykresu kołowego
    QPieSeries * cateogrySeries = new QPieSeries(this);
    for(const auto& series : catWithPercentge){
        cateogrySeries->append(series.first, series.second);
    }
    QChart* pieChart = new QChart();
    pieChart->addSeries(cateogrySeries);
    pieChart->setTitle("Spendings by category");

    QChartView* pieChartView = new QChartView(pieChart, this);
    pieChartView->setRenderHint(QPainter::Antialiasing);
    ui->horizontalLayout->addWidget(pieChartView);

}

void ChartsGenerator::generateLineForOne()
{
    tmpTV->sortByColumn(2,Qt::AscendingOrder);

    QLineSeries *series = new QLineSeries(this);
    for(int j = 0; j < tmpTV->model()->rowCount(); j++){
        if(tmpTV->model()->index(j,0).data().toDouble() < 0 && tmpTV->model()->index(j,1).data().toString() == accnum){
            QString tmp = tmpTV->model()->index(j,2).data().toString();
            QStringList values = tmp.split(QLatin1Char('-'), Qt::SkipEmptyParts);
            qDebug() << values[0]<<" "<<values[1]<<" "<<values[2];
            QDateTime someMoment;
            someMoment.setDate(QDate(values[0].toInt(),values[1].toInt(),values[2].toInt()));
            series->append(someMoment.toMSecsSinceEpoch(), abs(tmpTV->model()->index(j,0).data().toDouble()));
        }
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setTitle("Spendings over time");

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(5);
    axisX->setFormat("d-MM-yy");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Money spent");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->horizontalLayout->addWidget(chartView);
}


