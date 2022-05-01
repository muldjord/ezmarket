/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            itemstab.cpp
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

#include <stdio.h>

#include "itemstab.h"

ItemsTab::ItemsTab(QList<Item> &items, QWidget *parent)
  : QListWidget(parent), items(items)
{
  refreshTimer.setInterval(1000);
  refreshTimer.setSingleShot(true);
  connect(&refreshTimer, &QTimer::timeout, this, &ItemsTab::refreshItems);
  refreshTimer.start();

  elapsedTime.start();

  connect(this, &ItemsTab::itemPressed, this, &ItemsTab::itemSelected);
}

ItemsTab::~ItemsTab()
{
}

void ItemsTab::refreshItems()
{
  printf("REFRESHING ITEMS!\n");
  clear();

  for(auto &item: items) {
    item.age += elapsedTime.elapsed();
    printf("Item: %s, age: %d\n", qPrintable(item.id), item.age);
    /*
    if(!item.loanedBy.isEmpty()) {
    } else {
      item.loanedTimer = 0.0;
    }
    */
    QListWidgetItem *listItem = new QListWidgetItem;
    listItem->setText(item.id + " (" + item.barcode + ")");
    /*
    if(item.loanedTimer > 240 * 1000) {
      listItem->setForeground(Qt::red);
    } else if(!item.reservedBy.isEmpty()) {
      listItem->setForeground(Qt::blue);
    } else if(!item.loanedBy.isEmpty()) {
      listItem->setForeground(Qt::gray);
    }
    */
    listItem->setData(Qt::UserRole, item.barcode);
    addItem(listItem);
  }
  elapsedTime.restart();
  refreshTimer.start();
}

void ItemsTab::itemSelected(QListWidgetItem *item)
{
  //emit focusItem(item->data(Qt::UserRole).toString());
  refreshTimer.start();
}
