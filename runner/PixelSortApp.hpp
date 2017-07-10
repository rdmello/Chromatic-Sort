
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

// Magick++ includes
#include <Magick++.h>

// PixelSort includes
#include "PixelSortOptions.hpp"

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
    QPushButton exportbutton;
    QFileDialog fileexport;
    
    QFormLayout formlayout;
    QPushButton sortbutton;
      
    QPushButton quitbutton;
        
    QMenu fileMenu; 
    
    // Image and PixelSort resources
    Magick::Image img;
    PixelSortOptions opts;

public slots:
    void reloadImage(QString);
    void writeImage(QString);
    void updateScene(QPixmap&);
    void sortButtonAction();
};


