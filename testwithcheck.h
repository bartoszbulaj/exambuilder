#ifndef TESTWITHCHECK_H
#define TESTWITHCHECK_H

#include <QWidget>
#include <QSqlDatabase>
#include <QRadioButton>
#include <QButtonGroup>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>

#include <QDebug>

class TestWithCheck : public QWidget
{
    Q_OBJECT
public:
    explicit TestWithCheck(QSqlDatabase &db, int setIloscPytan, QString wybranaKategoria, QWidget *parent = 0);

signals:

private slots:
    void nastepne();
    void poprzednie();
    void zakoncz();
    void rozpocznij();

    void putQuestion();
    void clearRadioButtons();
    void checkAnswer();


public slots:
private:


    QHBoxLayout *mainLayout;

    QGridLayout *layoutRadioButtons;
    QGridLayout *layoutButtons;
    QHBoxLayout *layoutRightBottomLabels;

    QVBoxLayout *layoutRightSide;

    QSqlDatabase &db;
    QSqlQuery *query;
    QSqlQuery *getId;

    QString myKategoria;

    QVector<int> idWszystkichPytan;
    QVector<int> idWybranychPytan;
    int randomValue;
    int myStep;
    int iloscPytan;
    QVector<int> *punktacja;

    QTextEdit *txt;

    QLabel *labelA;
    QLabel *labelB;
    QLabel *labelC;
    QLabel *labelD;

    QLabel *labelPytanie;
    QLabel *labelNrAktualnegoPytania;

    QRadioButton *radioA;
    QRadioButton *radioB;
    QRadioButton *radioC;
    QRadioButton *radioD;

    QPushButton *buttonPoprzednie;
    QPushButton *buttonNastepne;
    QPushButton *buttonZakonczTest;
    QPushButton *buttonRozpocznij;

    int randInt(int low, int high);

};

#endif // TESTWITHCHECK_H
