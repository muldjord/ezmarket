/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            itemwidget.h
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

#ifndef __ITEMWIDGET_H__
#define __ITEMWIDGET_H__

#include "datatypes.h"
#include "data.h"
#include "lineedit.h"

#include <QWidget>
#include <QComboBox>

class ItemWidget : public QWidget
{
  Q_OBJECT
    
public:
  ItemWidget(Data &data,
             Item &item,
             QWidget *parent);
  ~ItemWidget();
  bool isSane();
  void commitItem();

private slots:
  void setIconSearchText();
  void searchIcons();
  
private:
  Data &data;
  Item &item;
  LineEdit *idLineEdit = nullptr;
  LineEdit *searchLineEdit = nullptr;
  QComboBox *iconComboBox = nullptr;
  QComboBox *categoryComboBox = nullptr;
  LineEdit *priceLineEdit = nullptr;
  LineEdit *discountLineEdit = nullptr;
};

#endif // __ITEMWIDGET_H__
