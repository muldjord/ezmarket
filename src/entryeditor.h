/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            entryeditor.cpp
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

#pragma once

#include "datatypes.h"
#include "data.h"
#include "accountwidget.h"
#include "itemwidget.h"
#include "categorywidget.h"

#include <QDialog>
#include <QAbstractButton>
#include <QPushButton>
#include <QStackedLayout>

class EntryEditor : public QDialog
{
Q_OBJECT

public:
  EntryEditor(const QString &barcode,
              Data &data,
              QWidget *parent);
  QString getType();
  void addAccount();
  void addItem();
  void addCategory();

public slots:

signals:

private slots:
  void typeChanged(QAbstractButton *button);
  void checkSanity();

private:
  QButtonGroup *typeGroup = nullptr;

  QPushButton *accountButton = nullptr;
  QPushButton *itemButton = nullptr;
  QPushButton *categoryButton = nullptr;

  AccountWidget *accountWidget = nullptr;
  ItemWidget *itemWidget = nullptr;
  CategoryWidget *categoryWidget = nullptr;

  QStackedLayout *typeLayout = nullptr;

  const QString &barcode;
  Account account;
  Item item;
  Category category;
  Data &data;
};
