#include "form.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>

Form::Form(QSqlDatabase &db, QWidget *parent) :
    QWidget(parent), db(db)
{
    //  nazwa testu label, editline
    labelNazwa = new QLabel("Nazwa testu:");
    lineEditNazwa = new QLineEdit();
    lineEditNazwa->setAlignment(Qt::AlignRight);
    lineEditNazwa->setMaxLength(40);

    //  ilość pytań label, spinbox
    labeliloscPytan = new QLabel("Ilość pytań:");
    spinIloscPytan = new QSpinBox();
    spinIloscPytan->setValue(1);
    spinIloscPytan->setMinimum(1);
    spinIloscPytan->setMaximum(99);

    labelKategoria = new QLabel("Kategoria:");
    comboKategoria = new QComboBox();

    labelIloscWersji = new QLabel("Ilość wersji:");
    spinIloscWersji = new QSpinBox();
    spinIloscWersji->setValue(1);
    spinIloscWersji->setMinimum(1);
    spinIloscWersji->setMaximum(99);

    myLayout = new QGridLayout();

    clearComboKategoria();
    fillComboKategoria();

    myLayout->addWidget(labelNazwa,0,0);
    myLayout->addWidget(lineEditNazwa,0,1);
    myLayout->addWidget(labeliloscPytan,1,0);
    myLayout->addWidget(spinIloscPytan,1,1);
    myLayout->addWidget(labelKategoria,2,0);
    myLayout->addWidget(comboKategoria,2,1);
    myLayout->addWidget(labelIloscWersji,3,0);
    myLayout->addWidget(spinIloscWersji,3,1);
    myLayout->setMargin(50);

    setLayout(myLayout);

}

int Form::getIloscPytan()
{
    return spinIloscPytan->value();
}

void Form::wyczyscForm()
{
    lineEditNazwa->clear();
    spinIloscPytan->setValue(0);
    spinIloscWersji->setValue(0);
    clearComboKategoria();
    fillComboKategoria();
}
bool Form::nazwaIsEmpty()
{
    if(lineEditNazwa->text().size() == 0)
    {return true;}
    else return false;

}

QString Form::getNazwaTestu()
{
    return lineEditNazwa->text();
}

void Form::fillComboKategoria()
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM kategorie");
    query.exec();
    while(query.next())
    {
        comboKategoria->addItem(query.value(0).toString());
    }
}

QString Form::getComboKategoria()
{
    return comboKategoria->currentText();
}

void Form::clearComboKategoria() // czysci combobox z lista kategorii
{
    comboKategoria->clear();
}

int Form::getIloscWersji()
{
    return spinIloscWersji->value();
}
