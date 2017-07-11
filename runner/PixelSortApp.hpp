
#include <QMainWindow>
#include <QApplication>
#include <QDockWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QMenu>
#include <QFormLayout>
#include <QCheckBox>
#include <QLabel>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include <QColor>
#include <QGraphicsPixmapItem>

#include <QFileDialog>
#include <QPushButton>

// Magick++ includes
#include <Magick++.h>

// PixelSort includes
#include "PixelSortOptions.hpp"

class PixelSortApp : public QMainWindow, public Notifiable
{
    Q_OBJECT

public:
    PixelSortApp(QApplication* appPtr);
    void notify(const char*);

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

    /* Sort options buttons */
    QWidget sortContainer;
    QVBoxLayout sortBox;
    QLabel sortLabel;
    QCheckBox rSort;
    QCheckBox gSort;
    QCheckBox bSort;

    QWidget moveContainer;
    QVBoxLayout moveBox;
    QLabel moveLabel;
    QCheckBox rMove;
    QCheckBox gMove;
    QCheckBox bMove;

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


