
#include <QMainWindow>
#include <QApplication>
#include <QDockWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QMenu>
#include <QFormLayout>
#include <QCheckBox>
#include <QLabel>
#include <QSlider>
#include <QDial>
#include <QSpinBox>

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
    QHBoxLayout sortBox;
    QLabel sortLabel;
    QCheckBox rSort;
    QCheckBox gSort;
    QCheckBox bSort;

    /* Move options buttons */
    QWidget moveContainer;
    QHBoxLayout moveBox;
    QLabel moveLabel;
    QCheckBox rMove;
    QCheckBox gMove;
    QCheckBox bMove;

    /* Matcher options sliders */
    QLabel minLabel;
    QSlider rMin;
    QSlider gMin;
    QSlider bMin;
    QLabel maxLabel;
    QSlider rMax;
    QSlider gMax;
    QSlider bMax;

    /* Angle controls */
    QLabel angleLabel;
    QDial angleDial;

    /* Box Size */
    QSpinBox boxW;
    QSpinBox boxH;

    /* Repeat distance */
    QSpinBox distX;
    QSpinBox distY;

    QPushButton sortbutton;
    // QPushButton quitbutton;
        
    QMenu fileMenu; 
    
    // Image and PixelSort resources
    Magick::Image img;
    PixelSortOptions opts;

public slots:
    void reloadImage(QString);
    void writeImage(QString);
    void updateScene(QPixmap&);
    void sortButtonAction();
    void distXset(int);
    void distYset(int);
};


