#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>

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
    
    // ---------------------------------------------------------------------

    // Example of signal definition in mainwindow.h-------------------------
signals:
    void statusUpdateMessage(const QString& message, int timeout);
    // ---------------------------------------------------------------------

private:
    Ui::MainWindow* ui;

    vtkSmartPointer<vtkRenderer> renderer; //pointer for red

};

#endif // MAINWINDOW_H
