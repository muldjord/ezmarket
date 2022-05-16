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
#include "itemeditor.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QSound>

ItemsTab::ItemsTab(Data &data,
                   QWidget *parent)
  : QWidget(parent), data(data)
{
  setStyleSheet("QTableView {font-size: " + QString::number(data.fontSizeSmall) + "px;}"
                "QHeaderView {font-size: " + QString::number(data.fontSizeSmall) + "px;}");
  itemsView = new QTableView(this);
  itemsModel = new ItemsModel(data, this);

  proxyModel = new QSortFilterProxyModel(this);
  proxyModel->setSourceModel(itemsModel);

  itemsView->setModel(proxyModel);
  itemsView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  itemsView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
  itemsView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  itemsView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
  itemsView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
  itemsView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  itemsView->verticalHeader()->setMinimumSectionSize(data.iconSize + 6);
  itemsView->verticalHeader()->setMaximumSectionSize(data.iconSize + 6);
  itemsView->setSelectionBehavior(QTableView::SelectRows);
  itemsView->setSelectionMode(QAbstractItemView::SingleSelection);
  itemsView->verticalHeader()->setVisible(false);
  itemsView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  itemsView->setSortingEnabled(true);
  connect(itemsView, &QTableView::doubleClicked, this, &ItemsTab::editItem);
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(itemsView);
  setLayout(layout);

  /*
  agingTimer.setInterval(2 * 60 * 1000);
  agingTimer.setSingleShot(false);
  connect(&agingTimer, &QTimer::timeout, this, &ItemsTab::ageItems);
  agingTimer.start();
  */
}

ItemsTab::~ItemsTab()
{
}

void ItemsTab::editItem(const QModelIndex &index)
{
  ItemEditor itemEditor(data.items.at(proxyModel->mapToSource(index).row()).barcode, data, this);
  itemEditor.exec();
  itemsModel->refreshAll();
  //itemsModel->emit dataChanged(proxyModel->mapToSource(index), proxyModel->mapToSource(index));
}

bool ItemsTab::ageItems()
{
  bool itemExpired = false;
  for(auto &item: data.items) {
    if(item.stock > 0) {
      item.age += 1;
      for(const auto &category: data.categories) {
        if(item.category == category.barcode) {
          if(!itemExpired && item.age > category.lifespan) {
            itemExpired = true;
          }
        }
      }
    } else {
      item.age = 0;
    }
  }
  itemsModel->refreshAll();

  return itemExpired;
}

void ItemsTab::addStock(const QString &barcode)
{
  for(int a = 0; a < data.items.length(); ++a) {
    if(data.items.at(a).barcode == barcode) {
      data.items[a].stock += 1;
      data.items[a].age = 0;
      itemsModel->refreshAll();
    }
  }
}

void ItemsTab::focusRow(const QString &barcode)
{
  for(int a = 0; a < data.items.length(); ++a) {
    if(data.items.at(a).barcode == barcode) {
      itemsView->selectRow(proxyModel->mapFromSource(itemsModel->index(a, 0)).row());
      break;
    }
  }
}
