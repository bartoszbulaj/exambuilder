#include "mainwindow.h"

#include <QCloseEvent>
#include <QPushButton>
#include <QMenuBar>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{

    if(!QFileInfo::exists("MyDataBase.db")){
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("MyDataBase.db");
        db.open();
        QSqlQuery query(db);
        query.prepare("CREATE TABLE kategorie (nazwa VARCHAR(25));");
        query.exec();
        qDebug()<<"stworzylem baze i tabele";
    }else{
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("MyDataBase.db");
        db.open();
        qDebug()<<"baza danych stworzona zostala wczesniej";
    }

    createActions();
    createMenus();
    createStatusBar();

    setWindowIcon(QIcon(":/icons/computer.png"));
    formMakeExam = 0;
    myTestWithKey = 0;
    myTestWithCheck = 0;

    ekranPowitalny = new Logo();
    setCentralWidget(ekranPowitalny);

    setMinimumSize(sizeHint());

}

MainWindow::~MainWindow()
{

}

void MainWindow::createActions() // tworzy akcje do paska menu
{
    nowyAction = new QAction(tr("&Nowy"),this);
    nowyAction->setShortcut(tr("Ctrl+N"));
    nowyAction->setStatusTip(tr("Otwiera formularz do tworzenia testów"));
    nowyAction->setIcon(QIcon(":/icons/page.png"));
    connect(nowyAction,SIGNAL(triggered()),this,SLOT(nowy()));

    zamknijAction = new QAction(tr("&Zamknij"),this);
    zamknijAction->setShortcut(tr("Ctrl+Z"));
    zamknijAction->setStatusTip(tr("Zamyka program"));
    zamknijAction->setIcon(QIcon(":/icons/door_in.png"));
    connect(zamknijAction,SIGNAL(triggered()),this,SLOT(zamknij()));

    wyczyscAction = new QAction(tr("&Wyczyść"),this);
    wyczyscAction->setShortcut(tr("Ctrl+W"));
    wyczyscAction->setStatusTip(tr("Czyści formularz"));
    wyczyscAction->setIcon(QIcon(":/icons/page_delete.png"));
    wyczyscAction->setDisabled(true);
    connect(wyczyscAction,SIGNAL(triggered()),this,SLOT(wyczysc()));

    pokazAction = new QAction(tr("&Pokaż test"),this);
    pokazAction->setShortcut(tr("Ctrl+P"));
    pokazAction->setStatusTip(tr("Wyświetla test według podanych wymagań"));
    pokazAction->setIcon(QIcon(":/icons/pencil_go.png"));
    pokazAction->setDisabled(true);
    connect(pokazAction,SIGNAL(triggered()),this,SLOT(pokaz()));

    cofnijAction = new QAction(tr("&Cofnij"),this);
    cofnijAction->setShortcut(tr("Ctrl+C"));
    cofnijAction->setStatusTip(tr("Wróć do formularza z kryteriami generowania testów"));
    cofnijAction->setIcon(QIcon(":/icons/arrow_undo.png"));
    cofnijAction->setDisabled(true);
    connect(cofnijAction,SIGNAL(triggered()),this,SLOT(cofnij()));

    zarzadzajAction = new QAction(tr("&Zarządzaj"),this);
    zarzadzajAction->setShortcut(tr("Ctrl+X"));
    zarzadzajAction->setStatusTip(tr("Otwórz panel administracyjny programu"));
    zarzadzajAction->setIcon(QIcon(":/icons/database_gear.png"));
    connect(zarzadzajAction,SIGNAL(triggered()),this,SLOT(zarzadzaj()));
}

void MainWindow::createMenus() // tworzy pasek menu
{
    plikMenu = menuBar()->addMenu(tr("&Plik"));
    plikMenu->addAction(nowyAction);
    plikMenu->addAction(zamknijAction);

    edycjaMenu = menuBar()->addMenu(tr("&Edycja"));
    edycjaMenu->addAction(wyczyscAction);
    edycjaMenu->addSeparator();
    edycjaMenu->addAction(pokazAction);
    edycjaMenu->addAction(cofnijAction);

    narzedziaMenu = menuBar()->addMenu(tr("&Narzędzia"));
    narzedziaMenu->addAction(zarzadzajAction);
}

void MainWindow::createStatusBar() // tworzy status bar
{
    statusBar()->setSizeGripEnabled(true);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText("Czy chcesz zamknąć program?");
    msgBox.setWindowTitle("Zamknij");
    msgBox.setWindowIcon(QIcon(":/icons/computer.png"));

    QPushButton* buttonTak = msgBox.addButton(QMessageBox::Yes);
    buttonTak->setText("Tak");
    buttonTak->setIcon(QIcon(":/icons/accept.png"));
    QPushButton* buttonNie = msgBox.addButton(QMessageBox::No);
    buttonNie->setText("Nie");
    buttonNie->setIcon(QIcon(":/icons/cancel.png"));

    msgBox.setDefaultButton(buttonNie);
    msgBox.setEscapeButton(buttonNie);

    int r = msgBox.exec();
    if(r == QMessageBox::Yes) event->accept();
    else event->ignore();
}

void MainWindow::nowy()
{
    if(!formMakeExam && !myTestWithKey && !myTestWithCheck)
    {
        formMakeExam = new Form(db);
        setCentralWidget(formMakeExam);
        delete ekranPowitalny;

        nowyAction->setDisabled(true);
        wyczyscAction->setEnabled(true);
        pokazAction->setEnabled(true);

    }
}

void MainWindow::zamknij()
{
    close();
}

void MainWindow::wyczysc() // czysci formularz
{
    if(formMakeExam) formMakeExam->wyczyscForm();
}

void MainWindow::pokaz() // przechodzi do pokazania testu wg wymagan formularza
{
    if(formMakeExam && !myTestWithKey && !myTestWithCheck)
    {
        validateForm();
    }
}

void MainWindow::cofnij() // slot, powraca do formularza
{
    if(!formMakeExam)
    {
        if(myTestWithKey){
            delete myTestWithKey;
            myTestWithKey = 0;
            cofnijAction->setDisabled(true);
            pokazAction->setEnabled(true);
            wyczyscAction->setEnabled(true);
        }
        if(myTestWithCheck){
            delete myTestWithCheck;
            myTestWithCheck = 0;
            cofnijAction->setDisabled(true);
            pokazAction->setEnabled(true);
            wyczyscAction->setEnabled(true);
        }
        formMakeExam = new Form(db);
        setCentralWidget(formMakeExam);
    }
}

void MainWindow::validateForm() // sprawdza czy wszystkie pola sa wypelnione i przechodzi do wyboru trybu rozwiazywania lub generowania testu
{
    QSqlQuery checkQuestionsNumber;
    checkQuestionsNumber.prepare(QString("SELECT COUNT(*) FROM %1").arg(formMakeExam->getComboKategoria()));
    checkQuestionsNumber.exec();
    while(checkQuestionsNumber.next()){
        if(checkQuestionsNumber.value(0).toInt()>=formMakeExam->getIloscPytan())
        {
            if(formMakeExam->nazwaIsEmpty())
            {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText(QString("Wpisz nazwę testu!"));
                msgBox.setWindowTitle("Błąd");
                msgBox.setWindowIcon(QIcon(":/icons/computer.png"));
                QPushButton* buttonOk = msgBox.addButton(QMessageBox::Ok);
                buttonOk->setText("Wróć");
                msgBox.setDefaultButton(buttonOk);
                msgBox.setEscapeButton(buttonOk);
                msgBox.exec();
            }
            else
            {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Question);
                msgBox.setText("Chcesz utworzyć wybraną liczbę testów, czy rozwiązać jeden?");
                msgBox.setWindowTitle("Wybierz tryb działania");
                msgBox.setWindowIcon(QIcon(":/icons/computer.png"));
                QPushButton *buttonTworzTest = msgBox.addButton(QMessageBox::Yes);
                buttonTworzTest->setText("Tworzenie testu");
                buttonTworzTest->setIcon(QIcon(":/icons/application_form.png"));
                QPushButton *buttonRozwiazTest = msgBox.addButton(QMessageBox::No);
                buttonRozwiazTest->setText("Rozwiązywanie testu");
                buttonRozwiazTest->setIcon(QIcon(":/icons/application_form_edit.png"));
                QPushButton *buttonAnuluj = msgBox.addButton(QMessageBox::Abort);
                buttonAnuluj->setText("Anuluj");
                buttonAnuluj->setIcon(QIcon(":/icons/cancel.png"));
                msgBox.setDefaultButton(buttonAnuluj);
                msgBox.setEscapeButton(buttonAnuluj);



                int r = msgBox.exec();
                if(r == QMessageBox::Yes){
                    myTestWithKey = new TestWithKey(db, formMakeExam->getIloscPytan(), formMakeExam->getNazwaTestu(), formMakeExam->getComboKategoria(), formMakeExam->getIloscWersji());
                    setCentralWidget(myTestWithKey);
                    delete formMakeExam;
                    formMakeExam = 0;

                    cofnijAction->setEnabled(true);
                    pokazAction->setDisabled(true);
                    wyczyscAction->setDisabled(true);
                }
                if(r == QMessageBox::No){
                    myTestWithCheck = new TestWithCheck(db, formMakeExam->getIloscPytan(), formMakeExam->getComboKategoria());
                    setCentralWidget(myTestWithCheck);
                    delete formMakeExam;
                    formMakeExam = 0;

                    cofnijAction->setEnabled(true);
                    pokazAction->setDisabled(true);
                    wyczyscAction->setDisabled(true);
                }
            }


        }else
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(QString("Maksymalna liczba pytań z tej kategorii wynosi %1").arg(checkQuestionsNumber.value(0).toString()));
            msgBox.setWindowTitle("Błąd");
            QPushButton *buttonOk = msgBox.addButton(QMessageBox::Ok);
            buttonOk->setText("Wróć");
            msgBox.setDefaultButton(buttonOk);
            msgBox.setEscapeButton(buttonOk);
            msgBox.exec();
        }
    }
}

void MainWindow::zarzadzaj() // wlacza panelu admina
{
    myAdminPanel = new AdminPanel(db,formMakeExam);
    myAdminPanel->exec();
}
