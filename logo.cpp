#include "logo.h"

Logo::Logo(QWidget *parent) :
    QWidget(parent)
{
    mainLayout = new QVBoxLayout();
    mainLayout->setMargin(2);
    picture = new QLabel();
    picture->setPixmap(QPixmap(":/images/logo_new.png")); // przypisuje obraz do QLabel
    picture->setAlignment(Qt::AlignCenter); // wyrównuje widget

    mainLayout->addWidget(picture);

    setLayout(mainLayout);
}
