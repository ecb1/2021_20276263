#include <vtkSmartPointer.h>
#include <vtkCubeSource.h>
#include <vtkSphereSource.h>
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
#include <QStatusBar>
#include <QFileDialog>
#include <QInputDialog>
#include <QDebug> 
#include <QColorDialog>
#include <QAbstractButton>


#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // standard call to setup Qt UI (same as previously)
    ui->setupUi( this );
	
	// Connect the released() signal of each action object to the action slot in each object
	connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);
	connect(ui->actionFileSave, &QAction::triggered, this, &MainWindow::handleSaveButton);
	connect(ui->actionFileOpen, &QAction::triggered, this, &MainWindow::handleOpenButton);
	connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::handleHelpButton);
	connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::handlePrintButton);
	connect(ui->ModelColourButton, &QPushButton::released, this, &MainWindow::handleModelColorButton);
	connect(ui->BackgroundColourButton, &QPushButton::released, this, &MainWindow::handleBackgroundColorButton);
	connect(ui->shrinkFilter_checkBox, &QCheckBox::toggled, this, &MainWindow::handleShrinkFilterCheckBox );
	connect(ui->clipFilter_checkBox, &QCheckBox::toggled, this, &MainWindow::handleClipFilterCheckBox );
	connect(ui->actionCube, &QAction::triggered, this, &MainWindow::handleRenderCube);
	connect(ui->actionSphere, &QAction::triggered, this, &MainWindow::handleRenderSphere);

	ShrinkFilterStatus = ui->shrinkFilter_checkBox->isChecked();
	ClipFilterStatus = ui->clipFilter_checkBox->isChecked();


	//Create and Setup Renderer Window
	//Now need to create a VTK render windowand link it to the QtVTK widget
	vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
	ui->qvtkWidget->SetRenderWindow(renderWindow);			// note that vtkWidget is the name I gave to my QtVTKOpenGLWidget in Qt creator

	// Create a renderer, and render window
	renderer = vtkSmartPointer<vtkRenderer>::New();
	//vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();		// ###### We've already created the renderWindow this time ######
	ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);									// ###### ask the QtVTKOpenGLWidget for its renderWindow ######

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

	reader->SetFileName(c_fileName);
	reader->Update();

	source = reader;

	//Code to display the STL file data
	mapper->SetInputConnection(reader->GetOutputPort());

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


	emit statusUpdateMessage(QString("CAD (.stl) File has been opened"), 0);
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

void MainWindow::handleModelColorButton() {
	//Open colour dialog to prompt user for colour of choice
	QColor Color = QColorDialog::getColor(Qt::white, this, "Choose Model Colour");
	
	if (Color.isValid())
	{
		double red = Color.redF();
		double green = Color.greenF();
		double blue = Color.blueF();

		actor->GetProperty()->SetColor(red, green, blue);
	}

	//Rerenders the model with the changed colour
	renderWindow->Render();
	ui->qvtkWidget->GetRenderWindow()->Render();

	emit statusUpdateMessage(QString("Change Model Colour Button was clicked"), 0);
}

void MainWindow::handleBackgroundColorButton() {
	//Open colour dialog to prompt user for colour of choice
	QColor Color = QColorDialog::getColor(Qt::white, this, "Choose Background Colour");
	
	if (Color.isValid())
	{
		double red = Color.redF();
		double green = Color.greenF();
		double blue = Color.blueF();

		renderer->SetBackground(red, green, blue);
	}
	
	//Rerenders the background with the changed colour
	renderWindow->Render();
	ui->qvtkWidget->GetRenderWindow()->Render();

	emit statusUpdateMessage(QString("Change Background Colour Button was clicked"), 0);
}

void MainWindow::handleShrinkFilterCheckBox(bool ShrinkFilterStatus) {
	if (ShrinkFilterStatus == true) {
		//ensure that no other filter is applied
		ShrinkFilter->SetInputConnection(source->GetOutputPort());
		ShrinkFilter->SetShrinkFactor(.5);
		ShrinkFilter->Update();

		mapper->SetInputConnection(ShrinkFilter->GetOutputPort());

		//Display update message
		emit statusUpdateMessage(QString("Shrink Filter Applied"), 0);
	}

	else {
		mapper->SetInputConnection(source->GetOutputPort());
	}

	renderWindow->Render();
	ui->qvtkWidget->GetRenderWindow()->Render();
}



void MainWindow::handleClipFilterCheckBox(bool ClipFilterStatus) {
	if (ClipFilterStatus == true) {
		planeLeft->SetOrigin(0.0, 0.0, 0.0);
		planeLeft->SetNormal(-1.0, 0.0, 0.0);

		ClipFilter->SetInputConnection(source->GetOutputPort());
		ClipFilter->SetClipFunction(planeLeft.Get());
		mapper->SetInputConnection(ClipFilter->GetOutputPort());

		//Display Update Message
		emit statusUpdateMessage(QString("Clip filter applied"), 0);
	}
	else
	{
		mapper->SetInputConnection(source->GetOutputPort());
	}
	
	renderWindow->Render();
	ui->qvtkWidget->GetRenderWindow()->Render();
}

void MainWindow::handleRenderCube() {
	// Create a cube using a vtkCubeSource
	vtkSmartPointer<vtkCubeSource> cubeSource = vtkSmartPointer<vtkCubeSource>::New();

	source = cubeSource;

	//mapper that will hold the cube's geometry in a format suitable for rendering
	mapper->SetInputConnection(cubeSource->GetOutputPort());

	//actor that is used to set the cube's properties for rendering and place it in the window
	actor->SetMapper(mapper);
	actor->GetProperty()->EdgeVisibilityOn();

	actor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());

	// Add the actor to the scene
	renderer->AddActor(actor);
	renderer->SetBackground(colors->GetColor3d("Silver").GetData());

	// Setup the renderers's camera
	renderer->ResetCamera();
	renderer->GetActiveCamera()->Azimuth(30);
	renderer->GetActiveCamera()->Elevation(30);
	renderer->ResetCameraClippingRange();
}

void MainWindow::handleRenderSphere() {
	// Create a sphere using a vtkCubeSource
	vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();

	source = sphereSource;

	//mapper that will hold the cube's geometry in a format suitable for rendering
	mapper->SetInputConnection(sphereSource->GetOutputPort());

	//actor that is used to set the cube's properties for rendering and place it in the window
	actor->SetMapper(mapper);
	actor->GetProperty()->EdgeVisibilityOn();

	actor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());

	// Add the actor to the scene
	renderer->AddActor(actor);
	renderer->SetBackground(colors->GetColor3d("Silver").GetData());

	// Setup the renderers's camera
	renderer->ResetCamera();
	renderer->GetActiveCamera()->Azimuth(30);
	renderer->GetActiveCamera()->Elevation(30);
	renderer->ResetCameraClippingRange();
}
