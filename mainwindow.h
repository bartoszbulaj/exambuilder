#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QStatusBar>

#include "form.h"
#include "logo.h"
#include "testwithkey.h"
#include "testwithcheck.h"
#include "adminpanel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:

    void nowy();
    void zamknij();
    void wyczysc();
    void pokaz();
    void cofnij();
    void zarzadzaj();


private:
    Logo* ekranPowitalny;
    Form* formMakeExam;
    TestWithKey* myTestWithKey;
    TestWithCheck* myTestWithCheck;
    AdminPanel* myAdminPanel;

    QSqlDatabase db;

    void createMenus();
    void createActions();
    void createStatusBar();

    QMenu *plikMenu;
    QMenu *edycjaMenu;
    QMenu *narzedziaMenu;

    QAction *nowyAction;
    QAction *zamknijAction;
    QAction *wyczyscAction;
    QAction *pokazAction;
    QAction *cofnijAction;
    QAction *zarzadzajAction;

    QLabel *nazwaStatusBar;
    QLabel *infoStatusBar;

    void validateForm();
};


#endif // MAINWINDOW_H
