#ifndef EXECUTEQUERY_H
#define EXECUTEQUERY_H

#include <QWidget>
#include <QSqlDatabase>

class ExecuteQuery : public QWidget
{
    Q_OBJECT
public:
    explicit ExecuteQuery(QSqlDatabase &db, int rows, QWidget *parent = 0);

signals:

private slots:


public slots:
private:
    QSqlDatabase &db;

};

#endif // EXECUTEQUERY_H
