#ifndef SECTIONS_OSCILLO_H
#define SECTIONS_OSCILLO_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QBoxLayout>
#include <QComboBox>

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

class CustomComboBox;


class OscilloSection : public QWidget{
    
    Q_OBJECT
    public:
        OscilloSection(QWidget *parent = 0);
        
        // QWidget *box;
        // QVBoxLayout *vbox;

        void receiveSignal(std::vector<float> signal, int);

        void checkState();
        void resizeEvent(QResizeEvent *event);
        void paintEvent(QPaintEvent *event);

        std::vector<std::string> currentInputs;
    private:
        QGraphicsScene *scene;
        // CustomView *view;
        QGraphicsView *view;

        void plotAxes();
        std::vector<std::vector<float>> signalsVector;
        
        void plotSignals();

        

        // QComboBox * cbox;
        std::vector<CustomComboBox *> cboxes;
        // CustomComboBox * cbox2;
        QStringList sources;
        std::vector<std::string> stateKeys;
        // std::vector<std::string> colors;
        


    protected:
            // virtual void paintEvent(QPaintEvent*) override;
            // void resizeEvent(QResizeEvent* event);
    
    public slots:
        void repaintSlot();
        void signalSelection(QString);

    signals:
        void repaintDisplay();
        

};


class CustomComboBox : public QComboBox{
    Q_OBJECT
    public:
        CustomComboBox(QWidget *widget);
        std::string stateKey;
        static void receiveSignal(void * os, std::vector<float> signal);
        int index;
        OscilloSection* os;
        QColor color;
    private:
        // send events to parent to be able to quit by pressing 'q'
        virtual void keyPressEvent(QKeyEvent* e); 
    public slots:
        void signalSelection(QString);
};


#endif /* SECTIONS_OSCILLO_H */
