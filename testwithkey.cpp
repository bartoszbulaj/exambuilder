#include "testwithkey.h"


#include <QPushButton>
#include <QTime>
#include <QDate>

TestWithKey::TestWithKey(QSqlDatabase &db, int setIloscPytan, QString nazwaTestu, QString wybranaKategoria, int setIloscWersji, QWidget *parent) :
    QWidget(parent), db(db), testName(nazwaTestu), myKategoria(wybranaKategoria), iloscPytan(setIloscPytan), iloscWersji(setIloscWersji)
{
    QTime time = QTime::currentTime(); // sadzenie ziarna generatora w zaleznosci od czasu
    qsrand((uint)time.msec()); // jw
// tworzy unikalne nazwy testu i klucza
    QString myTestName = testName;
    myTestName += ".txt";
    QString myKeyName = testName;
    myKeyName += "_key.txt";
// tworzy zmienne plikowe o nazwach wygenerowanych powyzej
    QFile myTestFile;
    myTestFile.setFileName(myTestName);
    QFile myKeyFile;
    myKeyFile.setFileName(myKeyName);

    mainLayout = new QHBoxLayout();

    query = new QSqlQuery(db);


    randomValue = new int;

    resultTest = new QString;
    resultKey = new QString;
    myTest = new QTextEdit();
    myTest->setReadOnly(true);
    myKey = new QTextEdit();
    myKey->setReadOnly(true);

    queryGetIDPytan = new QSqlQuery(db);
    queryGetIDPytan->prepare(QString("SELECT id FROM %1").arg(myKategoria)); // zapytanie o ID pytan w bazie
    queryGetIDPytan->exec();

    while (queryGetIDPytan->next()) //zbiera ID'ki do wektora
    {
        idWszystkichPytan.push_back(queryGetIDPytan->value(0).toInt());
    }

    //po zebraniu wszystkich numerow ID pytan z okreslonej kategorii, zaczyna sie ich losowanie i wrzucanie numerow do wektora

    // poczatek petli po wersjach testu
    for(int nrTestu = 1 ; nrTestu<=iloscWersji; nrTestu++)
    {

        // losuje pytania do testu
        for(int i = 0; i<iloscPytan ; )
        {
            *randomValue = randInt(0,idWszystkichPytan.size()-1); // losuje  numer pytania z tablicy przechowywujacej ID pytan
            if(!idWybranychPytan.contains(idWszystkichPytan[*randomValue]))
            {
                idWybranychPytan.push_back(idWszystkichPytan[*randomValue]); // wpisuje indeksy tablicy z ID wybranych pytan
                ++i;
            }
        }

        // tworzy nazwe testu na podstawie daty
        QString idTestu = QDate::currentDate().toString() + " " + QTime::currentTime().toString();
        *resultTest += "Test: \""+ testName + "\" " + idTestu + " v."+ QString::number(nrTestu) + "\n\n";
        *resultTest += "imię ......................... nazwisko ......................... nr indeksu ............... data .............\n\n";

        *resultKey += "klucz: \"" + testName +"\" "+ idTestu + " v."+ QString::number(nrTestu) + "\n\n";

        // wypisuje pytania  i odpowiedzi do okien wynikowych w formatce
        for(int i = 0; i<idWybranychPytan.size(); ++i)
        {
            query->prepare(QString("SELECT pytanie, odp_a, odp_b, odp_c, odp_d, good_odp FROM %1 WHERE id = %2").arg(myKategoria).arg(QString::number(idWybranychPytan[i])));
            query->exec();
            while (query->next()){
                *resultTest += "Pytanie " + QString::number(i+1) + ":\n" +query->value(0).toString();
                *resultTest += "\r\n";
                *resultTest += "A: " + query->value(1).toString()+ "   " + "B: " + query->value(2).toString()+ "   ";
                *resultTest += "C: " + query->value(3).toString()+ "   " + "D: " + query->value(4).toString() + "\n";

                *resultKey += "Pytanie " + QString::number(i+1) + ": " +query->value(5).toString().toUpper() + "\n";
            }
        }

        //oddziela jedna wersje od kolejnej
        *resultTest += "\n----------------------------------------\n";
        *resultKey += "\n----------------------------------------\n";

        // czysci wektor wylosowanych pytan do danej wersji
        idWybranychPytan.clear();

    } // koniec petli "liczba wersji"

    myTest->setPlainText(*resultTest);
    myKey->setPlainText(*resultKey);

    // tworzy pliki z testem
    if((myTestFile.open(QIODevice::WriteOnly | QIODevice::Text))&&(myKeyFile.open(QIODevice::WriteOnly | QIODevice::Text)))
    {
        QTextStream outStreamTest(&myTestFile);
        outStreamTest << myTest->toPlainText() << endl;
        myTestFile.flush();
        myTestFile.close();

        QTextStream outStreamKey(&myKeyFile);
        outStreamKey << myKey->toPlainText() << endl;
        myKeyFile.flush();
        myKeyFile.close();

        QMessageBox msgBox;
        msgBox.setText("Pliki z pytaniami i odpowiedziami zostały poprawnie zapisane");
        msgBox.setWindowIcon(QIcon(":/icons/computer.png"));
        msgBox.setWindowTitle("Test został utworzony");
        msgBox.setIconPixmap(QPixmap(":/icons/disk.png"));
        QPushButton *buttonOk = msgBox.addButton(QMessageBox::Ok);
        buttonOk->setIcon(QIcon(":/icons/accept.png"));
        buttonOk->setText("Ok");
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Nie mozna utworzyc plików!\n\nBrak możliwości zapisu pytań i odpowiedzi!");
        msgBox.setWindowIcon(QIcon(":/icons/computer.png"));
        msgBox.setWindowTitle("Wystąpił błąd!");
        msgBox.setIconPixmap(QPixmap(":/icons/application_error.png"));
        QPushButton *buttonOk = msgBox.addButton(QMessageBox::Ok);
        buttonOk->setIcon(QIcon(":/icons/cancel.png"));
        buttonOk->setText("Anuluj");
        msgBox.exec();
    }

    mainLayout->addWidget(myTest);
    mainLayout->addWidget(myKey);

    setLayout(mainLayout);

}

int TestWithKey::randInt(int low, int high)
{
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}
