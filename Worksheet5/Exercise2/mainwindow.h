#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include "stockitemlistmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

// Example of slot definition in mainwindow.h---------------------------
public slots:
       void handleAddButton();
       void handleEditButton();
       void handleRemoveButton();
       void saveList();
// ---------------------------------------------------------------------

// Example of signal definition in mainwindow.h-------------------------
signals:
       void statusUpdateMessage( const QString & message, int timeout );
// ---------------------------------------------------------------------

private:
    Ui::MainWindow *ui;
    StockItemListModel stockList;
};


#endif // MAINWINDOW_H
