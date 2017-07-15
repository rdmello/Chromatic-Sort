
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
    sortContainer(&dockwidget_mid),
    sortBox(),
    rSort("", &sortContainer),
    gSort("", &sortContainer),
    bSort("", &sortContainer),
    moveContainer(&dockwidget_mid),
    moveBox(),
    rMove("", &moveContainer),
    gMove("", &moveContainer),
    bMove("", &moveContainer),
    minLabel("Min Threshold", &dockwidget_mid),
    rMin(Qt::Horizontal, &dockwidget_mid),
    gMin(Qt::Horizontal, &dockwidget_mid),
    bMin(Qt::Horizontal, &dockwidget_mid),
    maxLabel("Max Threshold", &dockwidget_mid),
    rMax(Qt::Horizontal, &dockwidget_mid),
    gMax(Qt::Horizontal, &dockwidget_mid),
    bMax(Qt::Horizontal, &dockwidget_mid),
    angleLabel("Angle"),
    angleDial(&dockwidget_mid),
    boxW(&dockwidget_mid),
    boxH(&dockwidget_mid),
    distX(&dockwidget_mid),
    distY(&dockwidget_mid),
    sortbutton("PixelSort", &dockwidget_mid),
    fileMenu("File"),
    img(),
    drv(img)
{
    this->resize(800, 500);
    this->setWindowTitle("PixelSort app v2");
    this->setUnifiedTitleAndToolBarOnMac(true);

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
    fileimport.setNameFilter("Images (*.png *.tiff *.tif)");
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
    formlayout.setLabelAlignment(Qt::AlignCenter);
    
    // add sort options buttons to formlayout
    sortContainer.setLayout(&sortBox);
    sortBox.setContentsMargins(0, 0, 0, 0);
    sortBox.addWidget(&rSort);
    sortBox.addWidget(&gSort);
    sortBox.addWidget(&bSort);
    rSort.setChecked(true);
    gSort.setChecked(true);
    bSort.setChecked(true);
    rSort.setStyleSheet("background-color: red;");
    gSort.setStyleSheet("background-color: green;");
    bSort.setStyleSheet("background-color: blue;");
    
    moveContainer.setLayout(&moveBox);
    moveBox.setContentsMargins(0, 0, 0, 0);
    moveBox.addWidget(&rMove);
    moveBox.addWidget(&gMove);
    moveBox.addWidget(&bMove);
    rMove.setChecked(true);
    gMove.setChecked(true);
    bMove.setChecked(true);
    rMove.setStyleSheet("background-color: red;");
    gMove.setStyleSheet("background-color: green;");
    bMove.setStyleSheet("background-color: blue;");

    rMin.setStyleSheet("background-color: red");
    rMax.setStyleSheet("background-color: red");
    gMin.setStyleSheet("background-color: green");
    gMax.setStyleSheet("background-color: green");
    bMin.setStyleSheet("background-color: blue");
    bMax.setStyleSheet("background-color: blue");
    rMin.setMinimum(0);
    rMin.setMaximum(100);
    gMin.setMinimum(0);
    gMin.setMaximum(100);
    bMin.setMinimum(0);
    bMin.setMaximum(100);
    rMax.setMinimum(0);
    rMax.setMaximum(100);
    gMax.setMinimum(0);
    gMax.setMaximum(100);
    bMax.setMinimum(0);
    bMax.setMaximum(100);
    rMax.setValue(100);
    gMax.setValue(100);
    bMax.setValue(100);
   
    formlayout.addRow("Sort", &sortContainer);
    formlayout.addRow("Move", &moveContainer);
    formlayout.addRow(&minLabel);
    formlayout.addRow(&rMin);
    formlayout.addRow(&gMin);
    formlayout.addRow(&bMin);
    formlayout.addRow(&maxLabel);
    formlayout.addRow(&rMax);
    formlayout.addRow(&gMax);
    formlayout.addRow(&bMax);

    /* Set angle control property */
    angleDial.setWrapping(true);
    angleDial.setMinimum(-2);
    angleDial.setMaximum(6);
    angleDial.setValue(0);
    formlayout.addRow(&angleLabel);
    formlayout.addRow(&angleDial);

    /* Set box width and height properties */
    boxW.setMinimum(1);
    boxW.setMaximum(10000);
    boxW.setSingleStep(100);
    boxW.setValue(200);
    boxH.setMinimum(1);
    boxH.setMaximum(10000);
    boxH.setSingleStep(100);
    boxH.setValue(200);
    formlayout.addRow("Width", &boxW);
    formlayout.addRow("Height", &boxH);

    /* Set repeat width and height properties */
    distX.setMinimum(200);
    distX.setMaximum(10000);
    distX.setSingleStep(100);
    distX.setValue(200);
    distY.setMinimum(200);
    distY.setMaximum(10000);
    distY.setSingleStep(100);
    distY.setValue(200);
    formlayout.addRow("RepeatX", &distX);
    formlayout.addRow("RepeatY", &distY);
    QObject::connect(&boxW, SIGNAL(valueChanged(int)), this, SLOT(distXset(int)));
    QObject::connect(&boxH, SIGNAL(valueChanged(int)), this, SLOT(distYset(int)));

    // add sort button to formlayout
    formlayout.addRow(&sortbutton);
    QObject::connect(&sortbutton, SIGNAL(clicked()), this, SLOT(sortButtonAction()));
 
    // add quit button to vbox
    // vbox.addWidget(&quitbutton);
    // QObject::connect(&quitbutton, SIGNAL(clicked()), appPtr, SLOT(quit()));

    // Set up menubar
    fileMenu.addAction("Item 1");
    fileMenu.addAction("Item 2");
    menuBar()->addMenu(&fileMenu);
    menuBar()->addAction(fileMenu.menuAction());
    menuBar()->show();
    // menuBar()->setNativeMenuBar(false);

    // Read initial image file
    imageFilePath = "../resources/lake.png";
    img.load(imageFilePath);
    QPixmap newImg = QPixmap::fromImage(img);
    mainImg = scene.addPixmap(newImg);
    opts.setImage(&drv);

    statusBar()->showMessage("New image successfully loaded: " + imageFilePath);

    // Set up statusbar (at bottom of window)
    opts.notifyMe = this;
    statusBar()->showMessage("Ready");
}

void PixelSortApp::reloadImage(QString fileStr) 
{
    statusBar()->showMessage("Reading image");
 
    // Read image using QImage
    img.load(fileStr);
    QPixmap newImg = QPixmap::fromImage(img);
    mainImg->setPixmap(newImg);
    updateScene(newImg);
 
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
    img.save(fileStr);
    statusBar()->showMessage("Successfully written image: " + fileStr);
}

void PixelSortApp::sortButtonAction() 
{
    /* Pick up current options */
    opts.sortColors[0] = rSort.isChecked();
    opts.sortColors[1] = gSort.isChecked();
    opts.sortColors[2] = bSort.isChecked();
    opts.moveColors[0] = rMove.isChecked();
    opts.moveColors[1] = gMove.isChecked();
    opts.moveColors[2] = bMove.isChecked();
    opts.colorMatcher[0] = double(rMin.value()) / 100.0;
    opts.colorMatcher[1] = double(rMax.value()) / 100.0;
    opts.colorMatcher[2] = double(gMin.value()) / 100.0;
    opts.colorMatcher[3] = double(gMax.value()) / 100.0;
    opts.colorMatcher[4] = double(bMin.value()) / 100.0;
    opts.colorMatcher[5] = double(bMax.value()) / 100.0;
    opts.theta = 45.0*double(angleDial.value());
    opts.rect[2] = boxW.value();
    opts.rect[3] = boxH.value();
    opts.Xrepeat[1] = distX.value();
    opts.Yrepeat[1] = distY.value();

    /* Sort and Apply */
    statusBar()->showMessage("Sorting image...");
    opts.doSort();

    /* Updating Qt::QPixmap */
    statusBar()->showMessage("Writing into QPixmap");
    appPtr->processEvents();

    QPixmap newImg = QPixmap::fromImage(img);
    mainImg->setPixmap(newImg);
    updateScene(newImg);
    statusBar()->showMessage("Ready");
}

void PixelSortApp::notify(const char* str)
{
    statusBar()->showMessage(str);
    appPtr->processEvents();
    QPixmap newImg = QPixmap::fromImage(img);
    mainImg->setPixmap(newImg);
    updateScene(newImg);

}

void PixelSortApp::distXset(int newval)
{
    if (distX.value() < newval) {
        distX.setValue(newval);
    }
    distX.setMinimum(newval);
}

void PixelSortApp::distYset(int newval)
{
    if (distY.value() < newval) {
        distY.setValue(newval);
    }
    distY.setMinimum(newval);
}
