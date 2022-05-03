/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            itemsmodel.h
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

#ifndef __ITEMSMODEL_H__
#define __ITEMSMODEL_H__

#include "itemsdata.h"

#include <QAbstractTableModel>
#include <QStringList>
#include <QVariant>

class ItemsModel : public QAbstractTableModel
{
Q_OBJECT
  
public:
  ItemsModel(const QList<ItemsData> &data, QObject *parent = 0);
  
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
  
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

  QList<ItemsData> getRows(const QModelIndexList &indexes) const;
  int getFocusRow(const int &itemsUid) const;
  
  //bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
  //bool insertColumns(int position, int columns, const QModelIndex &parent = QModelIndex());
  //bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());
  //bool removeColumns(int position, int columns, const QModelIndex &parent = QModelIndex());

signals:
  void updateItems(const ItemsData &data) const;
  
private:
  QList<ItemsData> rowList;
};

#endif // __ITEMSMODEL_H__
