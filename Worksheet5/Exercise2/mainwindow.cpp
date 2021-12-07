// mainwindow.cpp ------------------------------------------------------
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stockitem.h"
#include "edititemdialog.h"
#include <QMessageBox>
#include <QFile>
#include "stockitemlistmodel.h"

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow) {
   ui->setupUi(this);
   
   // Link the ListModel to the ListView
   ui->listView->setModel( &stockList );
   // Tell this list view to only accept single selections
   ui->listView->setSelectionBehavior( QAbstractItemView::SelectItems );

   // Connect the released() signal of the addButton object to the handleAddButton slot in this object
   connect( ui->addButton, &QPushButton::released, this, &MainWindow::handleAddButton );
      connect( ui->editButton, &QPushButton::released, this, &MainWindow::handleEditButton );
      connect( ui->removeButton, &QPushButton::released, this, &MainWindow::handleRemoveButton );
      connect( this, &MainWindow::statusUpdateMessage, ui->statusBar, &QStatusBar::showMessage );
      connect(ui->action_Save, &QAction::triggered, this, &MainWindow::saveList );
}

MainWindow::~MainWindow() {
   delete ui;
}

void MainWindow::handleAddButton() {
   StockItem item;

   stockList.addItem( item );

   emit statusUpdateMessage( QString("Add button was clicked"), 0 );
}

void MainWindow::handleEditButton() {
   EditItemDialog dialog( this );
   QModelIndexList selectedList;
   
   selectedList = ui->listView->selectionModel()->selectedIndexes();
   
   if( selectedList.length() == 1 ) {
      // selectedList is a list of all selected items in the listView. Since we set its
      // behaviour to single selection, were only interested in the first selecteded item.
      StockItem item = stockList.getItem( selectedList[0] );

   if( dialog.runDialog( item ) ) {
   // user clicked ok, need to update item in list...
   stockList.setItem( item, selectedList[0] );
   }
 } else {
   emit statusUpdateMessage( QString("No item selected to edit!"), 0 );
 }
}

void MainWindow::handleRemoveButton() {
   QModelIndexList selectedList;
   
   selectedList = ui->listView->selectionModel()->selectedIndexes();

   stockList.removeItem( selectedList[0] );

   emit statusUpdateMessage( QString("Remove button was clicked"), 0 );
}

void MainWindow::saveList() {
   QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                           "C:\\Users\\eeyec3\\Desktop",
                           tr("Images (*.txt)"));

   QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    for(int i = 0; i < stockList.rowCount(); i++) {
        out<<stockList<data<<"\n";
}

   emit statusUpdateMessage( QString("The stock list has been saved"), 0 );
   
}

// ---------------------------------------------------------------------
