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

#include "data.h"
#include "datatypes.h"
#include "accountstab.h"
#include "itemstab.h"
#include "categoriestab.h"

#include <QMainWindow>
#include <QTimer>
#include <QLineEdit>
#include <QGroupBox>
#include <QListWidget>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QTabWidget>

class MainWindow : public QMainWindow
{
  Q_OBJECT
    
 public:
  MainWindow();
  ~MainWindow();
  
private slots:
  void showAbout();
  void checkBarcode();
  void focusBarcodeLineEdit();
  void initRandomSound();
  
private:
  void loadIcons();
  void loadDatabase();
  void saveDatabase();

  void createActions();
  void createMenus();
  void createToolBar();

  void parseAccount(const QString &string);
  void parseItem(const QString &string);
  void parseCategory(const QString &string);
  
  QString getAccountFromBarcode(const QString &barcode);
  QString getItemFromBarcode(const QString &barcode);
  QString getCategoryFromBarcode(const QString &barcode);

  void playRandomSound();

  QAction *quitAct;
  QAction *aboutAct;
  QMenu *fileMenu;
  QMenu *helpMenu;
  QMenuBar *menuBar;

  QLineEdit *barcodeLineEdit = nullptr;

  QTabWidget *modeTabs = nullptr;

  AccountsTab *accountsTab = nullptr;
  ItemsTab *itemsTab = nullptr;
  CategoriesTab *categoriesTab = nullptr;
  /*
  CheckoutTab *checkoutTab = nullptr;
  */

  QTimer randomTimer;
  QTimer focusTimer;

  Data data;
};

#endif // __MAINWINDOW_H__
