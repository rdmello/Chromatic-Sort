
#include <iostream>
#include <cmath>

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
    this->addDockWidget(Qt::LeftDockWidgetArea, &dockwidget);
    dockwidget.setAllowedAreas(Qt::LeftDockWidgetArea);
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
    this->menuBar()->addMenu(&fileMenu);
    this->menuBar()->addAction(fileMenu.menuAction());
    this->menuBar()->show();
    // this->menuBar()->setNativeMenuBar(false);
}

void PixelSortApp::reloadImage(QString fileStr) {
    QPixmap newImg(fileStr);
    mainImg->setPixmap(newImg);
    updateScene(newImg);
  
    // Read image using imagemagick
    img.read(fileStr.toStdString());
    opts.setImage(&img);
}

void PixelSortApp::updateScene(QPixmap& newImg) {
    scene.update();
    scene.setSceneRect(newImg.rect());
    view.update();
    view.fitInView(mainImg, Qt::KeepAspectRatio);
}

void PixelSortApp::writeImage(QString fileStr) {

    std::cout << "Written Image: " << fileStr.toStdString() << std::endl;
    img.write(fileStr.toStdString());
    // QPixmap newImg(fileStr);
    // mainImg->setPixmap(newImg);
    // updateScene(newImg);
}

void PixelSortApp::sortButtonAction() {
   
    /* Sort and Apply */
    std::cout << "Sorting pixelvector..." << std::endl;
    opts.doSort();

    /* Updating Qt::QPixmap */
    std::cout << "Converting pixelvector to QImage..." << std::endl;
    QImage qimg(img.columns(), img.rows(), QImage::Format_RGB32);
    QRgb value;
    Magick::ColorRGB mcol;
    for (unsigned int i = 0; i < img.rows(); ++i) {
        for (unsigned int j = 0; j < img.columns(); ++j) {
            mcol = Magick::ColorRGB(img.pixelColor(j, i));
            value = qRgb(
                std::floor(256*mcol.red()), 
                std::floor(256*mcol.green()), 
                std::floor(256*mcol.blue())
            );
            qimg.setPixel(j, i, value);
        }
    }
    
    QPixmap newImg = QPixmap::fromImage(qimg);
    mainImg->setPixmap(newImg);
    updateScene(newImg);
}

