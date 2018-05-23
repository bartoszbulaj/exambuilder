#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <QDialog>

#include <QLineEdit>
#include <QRadioButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QComboBox>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QGroupBox>
#include "form.h"

class AdminPanel : public QDialog
{
    Q_OBJECT
public:
    explicit AdminPanel(QSqlDatabase &db, Form *formMakeExam, QWidget *parent = 0);

signals:

public slots:

private slots:
    void dodajPytanie();
    void dodajKategorie();
    void pokazWybranaTabele();
    void usunPytanie();
    void usunKategorie();

private:
    QSqlDatabase &db;
    Form *myForm;

    QLabel *labelPytanie;
    QLabel *labelOdpA;
    QLabel *labelOdpB;
    QLabel *labelOdpC;
    QLabel *labelOdpD;
    QLabel *labelPoprawnaOdpowiedz;
    QLabel *labelListaKategorii;
    QLabel *labelUsunPytanie;
    QLabel *labelDodajNowaKategorie;

    QLineEdit *lineEditPytanie;
    QLineEdit *lineEditA;
    QLineEdit *lineEditB;
    QLineEdit *lineEditC;
    QLineEdit *lineEditD;
    QLineEdit *lineEditNazwaTabeli;
    QLineEdit *lineEditNrPytaniaDoUsuniecia;

    QRadioButton *radioA;
    QRadioButton *radioB;
    QRadioButton *radioC;
    QRadioButton *radioD;

    QPushButton *buttonDodajPytanie;
    QPushButton *buttonPokazTabele;
    QPushButton *buttonDodajKategorie;
    QPushButton *buttonUsunPytanie;
    QPushButton *buttonUsunKategorie;

    QComboBox *comboTable;

    //layouty
    QGridLayout *mainLayout;
    QGridLayout *layoutDodajPytanie;
    QGridLayout *layoutUsunKategorie;
    QHBoxLayout *layoutUsunPytanie;
    QHBoxLayout *layoutDodajKategorie;
    //grupy do layoutow
    QGroupBox *groupBoxDodajPytanie;
    QGroupBox *groupBoxUsunPytanie;
    QGroupBox *groupBoxDodajKategorie;
    QGroupBox *groupBoxUsunKategorie;

    QSqlTableModel *model;

    //tworzy widgety i je ustawia
    void createWidgets();

    QString radioSelected();
    void wyczyscPola();
    void clearRadioButtons();

    void fillComboTable();
    void clearComboTable();
};

#endif // ADMINPANEL_H
