
#include <QMainWindow>
#include <QApplication>
#include <QDockWidget>
#include <QMenuBar>
#include <QMenu>
#include <QFormLayout>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>

#include <QFileDialog>
#include <QPushButton>


class PixelSortApp : public QMainWindow
{
    Q_OBJECT

public:
    PixelSortApp();

private:
    QString imageFilePath;
    QGraphicsScene scene;
    QPixmap pixmap;
    QGraphicsView view;
    QDockWidget dockwidget;
    QWidget dockwidget_mid;
    
    QVBoxLayout vbox;
    QPushButton importbutton;
    QFileDialog fileimport;
    
    QFormLayout formlayout;
    QPushButton importbutton2;
      
    QPushButton quitbutton;
        
    QMenu fileMenu; 
};


