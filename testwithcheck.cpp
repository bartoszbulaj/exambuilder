#include "testwithcheck.h"

#include <QSqlQuery>
#include <QTime>

TestWithCheck::TestWithCheck(QSqlDatabase &db, int setIloscPytan, QString wybranaKategoria, QWidget *parent) :
    QWidget(parent), db(db), myKategoria(wybranaKategoria), iloscPytan(setIloscPytan)
{
    QTime time = QTime::currentTime(); // sadzenie ziarna generatora w zaleznosci od czasu
    qsrand((uint)time.msec()); // jw


    mainLayout = new QHBoxLayout;

    layoutRadioButtons = new QGridLayout;
    layoutButtons = new QGridLayout;
    layoutRightBottomLabels = new QHBoxLayout;

    layoutRightSide = new QVBoxLayout;


    txt = new QTextEdit("Powodzenia!");
    txt->setAlignment(Qt::AlignCenter);
    txt->setReadOnly(true);

    labelA = new QLabel("A:");
    labelB = new QLabel("B:");
    labelC = new QLabel("C:");
    labelD = new QLabel("D:");
    radioA = new QRadioButton;
    radioB = new QRadioButton;
    radioC = new QRadioButton;
    radioD = new QRadioButton;
    radioA->setDisabled(true);
    radioB->setDisabled(true);
    radioC->setDisabled(true);
    radioD->setDisabled(true);

    labelPytanie = new QLabel("Pytanie");
    labelNrAktualnegoPytania = new QLabel("x/x");
    labelPytanie->sizeHint();
    labelNrAktualnegoPytania->sizeHint();

    // inicjalizacja buttonow sterowania pytaniami, podpiecie slotow
    buttonRozpocznij = new QPushButton("Rozpocznij test!");
    connect(buttonRozpocznij,SIGNAL(clicked()),this,SLOT(rozpocznij()));
    buttonPoprzednie = new QPushButton("Poprzednie\npytanie");
    connect(buttonPoprzednie,SIGNAL(clicked()),this,SLOT(poprzednie()));
    buttonNastepne = new QPushButton("Następne\npytanie");
    connect(buttonNastepne,SIGNAL(clicked()),this,SLOT(nastepne()));
    buttonZakonczTest = new QPushButton("Zakończ\ntest");
    connect(buttonZakonczTest,SIGNAL(clicked()),this,SLOT(zakoncz()));
    // wylaczenie buttonow sterowania pytaniami
    buttonNastepne->setDisabled(true);
    buttonPoprzednie->setDisabled(true);
    buttonZakonczTest->setDisabled(true);


    myStep = 0;

    labelNrAktualnegoPytania->setText( QString::number(myStep) +"/"+ QString::number(iloscPytan));

    punktacja = new QVector<int>(iloscPytan,0);

    query = new QSqlQuery(db);
    getId = new QSqlQuery(db);

    getId->prepare(QString("SELECT id FROM %1").arg(myKategoria)); // zapytanie o ID pytan w bazie
    getId->exec();

    while (getId->next()) //zbiera ID'ki do wektora
    {
        idWszystkichPytan.push_back(getId->value(0).toInt());
    }

    for(int i = 0; i<iloscPytan ; ) // losuje pytania do testu
    {
        randomValue = randInt(0,idWszystkichPytan.size()-1); // losuje  numer pytania z tablicy przechowywujacej ID pytan
        if(!idWybranychPytan.contains(idWszystkichPytan[randomValue]))
        {
            idWybranychPytan.push_back(idWszystkichPytan[randomValue]); // wpisuje indeksy tablicy z ID wybranych pytan
            ++i;
        }
    }

    // layout radiobuttons
    layoutRadioButtons->addWidget(labelA,0,0);
    layoutRadioButtons->addWidget(radioA,0,1);
    layoutRadioButtons->addWidget(labelB,1,0);
    layoutRadioButtons->addWidget(radioB,1,1);
    layoutRadioButtons->addWidget(labelC,2,0);
    layoutRadioButtons->addWidget(radioC,2,1);
    layoutRadioButtons->addWidget(labelD,3,0);
    layoutRadioButtons->addWidget(radioD,3,1);
    layoutRadioButtons->setMargin(40);
    // layout buttons
    layoutButtons->addWidget(buttonRozpocznij,0,0,1,2);
    layoutButtons->addWidget(buttonPoprzednie,1,0);
    layoutButtons->addWidget(buttonNastepne,1,1);
    layoutButtons->addWidget(buttonZakonczTest,2,0,2,2);
    // layout bottom labels
    layoutRightBottomLabels->addWidget(labelPytanie);
    layoutRightBottomLabels->addWidget(labelNrAktualnegoPytania);
    layoutRightBottomLabels->itemAt(1)->setAlignment(Qt::AlignRight);
    layoutRightBottomLabels->setAlignment(Qt::AlignBottom);

    layoutRightSide->addLayout(layoutRadioButtons);
    layoutRightSide->addStretch();
    layoutRightSide->addLayout(layoutButtons);
    layoutRightSide->addLayout(layoutRightBottomLabels);

    mainLayout->addWidget(txt);
    mainLayout->addLayout(layoutRightSide);
    setLayout(mainLayout);

}

int TestWithCheck::randInt(int low, int high)
{
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}
void TestWithCheck::rozpocznij()
{
    myStep++;
    putQuestion();
    labelNrAktualnegoPytania->setText( QString::number(myStep) +"/"+ QString::number(iloscPytan));
    radioA->setEnabled(true);
    radioB->setEnabled(true);
    radioC->setEnabled(true);
    radioD->setEnabled(true);

    buttonNastepne->setEnabled(true);
    buttonPoprzednie->setEnabled(true);
    buttonZakonczTest->setEnabled(true);
    buttonRozpocznij->hide();
}

void TestWithCheck::nastepne()
{
    checkAnswer();
    if(myStep<iloscPytan)
    {
        myStep++;
        putQuestion();
        clearRadioButtons();
        labelNrAktualnegoPytania->setText( QString::number(myStep) +"/"+ QString::number(iloscPytan));
    }
}

void TestWithCheck::poprzednie()
{
    checkAnswer();
    if(myStep>1)
    {
        myStep--;
        putQuestion();
        clearRadioButtons();
        labelNrAktualnegoPytania->setText( QString::number(myStep) +"/"+ QString::number(iloscPytan));
    }
}

void TestWithCheck::zakoncz()
{
    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon(":/icons/computer.png"));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText("Czy chcesz zakończyć test?");
    msgBox.setWindowTitle("Zakończ test");

    QPushButton* buttonTak = msgBox.addButton(QMessageBox::Yes);
    buttonTak->setText("Tak");
    buttonTak->setIcon(QIcon(":/icons/accept.png"));
    QPushButton* buttonNie = msgBox.addButton(QMessageBox::No);
    buttonNie->setText("Nie");
    buttonNie->setIcon(QIcon(":/icons/cancel.png"));

    msgBox.setDefaultButton(buttonNie);
    msgBox.setEscapeButton(buttonNie);

    int r = msgBox.exec();
    if(r == QMessageBox::Yes)
    {
        checkAnswer();
        QMessageBox wynikBox;
        wynikBox.setWindowIcon(QIcon(":/icons/computer.png"));
        wynikBox.setText("Ilość zdobytych punktów: " + QString::number(punktacja->count(1)) + " na " + QString::number(iloscPytan) +" możliwych.");
        wynikBox.setWindowTitle("Twój wynik");
        wynikBox.exec();
        txt->setPlainText("Chcesz rozwiązać kolejny test? W menu \"Edycja\" kliknij \"Cofnij\".");
        txt->setAlignment(Qt::AlignCenter);

        buttonNastepne->hide();
        buttonPoprzednie->hide();
        buttonZakonczTest->hide();

        labelPytanie->hide();
        labelNrAktualnegoPytania->hide();

        radioA->hide();
        radioB->hide();
        radioC->hide();
        radioD->hide();
        labelA->hide();
        labelB->hide();
        labelC->hide();
        labelD->hide();
    }

}

void TestWithCheck::putQuestion()
{
    query->prepare(QString("SELECT pytanie, odp_a, odp_b, odp_c, odp_d, good_odp FROM %1 WHERE id = %2").arg(myKategoria).arg(QString::number(idWybranychPytan[myStep-1])));
    query->exec();
    QString result;

    while(query->next())
    {
        result += "Pytanie " + QString::number(myStep) + ":\n\n";         // numeruje pytania
        result += query->value(0).toString() + "\n\n";                  // dodaje tresc pytania
        result += "A: " + query->value(1).toString()+ "\n" + "B: " + query->value(2).toString()+ "\n";
        result += "C: " + query->value(3).toString()+ "\n" + "D: " + query->value(4).toString() + "\n\n";
    }
    txt->setPlainText(result);
}

void TestWithCheck::clearRadioButtons()
{
    radioA->setAutoExclusive(false);
    radioA->setChecked(false);
    radioA->setAutoExclusive(true);

    radioB->setAutoExclusive(false);
    radioB->setChecked(false);
    radioB->setAutoExclusive(true);

    radioC->setAutoExclusive(false);
    radioC->setChecked(false);
    radioC->setAutoExclusive(true);

    radioD->setAutoExclusive(false);
    radioD->setChecked(false);
    radioD->setAutoExclusive(true);
}

void TestWithCheck::checkAnswer()
{
    if(myStep>0){
        // id jest wybierane z tablicy po indeksie[numer pytania-1]
        query->prepare(QString("SELECT good_odp FROM %1 WHERE id =%2").arg(myKategoria).arg(QString::number(idWybranychPytan[myStep-1])));
        query->exec();

        while(query->next())
        {
            if(query->value(0).toString() == "a" && radioA->isChecked()) punktacja->replace(myStep-1, 1); else
                if(query->value(0).toString() == "b" && radioB->isChecked()) punktacja->replace(myStep-1, 1); else
                    if(query->value(0).toString() == "c" && radioC->isChecked()) punktacja->replace(myStep-1, 1); else
                        if(query->value(0).toString() == "d" && radioD->isChecked()) punktacja->replace(myStep-1, 1); else punktacja->replace(myStep-1 , 0);
        }
    }
}
