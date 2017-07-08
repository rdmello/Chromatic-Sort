
#include "PixelSortApp.hpp"

PixelSortApp::PixelSortApp(QApplication* parent):
    appPtr(parent),
    scene(this),
    view(&scene, this),
    dockwidget("Tools", this),
    dockwidget_mid(&dockwidget),
    vbox(&dockwidget_mid),
    importbutton("Import", &dockwidget_mid),
    fileimport(this),
    formlayout(),
    importbutton2("Import", &dockwidget_mid),
    quitbutton("Quit", &dockwidget_mid),
    fileMenu("File")
{
    this->resize(800, 500);
    this->setWindowTitle("PixelSort app v2");

    imageFilePath = "/Users/Rylan/Desktop/Projects/Glitch/sorting/cxx_proj/images/expo_out.tiff";
    QPixmap pixmap(imageFilePath);
    mainImg = scene.addPixmap(pixmap);

    this->setCentralWidget(&view);
        
    // set up dockwidget
    this->addDockWidget(Qt::LeftDockWidgetArea, &dockwidget);
    dockwidget.setAllowedAreas(Qt::LeftDockWidgetArea);
    dockwidget.setFeatures(QDockWidget::NoDockWidgetFeatures);
    dockwidget.setWidget(&dockwidget_mid);
        
    // add import button to vbox
    vbox.addWidget(&importbutton);
    
    fileimport.setFileMode(QFileDialog::ExistingFile);
    fileimport.setAcceptMode(QFileDialog::AcceptOpen);
    fileimport.setNameFilter("Images (*.png *.tiff)");
    fileimport.fileSelected(imageFilePath);

    // Connect actual events
    QObject::connect(&importbutton, SIGNAL(clicked()), &fileimport, SLOT(exec()));
    QObject::connect(&fileimport, SIGNAL(fileSelected(QString)), this, SLOT(reloadImage(QString)));
 
    // Set up the form layout widget 
    vbox.addLayout(&formlayout);
    
    // add import button to formlayout
    formlayout.addRow("Import2", &importbutton2);
    QObject::connect(&importbutton2, SIGNAL(clicked()), appPtr, SLOT(quit()));
 
    // add quit button to vbox
    vbox.addWidget(&quitbutton);
    QObject::connect(&quitbutton, SIGNAL(clicked()), appPtr, SLOT(quit()));

    // Set up menubar
    fileMenu.addAction("Item 1");
    fileMenu.addAction("Item 2");
    this->menuBar()->addMenu(&fileMenu);
    this->menuBar()->addAction(fileMenu.menuAction());
    this->menuBar()->show();
    // this->menuBar()->setNativeMenuBar(false);
}

void PixelSortApp::reloadImage(QString fileStr) {
    QPixmap newImg(fileStr);
    mainImg->setPixmap(newImg);
    updateScene(newImg);
}

void PixelSortApp::updateScene(QPixmap& newImg) {
    scene.update();
    scene.setSceneRect(newImg.rect());
    view.update();
    view.fitInView(mainImg, Qt::KeepAspectRatio);
}
