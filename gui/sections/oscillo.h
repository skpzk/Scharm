#ifndef SECTIONS_OSCILLO_H
#define SECTIONS_OSCILLO_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QBoxLayout>

// class CustomView : public QGraphicsView{
//     Q_OBJECT
//     public:
//         CustomView(QGraphicsScene *scene);
//     protected:
//         virtual int heightForWidth(int) const;
// };

class AspectRatioWidget : public QWidget
{
public:
    AspectRatioWidget(QWidget *widget, float width, float height, QWidget *parent = 0);
    void resizeEvent(QResizeEvent *event);

private:
    QBoxLayout *layout;
    float arWidth; // aspect ratio width
    float arHeight; // aspect ratio height
};

class OscilloSection : public QWidget{
    
    Q_OBJECT
    public:
        OscilloSection(QWidget *parent = 0);
        
        // QWidget *box;
        // QVBoxLayout *vbox;

        static void receiveSignal(void * os, std::vector<float> signal);

        void checkState();
        void resizeEvent(QResizeEvent *event);
        void paintEvent(QPaintEvent *event);
    private:
        QGraphicsScene *scene;
        // CustomView *view;
        QGraphicsView *view;

        void plotAxes();
        std::vector<float> signal;
        void plotSignal();

        std::string currentInput;
        


    protected:
            // virtual void paintEvent(QPaintEvent*) override;
            // void resizeEvent(QResizeEvent* event);
    
    public slots:
        void repaintSlot();
        void signalSelection(QString);

    signals:
        void repaintDisplay();
        

};


#endif /* SECTIONS_OSCILLO_H */
