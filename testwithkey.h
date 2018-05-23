#ifndef TESTWITHKEY_H
#define TESTWITHKEY_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTextEdit>
#include <QHBoxLayout>

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

class TestWithKey : public QWidget
{
    Q_OBJECT
public:
    explicit TestWithKey(QSqlDatabase &db, int setIloscPytan, QString nazwaTestu, QString wybranaKategoria, int setIloscWersji, QWidget *parent = 0);

signals:

private slots:


public slots:
private:
    QSqlDatabase &db;
    QString testName;
    QString myKategoria;
    int iloscPytan;
    int iloscWersji;


    QHBoxLayout *mainLayout;
    QSqlQuery *query;
    QSqlQuery *queryGetIDPytan;

    QTextEdit *myTest;
    QTextEdit *myKey;

    QVector<int> idWszystkichPytan;
    QVector<int> idWybranychPytan;

    QString *resultTest;
    QString *resultKey;


    int *randomValue;

    int randInt(int low, int high);
};

#endif // TESTWITHKEY_H
