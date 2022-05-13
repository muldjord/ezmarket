/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            categoriesmodel.h
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

#ifndef __CATEGORIESMODEL_H__
#define __CATEGORIESMODEL_H__

#include "datatypes.h"
#include "data.h"

#include <QAbstractTableModel>

class CategoriesModel : public QAbstractTableModel
{
Q_OBJECT
  
public:
  CategoriesModel(Data &data,
             QObject *parent);
  
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  //bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
  
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  void beginInsertRow(int row);
  bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
  void refreshAll();
  //QList<ExamData> getRows(const QModelIndexList &indexes) const;
  //int getFocusRow(const int &examinationUid) const;
  
  //bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
  //bool insertColumns(int position, int columns, const QModelIndex &parent = QModelIndex());
  //bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());
  //bool removeColumns(int position, int columns, const QModelIndex &parent = QModelIndex());

signals:
  void editingDone();

private:
  Data &allData;
};

#endif // __CATEGORIESMODEL_H__
