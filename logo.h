#ifndef LOGO_H
#define LOGO_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

class Logo : public QWidget
{
    Q_OBJECT
public:
    explicit Logo(QWidget *parent = 0);

signals:

public slots:

private:

    QLabel *picture;
    QVBoxLayout *mainLayout;

};

#endif // LOGO_H
