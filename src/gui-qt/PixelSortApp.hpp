
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
#include <QAction>
#include <QGraphicsPixmapItem>

#include <QFileDialog>
#include <QPushButton>
#include <QKeySequence>
#include <QColorDialog>

// PixelSort includes
#include "PixelSortOptions.hpp"
#include "driver/QImageDriver.hpp"

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

    /* Color matcher options dialog */
    QPushButton minColBtn;
    QPushButton maxColBtn;
    QColorDialog minColDialog;
    QColorDialog maxColDialog;

    /* Angle controls */
    QLabel angleLabel;
    QDial angleDial;

    /* Start Point */
    QSpinBox startX;
    QSpinBox startY;

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
    QImage img;
    PS::QImageDriver drv;
    PixelSortOptions opts;

public slots:
    void reloadImage(QString);
    void writeImage(QString);
    void updateScene(QPixmap&);
    void sortButtonAction();
    void distXset(int);
    void distYset(int);
};


