/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            categorywidget.h
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

#ifndef __CATEGORYWIDGET_H__
#define __CATEGORYWIDGET_H__

#include "datatypes.h"
#include "data.h"
#include "lineedit.h"
#include "spinbox.h"

#include <QWidget>
#include <QComboBox>

class CategoryWidget : public QWidget
{
  Q_OBJECT
    
public:
  CategoryWidget(Data &data,
                 Category &category,
                 QWidget *parent);
  ~CategoryWidget();
  bool isSane();
  void commitCategory();
  void removeCategory();
  bool inUse();

private slots:
  void setIconSearchText();
  void searchIcons();
  
private:
  Data &data;
  Category &category;
  LineEdit *idLineEdit = nullptr;
  LineEdit *searchLineEdit = nullptr;
  QComboBox *iconComboBox = nullptr;
  SpinBox *lifespanSpinBox = nullptr;
};

#endif // __CATEGORYWIDGET_H__
