#ifndef FORM_H
#define FORM_H

#include "executequery.h"


#include <QWidget>
#include <QtSql>

#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QSpacerItem>
#include <QLineEdit>
#include <QSqlDatabase>
#include <QComboBox>
#include <QGridLayout>

#include <QVBoxLayout>
#include <QHBoxLayout>


class Form : public QWidget
{
    Q_OBJECT
public:
    explicit Form(QSqlDatabase &db, QWidget *parent = 0);

    int getIloscPytan();
    int getIloscWersji();
    QString getNazwaTestu();
    QString getComboKategoria();

    bool nazwaIsEmpty();

    void fillComboKategoria();
    void clearComboKategoria();

signals:


public slots:
    void wyczyscForm();

private slots:

private:
    QSqlDatabase &db;

    ExecuteQuery *myquery;

    QLabel *labeliloscPytan;
    QSpinBox *spinIloscPytan;

    QComboBox *comboKategoria;
    QLabel *labelKategoria;

    QLabel *labelNazwa;
    QLineEdit *lineEditNazwa;

    QLabel *labelIloscWersji;
    QSpinBox *spinIloscWersji;

    QGridLayout *myLayout;

};

#endif // FORM_H
