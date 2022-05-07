/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            categoriestab.cpp
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

#include "categoriestab.h"

CategoriesTab::CategoriesTab(const QList<Category> &categories, QWidget *parent)
  : QListWidget(parent), categories(categories)
{
  refreshTimer.setInterval(1000);
  refreshTimer.setSingleShot(true);
  connect(&refreshTimer, &QTimer::timeout, this, &CategoriesTab::refreshCategories);
  //refreshTimer.start();

  connect(this, &CategoriesTab::itemPressed, this, &CategoriesTab::categorySelected);
}

CategoriesTab::~CategoriesTab()
{
}

void CategoriesTab::refreshCategories()
{
  printf("REFRESHING CATEGORIES!\n");
  clear();

  for(auto &category: categories) {
    /*
    if(!category.loanedBy.isEmpty()) {
      category.loanedTimer += elapsedTime.elapsed();
      printf("Category: %s, loaned time: %d\n", qPrintable(category.title), category.loanedTimer);
    } else {
      category.loanedTimer = 0.0;
    }
    */
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(category.id + " (" + category.barcode + ")");
    /*
    if(category.loanedTimer > 240 * 1000) {
      item->setForeground(Qt::red);
    } else if(!category.reservedBy.isEmpty()) {
      item->setForeground(Qt::blue);
    } else if(!category.loanedBy.isEmpty()) {
      item->setForeground(Qt::gray);
    }
    */
    item->setData(Qt::UserRole, category.barcode);
    addItem(item);
  }
  //elapsedTime.restart();
  refreshTimer.start();
}

void CategoriesTab::categorySelected(QListWidgetItem *item)
{
  //emit focusCategory(item->data(Qt::UserRole).toString());
  refreshTimer.start();
}
