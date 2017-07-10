
#include <iostream>
#include <cmath>
#include <string>

#include <QImage>

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
    exportbutton("Export", &dockwidget_mid),
    fileexport(this),
    formlayout(),
    sortbutton("PixelSort", &dockwidget_mid),
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
    this->addDockWidget(Qt::RightDockWidgetArea, &dockwidget);
    dockwidget.setAllowedAreas(Qt::RightDockWidgetArea);
    dockwidget.setFeatures(QDockWidget::NoDockWidgetFeatures);
    dockwidget.setWidget(&dockwidget_mid);
        
    // add import and export button to vbox
    vbox.addWidget(&importbutton);
    vbox.addWidget(&exportbutton);
   
    // file import dialog settings
    fileimport.setFileMode(QFileDialog::ExistingFile);
    fileimport.setAcceptMode(QFileDialog::AcceptOpen);
    fileimport.setNameFilter("Images (*.png *.tiff)");
    fileimport.fileSelected(imageFilePath);

    // file export dialog settings
    fileexport.setFileMode(QFileDialog::AnyFile);
    fileexport.setAcceptMode(QFileDialog::AcceptSave);

    // Connect actual import button events
    QObject::connect(&importbutton, SIGNAL(clicked()), &fileimport, SLOT(exec()));
    QObject::connect(&fileimport, SIGNAL(fileSelected(QString)), this, SLOT(reloadImage(QString)));
 
    // Connect actual export button events
    QObject::connect(&exportbutton, SIGNAL(clicked()), &fileexport, SLOT(exec()));
    QObject::connect(&fileexport, SIGNAL(fileSelected(QString)), this, SLOT(writeImage(QString)));
 
    // Set up the form layout widget 
    vbox.addLayout(&formlayout);
    
    // add sort button to formlayout
    formlayout.addRow("Do sort", &sortbutton);
    QObject::connect(&sortbutton, SIGNAL(clicked()), this, SLOT(sortButtonAction()));
 
    // add quit button to vbox
    vbox.addWidget(&quitbutton);
    QObject::connect(&quitbutton, SIGNAL(clicked()), appPtr, SLOT(quit()));

    // Set up menubar
    fileMenu.addAction("Item 1");
    fileMenu.addAction("Item 2");
    menuBar()->addMenu(&fileMenu);
    menuBar()->addAction(fileMenu.menuAction());
    menuBar()->show();
    // menuBar()->setNativeMenuBar(false);

    // Set up statusbar (at bottom of window)
    opts.notifyMe = this;
    statusBar()->showMessage("Ready");
}

void PixelSortApp::reloadImage(QString fileStr) 
{
    statusBar()->showMessage("Reading image");
    QPixmap newImg(fileStr);
    mainImg->setPixmap(newImg);
    updateScene(newImg);
  
    // Read image using imagemagick
    img.read(fileStr.toStdString());
    opts.setImage(&img);
    statusBar()->showMessage("New image successfully loaded: " + fileStr);
}

void PixelSortApp::updateScene(QPixmap& newImg) {
    scene.update();
    scene.setSceneRect(newImg.rect());
    view.update();
    view.fitInView(mainImg, Qt::KeepAspectRatio);
}

void PixelSortApp::writeImage(QString fileStr) 
{
    statusBar()->showMessage("Writing image");
    img.write(fileStr.toStdString());
    statusBar()->showMessage("Successfully written image: " + fileStr);
    // QPixmap newImg(fileStr);
    // mainImg->setPixmap(newImg);
    // updateScene(newImg);
}

void PixelSortApp::sortButtonAction() 
{
    /* Sort and Apply */
    statusBar()->showMessage("Sorting image...");
    opts.doSort();

    /* Updating Qt::QPixmap */
    statusBar()->showMessage("Writing into QPixmap");
    QImage qimg(img.columns(), img.rows(), QImage::Format_RGB32);
    QColor value;
    Magick::ColorRGB mcol;
    for (unsigned int i = 0; i < img.rows(); ++i) {
        for (unsigned int j = 0; j < img.columns(); ++j) {
            mcol = Magick::ColorRGB(img.pixelColor(j, i));
            value = QColor(
                std::floor(255*mcol.red()), 
                std::floor(255*mcol.green()), 
                std::floor(255*mcol.blue())
            );
            qimg.setPixel(j, i, value.rgb());
        }
    }
    
    QPixmap newImg = QPixmap::fromImage(qimg);
    mainImg->setPixmap(newImg);
    updateScene(newImg);
    statusBar()->showMessage("Ready");
}

void PixelSortApp::notify(const char* str)
{
    statusBar()->showMessage(str);
}
