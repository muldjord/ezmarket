/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            itemstab.h
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

#ifndef __ITEMSTAB_H__
#define __ITEMSTAB_H__

#include "datatypes.h"

#include <QWidget>
#include <QTableWidget>
#include <QTimer>

class ItemsTab : public QWidget
{
  Q_OBJECT
    
public:
  ItemsTab(QList<Account> &accounts,
           QList<Item> &items,
           QList<Category> &categories,
           const QMap<QString, QIcon> &icons,
           QWidget *parent);
  ~ItemsTab();
  
public slots:
  void refreshItems();

private slots:
  void editItem(int row, int);

private:
  QTableWidget *itemsList = nullptr;
  QList<Account> &accounts;
  QList<Item> &items;
  QList<Category> &categories;
  const QMap<QString, QIcon> &icons;
};

#endif // __ITEMSTAB_H__
