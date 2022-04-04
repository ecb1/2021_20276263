#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkNamedColors.h>
#include <QStatusBar>
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
#include <vtkPlane.h>
#include <vtkSmartPointer.h>
#include <vtkShrinkFilter.h>
#include <vtkClipPolyData.h>
#include <vtkPolyData.h>
#include <vtkClipDataSet.h>
#include <vtkAlgorithm.h>
#include <QAbstractButton>




namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

    // Example of slot definition in mainwindow.h---------------------------
public slots:
    void handleOpenButton();
    void handleSaveButton();
    void handleHelpButton();
    void handlePrintButton();
    void handleModelColorButton();
    void handleBackgroundColorButton();
    void handleShrinkFilterCheckBox(bool ShrinkFilterStatus);
    void handleClipFilterCheckBox(bool ClipFilterStatus);
    void handleRenderCube();
    void handleRenderSphere();
    
    // ---------------------------------------------------------------------

    // Example of signal definition in mainwindow.h-------------------------
signals:
    void statusUpdateMessage(const QString& message, int timeout);
    // ---------------------------------------------------------------------

private:
    Ui::MainWindow* ui;

    //Required Pointer Definitions
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    vtkSmartPointer<vtkClipDataSet> ClipFilter = vtkSmartPointer<vtkClipDataSet>::New();
    vtkSmartPointer<vtkShrinkFilter> ShrinkFilter = vtkSmartPointer<vtkShrinkFilter>::New();
    vtkSmartPointer<vtkPlane> planeLeft = vtkSmartPointer<vtkPlane>::New();

    vtkSmartPointer<vtkAlgorithm> source = vtkSmartPointer<vtkAlgorithm>::New();

    bool ShrinkFilterStatus, ClipFilterStatus;

};

#endif // MAINWINDOW_H
