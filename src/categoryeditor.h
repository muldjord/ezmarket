/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            categoryeditor.cpp
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

#ifndef __CATEGORYEDITOR_H__
#define __CATEGORYEDITOR_H__

#include "datatypes.h"
#include "data.h"
#include "categorywidget.h"
#include "buttongroup.h"

#include <QDialog>
#include <QPushButton>

class CategoryEditor : public QDialog
{
Q_OBJECT

public:
  CategoryEditor(const QString &barcode,
                 Data &data,
                 QWidget *parent);

public slots:

signals:

private slots:
  void checkButton();

private:
  ButtonGroup *buttonGroup = nullptr;
  QPushButton *deleteButton = nullptr;
  QPushButton *saveButton = nullptr;
  QPushButton *cancelButton = nullptr;
  CategoryWidget *categoryWidget = nullptr;
  const QString &barcode;
  Data &data;
};
#endif // __CATEGORYEDITOR_H__
