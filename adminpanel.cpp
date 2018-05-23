#include "adminpanel.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QTableView>

AdminPanel::AdminPanel(QSqlDatabase &db, Form *formMakeExam, QWidget *parent) : QDialog(parent) , db(db), myForm(formMakeExam)
{
    createWidgets();
    fillComboTable();
}

void AdminPanel::createWidgets()
{
    labelPytanie = new QLabel("Pytanie:");
    labelOdpA = new QLabel("Odp A:");
    labelOdpB = new QLabel("Odp B:");
    labelOdpC = new QLabel("Odp C:");
    labelOdpD = new QLabel("Odp D:");
    labelPoprawnaOdpowiedz = new QLabel("Wybierz\npoprawną\nodpowiedź");
    labelListaKategorii = new QLabel("Lista kategorii");
    labelListaKategorii->setAlignment(Qt::AlignCenter);
    labelUsunPytanie = new QLabel("Id pytania do usunięcia");
    labelDodajNowaKategorie = new QLabel("Dodaj nową kategorię");

    lineEditA = new QLineEdit();
    lineEditB = new QLineEdit();
    lineEditC = new QLineEdit();
    lineEditD = new QLineEdit();
    lineEditPytanie = new QLineEdit();
    lineEditNazwaTabeli = new QLineEdit();
    lineEditNrPytaniaDoUsuniecia = new QLineEdit();

    radioA = new QRadioButton();
    radioB = new QRadioButton();
    radioC = new QRadioButton();
    radioD = new QRadioButton();

    buttonDodajPytanie = new QPushButton("Dodaj pytanie");
    buttonDodajPytanie->setIcon(QIcon(":/icons/table_row_insert.png"));
    connect(buttonDodajPytanie,SIGNAL(clicked()),this,SLOT(dodajPytanie()));

    buttonPokazTabele = new QPushButton("Pokaż wybraną tabelę");
    buttonPokazTabele->setIcon(QIcon(":/icons/table_go.png"));
    connect(buttonPokazTabele,SIGNAL(clicked()),this,SLOT(pokazWybranaTabele()));

    buttonDodajKategorie = new QPushButton("Dodaj kategorię");
    buttonDodajKategorie->setIcon(QIcon(":/icons/table_add.png"));
    connect(buttonDodajKategorie,SIGNAL(clicked()),this,SLOT(dodajKategorie()));

    buttonUsunPytanie = new QPushButton("Usuń pytanie");
    buttonUsunPytanie->setIcon(QIcon(":/icons/table_row_delete.png"));
    connect(buttonUsunPytanie,SIGNAL(clicked()),this,SLOT(usunPytanie()));

    buttonUsunKategorie = new QPushButton("Usuń kategorię");
    buttonUsunKategorie->setIcon(QIcon(":/icons/table_delete.png"));
    connect(buttonUsunKategorie,SIGNAL(clicked()),this,SLOT(usunKategorie()));

    comboTable = new QComboBox();

    // dodaj pytanie layout
    layoutDodajPytanie = new QGridLayout();
    layoutDodajPytanie->addWidget(labelPytanie,0,0);
    layoutDodajPytanie->addWidget(lineEditPytanie,0,1,1,2);
    layoutDodajPytanie->addWidget(labelPoprawnaOdpowiedz,1,3,4,1);
    layoutDodajPytanie->addWidget(labelOdpA,1,0);
    layoutDodajPytanie->addWidget(lineEditA,1,1);
    layoutDodajPytanie->addWidget(radioA,1,2);
    layoutDodajPytanie->addWidget(labelOdpB,2,0);
    layoutDodajPytanie->addWidget(lineEditB,2,1);
    layoutDodajPytanie->addWidget(radioB,2,2);
    layoutDodajPytanie->addWidget(labelOdpC,3,0);
    layoutDodajPytanie->addWidget(lineEditC,3,1);
    layoutDodajPytanie->addWidget(radioC,3,2);
    layoutDodajPytanie->addWidget(labelOdpD,4,0);
    layoutDodajPytanie->addWidget(lineEditD,4,1);
    layoutDodajPytanie->addWidget(radioD,4,2);
    layoutDodajPytanie->addWidget(buttonDodajPytanie,0,3);

    // usun kategorie layout
    layoutUsunKategorie = new QGridLayout();
    layoutUsunKategorie->addWidget(labelListaKategorii,1,4);
    layoutUsunKategorie->addWidget(comboTable,2,4);
    layoutUsunKategorie->addWidget(buttonPokazTabele,3,4);
    layoutUsunKategorie->addWidget(buttonUsunKategorie,4,4);

    // usun pytanie layout
    layoutUsunPytanie = new QHBoxLayout();
    layoutUsunPytanie->addWidget(labelUsunPytanie);
    layoutUsunPytanie->addWidget(lineEditNrPytaniaDoUsuniecia);
    layoutUsunPytanie->addWidget(buttonUsunPytanie);

    // dodaj kategorie layout
    layoutDodajKategorie = new QHBoxLayout();
    layoutDodajKategorie->addWidget(labelDodajNowaKategorie);
    layoutDodajKategorie->addWidget(lineEditNazwaTabeli);
    layoutDodajKategorie->addWidget(buttonDodajKategorie);

    // group boxy na odpowiednie layouty
    groupBoxDodajPytanie = new QGroupBox("Dodaj pytanie");
    groupBoxDodajPytanie->setLayout(layoutDodajPytanie);
    groupBoxUsunPytanie = new QGroupBox("Usuń pytanie");
    groupBoxUsunPytanie->setLayout(layoutUsunPytanie);
    groupBoxUsunKategorie = new QGroupBox("Pokaż/Usuń kategorię");
    groupBoxUsunKategorie->setLayout(layoutUsunKategorie);
    groupBoxDodajKategorie = new QGroupBox("Dodaj kategorię");
    groupBoxDodajKategorie->setLayout(layoutDodajKategorie);

    // sklej groupboxy/layouty
    mainLayout = new QGridLayout();
    mainLayout->addWidget(groupBoxDodajPytanie,0,0);
    mainLayout->addWidget(groupBoxUsunPytanie,1,0);
    mainLayout->addWidget(groupBoxDodajKategorie,2,0);
    mainLayout->addWidget(groupBoxUsunKategorie,3,0);

    setLayout(mainLayout);
}

void AdminPanel::fillComboTable() // wypelnia combobox kategoriami
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM kategorie");
    query.exec();
    while(query.next())
    {
        comboTable->addItem(query.value(0).toString());
    }

}

void AdminPanel::dodajPytanie() // dodaje pytanie do wybranej kategorii
{
    if(lineEditA->text().size()!=0)
        if(lineEditB->text().size()!=0)
            if(lineEditC->text().size()!=0)
                if(lineEditD->text().size()!=0)
                    if(lineEditPytanie->text().size()!=0)
                    {
                        if(radioA->isChecked() || radioB->isChecked() || radioC->isChecked() || radioD->isChecked())
                        {
                            //sprawdzenie ilosci pytan w tabeli i przypisane nowego najwyzszego id do nowego do wprowadzenia
                            int noweId;
                            int ostatniePytanie = 0;
                            int idTemp=0;
                            QSqlQuery najwiekszeId;
                            najwiekszeId.prepare(QString ("SELECT id from %1").arg(comboTable->currentText()));
                            najwiekszeId.exec();
                            while (najwiekszeId.next()) {
                                idTemp = najwiekszeId.value(0).toInt();
                                if(idTemp>ostatniePytanie) ostatniePytanie = idTemp;
                            }
                            noweId = ostatniePytanie+1;

                            // wprowadzenie pytania do bazy
                            QSqlQuery query(db);
                            query.prepare(QString("INSERT INTO %1 VALUES ('%2', '%3', '%4', '%5', '%6' , '%7', '%8' );").arg(comboTable->currentText()).arg(noweId).arg(lineEditPytanie->text()).arg(lineEditA->text()).arg(lineEditB->text()).arg(lineEditC->text()).arg(lineEditD->text()).arg(radioSelected()));
                            query.exec();
                            wyczyscPola();
                        }else
                        {
                            // wyswietl okno z info o niezaznaczonych radiobuttonach
                            QMessageBox::warning(this,"Błąd","Zaznacz poprawną odpowiedź");
                        }
                    }else
                    {
                        // wyswietl info ze pozostalo niewypelnione ktores pole
                        QMessageBox::warning(this,"Błąd","Wypełnij wszystkie pola.");
                    }
                else
                {
                    QMessageBox::warning(this,"Błąd","Wypełnij wszystkie pola.");
                }
            else
            {
                QMessageBox::warning(this,"Błąd","Wypełnij wszystkie pola.");
            }
        else
        {
            QMessageBox::warning(this,"Błąd","Wypełnij wszystkie pola.");
        }
    else
    {
        QMessageBox::warning(this,"Błąd","Wypełnij wszystkie pola.");
    }

}

QString AdminPanel::radioSelected() // zwraca odpowiedz z zaznaczonego radiobuttona
{
    if(radioA->isChecked()) return "a";
    if(radioB->isChecked()) return "b";
    if(radioC->isChecked()) return "c";
    if(radioD->isChecked()) return "d";
    else return "";
}

void AdminPanel::wyczyscPola() // usuwa dane z pol tekstowych
{
    lineEditA->clear();
    lineEditB->clear();
    lineEditC->clear();
    lineEditD->clear();
    lineEditPytanie->clear();
    clearRadioButtons();
}

void AdminPanel::clearRadioButtons() // odznacza radiobuttony
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

void AdminPanel::clearComboTable() // czysci combobox z lista kategorii
{
    comboTable->clear();
}

void AdminPanel::dodajKategorie() // dodaj kategorie
{
    if(lineEditNazwaTabeli->text().size() != 0){
        QSqlQuery q1(db);
        q1.prepare(QString("CREATE TABLE %1 (id INT IDENTITY(1,1) PRIMARY KEY, pytanie VARCHAR(50), odp_a VARCHAR(50), odp_b VARCHAR(50),odp_c VARCHAR(50),odp_d VARCHAR(50), good_odp VARCHAR(5))").arg(lineEditNazwaTabeli->text()));
        q1.exec();
        QSqlQuery q2(db);
        q2.prepare(QString("INSERT INTO kategorie VALUES ('%1')").arg(lineEditNazwaTabeli->text()));
        q2.exec();

        lineEditNazwaTabeli->clear();
        clearComboTable();
        fillComboTable();
        if(myForm != 0)
        {
            myForm->clearComboKategoria();
            myForm->fillComboKategoria();
        }
    }
}

void AdminPanel::pokazWybranaTabele() // pokazuje okno z wybrana tabela
{
    QDialog *dialog = new QDialog(this);
    model = new QSqlTableModel();
    model->setTable(comboTable->currentText());
    model->select();
    QTableView *view = new QTableView();
    view->setModel(model);
    QHBoxLayout *dialogLayout = new QHBoxLayout();
    dialogLayout->addWidget(view);
    dialogLayout->setMargin(0);
    dialog->setLayout(dialogLayout);
    dialog->show();
}

void AdminPanel::usunPytanie() // usuwa pytanie z wybranej aktualnie kategorii w comboBox, o ile istnieje wybrany numer pytania
{
    if(lineEditNrPytaniaDoUsuniecia->text().size()!=0)
    {
        QSqlQuery q1;
        q1.prepare(QString("SELECT * FROM %1 WHERE id = '%2'").arg(comboTable->currentText()).arg(lineEditNrPytaniaDoUsuniecia->text()));
        q1.exec();
        while(q1.next())
        {
            QMessageBox myMessage;
            myMessage.setIcon(QMessageBox::Question);
            myMessage.setText(QString("Id pytania: %1 \nNazwa kategorii: %2 \n\nUsunąć?").arg(lineEditNrPytaniaDoUsuniecia->text()).arg(comboTable->currentText()));
            myMessage.setWindowTitle("Potwierdź zmiany");
            QPushButton *buttonTak = myMessage.addButton(QMessageBox::Yes);
            buttonTak->setText("Tak");
            QPushButton *buttonNie = myMessage.addButton(QMessageBox::No);
            buttonNie->setText("Nie");

            int r = myMessage.exec();
            if(r == QMessageBox::Yes)
            {
                QSqlQuery q2;
                q2.prepare(QString("DELETE FROM %1 WHERE id = %2 ").arg(comboTable->currentText()).arg(lineEditNrPytaniaDoUsuniecia->text()));
                q2.exec();
            }
        }
        lineEditNrPytaniaDoUsuniecia->clear();
    }
}

void AdminPanel::usunKategorie()
{
    QMessageBox myMessage;
    myMessage.setIcon(QMessageBox::Question);
    myMessage.setText(QString("Nazwa kategorii: %1 \n\nUsunąć?").arg(comboTable->currentText()));
    myMessage.setWindowTitle("Potwierdź zmiany");
    QPushButton *buttonTak = myMessage.addButton(QMessageBox::Yes);
    buttonTak->setText("Tak");
    QPushButton *buttonNie = myMessage.addButton(QMessageBox::No);
    buttonNie->setText("Nie");

    int r = myMessage.exec();
    if(r == QMessageBox::Yes)
    {
        QSqlQuery q1;
        q1.prepare(QString("DROP TABLE %1").arg(comboTable->currentText()));
        q1.exec();
        QSqlQuery q2;
        q2.prepare(QString("DELETE FROM kategorie WHERE nazwa = '%1'").arg(comboTable->currentText()));
        q2.exec();

        if(myForm != 0)
        {
            myForm->clearComboKategoria();
            myForm->fillComboKategoria();
        }
        clearComboTable();
        fillComboTable();
    }
}
