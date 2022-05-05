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

#include "itemstab.h"
#include "itemsmodel.h"
#include "entryeditor.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>

ItemsTab::ItemsTab(QList<Item> &items,
                   const QList<Category> &categories,
                   const QMap<QString, QIcon> &icons,
                   QWidget *parent)
  : QWidget(parent), items(items), categories(categories), icons(icons)
{
  setStyleSheet("QTableView {font-size: 30px;}"
                "QHeaderView {font-size: 30px;}");
  itemsView = new QTableView(this);
  ItemsModel *itemsModel = new ItemsModel(items, icons, this);
  itemsView->setModel(itemsModel);
  itemsView->setSelectionBehavior(QTableView::SelectRows);
  itemsView->setSelectionMode(QAbstractItemView::SingleSelection);
  itemsView->verticalHeader()->setVisible(false);
  itemsView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  //itemsView->setSortingEnabled(true);
  //connect(itemsView, &QTableView::cellDoubleClicked, this, &ItemsTab::editItem);
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(itemsView);
  setLayout(layout);
  //refreshItems();
}

ItemsTab::~ItemsTab()
{
}
