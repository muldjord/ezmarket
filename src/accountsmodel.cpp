/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            accountsmodel.cpp
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

#include "accountsmodel.h"
#include "imgtools.h"

#include <QIcon>
#include <QPainter>
#include <QBrush>

AccountsModel::AccountsModel(Data &data,
                             QObject *parent)
  : QAbstractTableModel(parent), allData(data)
{
}

int AccountsModel::rowCount(const QModelIndex &) const
{
  return allData.accounts.size();
}

int AccountsModel::columnCount(const QModelIndex &) const
{
  return 3;
}

QVariant AccountsModel::data(const QModelIndex &index, int role) const
{
  if(!index.isValid()) {
    return QVariant();
  }

  if(role == Qt::DisplayRole) {
    switch(index.column()) {
    case 0:
      return allData.accounts.at(index.row()).id;
      break;
    case 1:
      return allData.accounts.at(index.row()).balance;
      break;
    case 2:
      return allData.accounts.at(index.row()).bonus;
      break;
      /*
    case 3:
      return allData.accounts.at(index.row()).barcode;
      break;
      */
    };
  } else if(role == Qt::DecorationRole) {
    if(index.column() == 0) {
      return ImgTools::getPreparedIcon(QPixmap("graphics/account.png").scaled(allData.iconSize, allData.iconSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation), allData.iconSize);
    }
  } else if(role == Qt::BackgroundRole) {
    switch(index.column()) {
    case 1:
      if(allData.accounts.at(index.row()).balance < 0.0) {
        return QBrush(QColor(242, 169, 169));
      }
      break;
    }
    /*
  } else if(role == Qt::ForegroundRole) {
    switch(index.column()) {
    case 0:
      return QBrush(QColor(255, 255, 255));
      break;
    }
    */
  }
  /*
  } else if(role == Qt::ForegroundRole) {
    if(allData.accounts[index.row()].values[index.column()].writeable) {
      if(allData.accounts[index.row()].values[index.column()].eye == DB::E_OD) {
        return QVariant(QBrush(QColor(0, 80, 0)));
      } else if(allData.accounts[index.row()].values[index.column()].eye == DB::E_OS) {
        return QVariant(QBrush(QColor(128, 0, 0)));
      }
    } else {
      return QVariant(QBrush(QColor(128, 128, 128)));
    }
  } else if(role == Qt::BackgroundRole) {
    if(allData.accounts[index.row()].values[index.column()].eye == DB::E_OD) {
      return QVariant(QBrush(QColor(242, 255, 242)));
    } else if(allData.accounts[index.row()].values[index.column()].eye == DB::E_OS) {
      return QVariant(QBrush(QColor(255, 242, 242)));
    }
  } else if(role == Qt::ToolTipRole &&
            allData.accounts[index.row()].values[index.column()].conclusionUid != DB::NONE) {
    return QVariant(allData.accounts[index.row()].values[index.column()].conclusionExaminer);
    */
  return QVariant();
}

/*
bool AccountsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if(!index.isValid() ||
     role != Qt::EditRole) {
    return false;
  }
    return true;
}
*/

QVariant AccountsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if(role == Qt::DisplayRole) {
    if(orientation == Qt::Horizontal) {
      switch(section) {
      case 0:
        return tr("Account");
        break;
      case 1:
        return tr("Balance");
        break;
      case 2:
        return tr("Bonus");
        break;
        /*
      case 3:
        return tr("Barcode");
        break;
        */
      default:
        return tr("Unknown");
      };
    }
  }
  return QVariant();
}

Qt::ItemFlags AccountsModel::flags(const QModelIndex &index) const
{
  if(!index.isValid()) {
    return Qt::ItemIsEnabled;
  }

  /*
  if(index.column() == 2 ||
     index.column() == 3) {
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
  }
  */
  
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void AccountsModel::beginInsertRow(int row)
{
  beginInsertRows(QModelIndex(), row, row);
}

bool AccountsModel::insertRows(int, int, const QModelIndex &)
{
  endInsertRows();
  return true;
}

void AccountsModel::refreshAll()
{
  beginResetModel();
  endResetModel();
}
