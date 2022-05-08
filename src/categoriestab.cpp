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

#include "categoriestab.h"
#include "categoryeditor.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>

CategoriesTab::CategoriesTab(Data &data,
                   QWidget *parent)
  : QWidget(parent), data(data)
{
  setStyleSheet("QTableView {font-size: 30px;}"
                "QHeaderView {font-size: 30px;}");
  categoriesView = new QTableView(this);
  categoriesModel = new CategoriesModel(data, this);

  proxyModel = new QSortFilterProxyModel(this);
  proxyModel->setSourceModel(categoriesModel);

  categoriesView->setModel(proxyModel);
  categoriesView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  categoriesView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  categoriesView->verticalHeader()->setMinimumSectionSize(data.iconSize + 6);
  categoriesView->verticalHeader()->setMaximumSectionSize(data.iconSize + 6);
  categoriesView->setSelectionBehavior(QTableView::SelectRows);
  categoriesView->setSelectionMode(QAbstractItemView::SingleSelection);
  categoriesView->verticalHeader()->setVisible(false);
  categoriesView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  categoriesView->setSortingEnabled(true);
  connect(categoriesView, &QTableView::doubleClicked, this, &CategoriesTab::editCategory);
  /*
  QPusbButton *addButton = new QPushButton(tr("Add category"));
  connect(addButton, &QPushButton::clicked, this, &CategoriesTab::addCategory);

  QHBoxLayout *functionLayout = new QHBoxLayout;
  functionsLayout->addWidget(addButton);
  */
  
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(categoriesView);
  setLayout(layout);
}

CategoriesTab::~CategoriesTab()
{
}

void CategoriesTab::editCategory(const QModelIndex &index)
{
  CategoryEditor categoryEditor(data.categories.at(proxyModel->mapToSource(index).row()).barcode, data, this);
  categoryEditor.exec();
}

/*
void CategoriesTab::addCategory()
{
  CategoryEditor categoryEditor(data.categories.at(proxyModel->mapToSource(index).row()).barcode, data, this);
  categoryEditor.exec();
}
*/
