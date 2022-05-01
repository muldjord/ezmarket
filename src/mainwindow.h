/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            mainwindow.h
 *
 *  Sat Apr 30 09:03:00 CEST 2022
 *  Copyright 2022 Lars Muldjord
 *  muldjordlars@gmail.com
 ****************************************************************************/
/*
 *  This file is part of EZMarket.
 *
 *  EZMarket is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  EZMarket is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with EZMarket; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include "booklist.h"

#include <QMainWindow>
#include <QTimer>
#include <QLineEdit>
#include <QGroupBox>
#include <QListWidget>
#include <QAction>
#include <QMenu>
#include <QMenuBar>

class MainWindow : public QMainWindow
{
  Q_OBJECT
    
 public:
  MainWindow();
  ~MainWindow();
  
public slots:
  
protected:
  
signals:

private slots:
  void showAbout();
  void checkBarcode();
  void clearAll();
  void clearPerson();
  void clearBook();
  void focusBook(const QString &barcode);
  void focusBarcodeLineEdit();
  void initRandomSound();
  
private:
  void loadDatabase();
  void saveDatabase();

  void createActions();
  void createMenus();
  void createToolBar();

  void parsePerson(const QString &string);
  void parseBook(const QString &string);
  
  QString getPersonFromBarcode(const QString &barcode);
  QString getBookFromBarcode(const QString &barcode);

  void playRandomSound();

  QAction *quitAct;
  QAction *preferencesAct;
  QAction *aboutAct;
  QMenu *fileMenu;
  QMenu *optionsMenu;
  QMenu *helpMenu;
  QMenuBar *menuBar;

  QTimer randomTimer;
  QTimer clearTimer;
  QTimer focusTimer;

  QLineEdit *barcodeLineEdit = nullptr;

  QGroupBox *personGroup = nullptr;
  QLineEdit *personBarcodeLineEdit = nullptr;
  QLineEdit *personNameLineEdit = nullptr;
  QListWidget *personOnLoanList = nullptr;
  QListWidget *personReservedList = nullptr;
  
  QGroupBox *bookGroup = nullptr;
  QLineEdit *bookBarcodeLineEdit = nullptr;
  QLineEdit *bookTitleLineEdit = nullptr;
  QLineEdit *bookStatusLineEdit = nullptr;
  QLineEdit *bookLoanedByLineEdit = nullptr;
  QLineEdit *bookReservedByLineEdit = nullptr;
  
  QGroupBox *allBooksGroup = nullptr;
  BookList *allBooksList = nullptr;

  QList<Person> persons;
  QList<Book> books;
};

#endif // __MAINWINDOW_H__
