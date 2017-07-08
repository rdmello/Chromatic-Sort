
#include <QMainWindow>
#include <QApplication>
#include <QDockWidget>
#include <QMenuBar>
#include <QMenu>
#include <QFormLayout>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include <QGraphicsPixmapItem>

#include <QFileDialog>
#include <QPushButton>


class PixelSortApp : public QMainWindow
{
    Q_OBJECT

public:
    PixelSortApp(QApplication* appPtr);

private:
    QApplication* appPtr;
    QString imageFilePath;
    QGraphicsScene scene;
    QGraphicsView view;
    QGraphicsPixmapItem* mainImg;
    QDockWidget dockwidget;
    QWidget dockwidget_mid;
    
    QVBoxLayout vbox;
    QPushButton importbutton;
    QFileDialog fileimport;
    
    QFormLayout formlayout;
    QPushButton importbutton2;
      
    QPushButton quitbutton;
        
    QMenu fileMenu; 

public slots:
    void reloadImage(QString);
    void updateScene(QPixmap&);
};


