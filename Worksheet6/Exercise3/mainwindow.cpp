#include <vtkSmartPointer.h>
#include <vtkCubeSource.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkPolyData.h>
#include <vtkDataSetMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkSTLReader.h>
#include <QFileDialog>
#include <QMessageBox>


#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // standard call to setup Qt UI (same as previously)
    ui->setupUi( this );

	//Set icon paths
	ui->actionFileSave->setIcon(QIcon("C:\Users\ecb\Desktop\2021_20276263\Worksheet6\Exercise3\Icons\filesave.png"));
	ui->actionFileOpen->setIcon(QIcon("C:\Users\ecb\Desktop\2021_20276263\Worksheet6\Exercise3\Icons\fileopen.png"));
	ui->actionHelp->setIcon(QIcon("C:\Users\ecb\Desktop\2021_20276263\Worksheet6\Exercise3\Icons\help.png"));
	ui->actionPrint->setIcon(QIcon("C:\Users\ecb\Desktop\2021_20276263\Worksheet6\Exercise3\Icons\print.png"));
	
	// Connect the released() signal of each action object to the action slot in each object
	connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);
	connect(ui->actionFileSave, &QAction::triggered, this, &MainWindow::handleSaveButton);
	connect(ui->actionFileOpen, &QAction::triggered, this, &MainWindow::handleOpenButton);
	connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::handleHelpButton);
	connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::handlePrintButton);

	//Create and Setup Renderer Window
	//Now need to create a VTK render windowand link it to the QtVTK widget
	vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
	ui->qvtkWidget->SetRenderWindow(renderWindow);			// note that vtkWidget is the name I gave to my QtVTKOpenGLWidget in Qt creator

	// Create a renderer, and render window
	renderer = vtkSmartPointer<vtkRenderer>::New();
	//vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();		// ###### We've already created the renderWindow this time ######
	ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);									// ###### ask the QtVTKOpenGLWidget for its renderWindow ######



	/*  EXERCISE 2 CODE FOR REFERENCE
	//Now need to create a VTK render windowand link it to the QtVTK widget
	vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
	ui->qvtkWidget->SetRenderWindow( renderWindow );			// note that vtkWidget is the name I gave to my QtVTKOpenGLWidget in Qt creator
	
	// Now use the VTK libraries to render something. To start with you can copy-paste the cube example code, there are comments to show where the code must be modified.
	//--------------------------------------------- Code From Example 1 --------------------------------------------------------------------------
	// Create a cube using a vtkCubeSource
	vtkSmartPointer<vtkCubeSource> cubeSource = vtkSmartPointer<vtkCubeSource>::New();

	// Create a mapper that will hold the cube's geometry in a format suitable for rendering
	vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	mapper->SetInputConnection( cubeSource->GetOutputPort() );

	// Create an actor that is used to set the cube's properties for rendering and place it in the window
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->EdgeVisibilityOn();

	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
	actor->GetProperty()->SetColor( colors->GetColor3d("Red").GetData() );

	// Create a renderer, and render window
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	//vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();		// ###### We've already created the renderWindow this time ######
	ui->qvtkWidget->GetRenderWindow()->AddRenderer( renderer );									// ###### ask the QtVTKOpenGLWidget for its renderWindow ######

	// Link a renderWindowInteractor to the renderer (this allows you to capture mouse movements etc)  ###### Not needed with Qt ######
	//vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//renderWindowInteractor->SetRenderWindow( ui->vtkWidget->GetRenderWindow() );				

	// Add the actor to the scene
	renderer->AddActor(actor);
	renderer->SetBackground( colors->GetColor3d("Silver").GetData() );

	// Setup the renderers's camera
	renderer->ResetCamera();
	renderer->GetActiveCamera()->Azimuth(30);
	renderer->GetActiveCamera()->Elevation(30);
	renderer->ResetCameraClippingRange();

	// Render and interact
	//renderWindow->Render();					// ###### Not needed with Qt ######
	//renderWindowInteractor->Start();			// ###### Not needed with Qt ######
	--------------------------------------------- /Code From Example 1 -------------------------------------------------------------------------*/
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::handleOpenButton() {
	//Code to load a STL file of the user's choice
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open CAD File"), "/home", tr("CAD Files (*.stl)"));
	//Convert QString to standard C string
	QByteArray ba = fileName.toLocal8Bit();
	const char* c_fileName = ba.data();

	vtkNew<vtkSTLReader> reader;
	reader->SetFileName(c_fileName);
	reader->Update();

	//Code to display the STL file data
	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputConnection(reader->GetOutputPort());

	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);
	actor->GetProperty()->EdgeVisibilityOn();

	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
	actor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());

	// Add the actor to the scene
	renderer->AddActor(actor);
	renderer->SetBackground(colors->GetColor3d("Silver").GetData());

	// Setup the renderers's camera
	renderer->ResetCamera();
	renderer->GetActiveCamera()->Azimuth(30);
	renderer->GetActiveCamera()->Elevation(30);
	renderer->ResetCameraClippingRange();


	emit statusUpdateMessage(QString("CAD File has been opened"), 0);
}


void MainWindow::handleSaveButton() {
	//Insert Code
}


void MainWindow::handleHelpButton() {
	//Insert Code
}


void MainWindow::handlePrintButton() {
	//Insert Code
}