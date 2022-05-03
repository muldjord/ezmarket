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
#include "entryeditor.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>

ItemsTab::ItemsTab(QList<Account> &accounts,
                   QList<Item> &items,
                   QList<Category> &categories,
                   const QMap<QString, QIcon> &icons,
                   QWidget *parent)
  : QWidget(parent), accounts(accounts), items(items), categories(categories), icons(icons)
{
  setStyleSheet("QTableWidget {qproperty-iconSize: 35px; font-size: 30px;}"
                "QHeaderView {font-size: 30px;}"
                "QTableWidget QLabel {background-color: black;}");
  itemsList = new QTableWidget(this);
  itemsList->setEditTriggers(QAbstractItemView::NoEditTriggers);
  itemsList->verticalHeader()->setVisible(false);
  itemsList->setSelectionBehavior(QTableView::SelectRows);
  itemsList->setSelectionMode(QAbstractItemView::SingleSelection);
  itemsList->setSortingEnabled(true);
  connect(itemsList, &QTableWidget::cellDoubleClicked, this, &ItemsTab::editItem);
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(itemsList);
  setLayout(layout);
  refreshItems();
}

ItemsTab::~ItemsTab()
{
}

void ItemsTab::refreshItems()
{
  printf("REFRESHING ITEMS!\n");
  itemsList->clear();
  itemsList->setColumnCount(8);
  itemsList->setRowCount(items.length());

  itemsList->setHorizontalHeaderLabels({"",
      tr("Item"),
      tr("Category"),
      tr("Price"),
      tr("Discount"),
      tr("Stock"),
      tr("Age"),
      tr("Barcode")});
  for(int row = 0; row < items.length(); ++row) {

    QLabel *iconLabel = new QLabel();
    iconLabel->setAlignment(Qt::AlignHCenter);
    iconLabel->setPixmap(icons[items.at(row).icon].pixmap(50, 50));
    //QTableWidgetItem *iconItem = new QTableWidgetItem(icons[items.at(row).icon], "");
    //iconItem->setData(Qt::UserRole, items.at(row).barcode);
    //iconItem->setTextAlignment(Qt::AlignCenter);
    //itemsList->setItem(row, 0, iconItem);
    itemsList->setCellWidget(row, 0, iconLabel);
    
    QTableWidgetItem *idItem = new QTableWidgetItem(items.at(row).id);
    idItem->setData(Qt::UserRole, items.at(row).barcode);
    itemsList->setItem(row, 1, idItem);

    QTableWidgetItem *categoryItem = new QTableWidgetItem(items.at(row).category);
    categoryItem->setData(Qt::UserRole, items.at(row).barcode);
    itemsList->setItem(row, 2, categoryItem);

    QTableWidgetItem *priceItem = new QTableWidgetItem();
    priceItem->setData(Qt::DisplayRole, items.at(row).price);
    priceItem->setData(Qt::UserRole, items.at(row).barcode);
    itemsList->setItem(row, 3, priceItem);

    QTableWidgetItem *discountItem = new QTableWidgetItem();
    discountItem->setData(Qt::DisplayRole, items.at(row).discount);
    discountItem->setData(Qt::UserRole, items.at(row).barcode);
    itemsList->setItem(row, 4, discountItem);

    QTableWidgetItem *stockItem = new QTableWidgetItem();
    stockItem->setData(Qt::DisplayRole, items.at(row).stock);
    stockItem->setData(Qt::UserRole, items.at(row).barcode);
    itemsList->setItem(row, 5, stockItem);

    QTableWidgetItem *ageItem = new QTableWidgetItem();
    ageItem->setData(Qt::DisplayRole, items.at(row).age);
    ageItem->setData(Qt::UserRole, items.at(row).barcode);
    itemsList->setItem(row, 6, ageItem);

    QTableWidgetItem *barcodeItem = new QTableWidgetItem(items.at(row).barcode);
    barcodeItem->setData(Qt::UserRole, items.at(row).barcode);
    itemsList->setItem(row, 7, barcodeItem);
  }
  itemsList->resizeColumnsToContents();
  itemsList->resizeRowsToContents();
}

void ItemsTab::editItem(int row, int)
{
  EntryEditor entryEditor(itemsList->item(row, 1)->data(Qt::UserRole).toString(), accounts, items, categories, icons, this);
  entryEditor.exec();
  
  printf("EDITING ROW %d, BARCODE %s\n", row, qPrintable(itemsList->item(row, 1)->data(Qt::UserRole).toString()));
}
