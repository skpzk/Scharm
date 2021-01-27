#include "window.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "sections/filter.h"
#include "sections/general.h"


Window::Window(QWidget *parent) :
QMainWindow(parent)
{
    printf("Window\n");
    //create filter for events
    this->installEventFilter(this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->addWidget(this->createMainSection());

    QWidget *centralWidget = new QWidget;
    this->setCentralWidget(centralWidget);
    centralWidget->setLayout(layout);

    this->setWindowTitle("Scharm");
    // path = join("gui", "images", "icon.svg")
    // this->setWindowIcon(QIcon(path));
    // this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->setFocus();
}

bool Window::eventFilter(QObject *object, QEvent *ev)
{
    if (ev->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = (QKeyEvent*)ev;

        if(keyEvent->key() == Qt::Key_Q)
        {
            QApplication::quit();
            // std::raise(SIGINT);
        }else{
            cout << "Key pressed : " << keyEvent->text().toStdString() <<"\n";
        }
    }
    return false;
}

QWidget *Window::createMainSection(){
    QWidget *widget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;

    layout->addWidget(this->createTopControls());
    layout->addWidget(this->createBottomControls());

    layout->setStretch(0, 5);
    layout->setStretch(1, 1);

    widget->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    widget->setContentsMargins(0, 0, 0, 0);

    return widget;
}

QWidget *Window::createTopControls(){
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;



    layout->setStretch(0, 5);
    layout->setStretch(1, 1);

    widget->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    widget->setContentsMargins(0, 0, 0, 0);

    return widget;
}

QWidget *Window::createBottomControls(){
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;

    layout->addWidget(createGeneralSection(this));
    layout->addWidget(createFilterSection(this));

    widget->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    widget->setContentsMargins(0, 0, 0, 0);

    return widget;
}
