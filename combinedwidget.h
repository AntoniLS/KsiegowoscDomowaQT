/** @file */
#ifndef COMBINEDWIDGET_H
#define COMBINEDWIDGET_H

#include <QDialog>
#include <QCheckBox>

namespace Ui {
class combinedWidget;
}

/** Klasa pełniąca rolę dialogu dotyczącego kombinacji profilów do wyświetlenia*/
class combinedWidget : public QDialog
{
    Q_OBJECT

public:
    explicit combinedWidget(QWidget *parent = nullptr, QList<QString>*lst = nullptr);
    ~combinedWidget();
    /**
    *   Metoda zwracająca zaznaczone profile
    */
    QList<QString> getCheckedProfiles();

private slots:
    void on_cancelButton_clicked();

    void on_okButton_clicked();
private:
    Ui::combinedWidget *ui;

    QList<QCheckBox*> tmp; ///<lista wskaźników na checkboxy
    QList<QString> checkedProfiles; ///<lista zaznaczonych profilów
};

#endif // COMBINEDWIDGET_H
