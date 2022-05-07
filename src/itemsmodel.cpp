/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            itemsmodel.cpp
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

#include "itemsmodel.h"

#include <QIcon>
#include <QBrush>

ItemsModel::ItemsModel(QList<Item> &items,
                       const QList<Category> &categories,
                       const QMap<QString, QIcon> &icons,
                       QObject *parent)
  : QAbstractTableModel(parent), items(items), categories(categories), icons(icons)
{
}

int ItemsModel::rowCount(const QModelIndex &) const
{
  return items.size();
}

int ItemsModel::columnCount(const QModelIndex &) const
{
  if(items.isEmpty()) {
    return 0;
  }
  return 7;
}

QVariant ItemsModel::data(const QModelIndex &index, int role) const
{
  if(!index.isValid()) {
    return QVariant();
  }

  if(role == Qt::DisplayRole) {
    switch(index.column()) {
      // No case 0 as it is an icon and handled by Qt::DecorationRole below
    case 0:
      return items.at(index.row()).id;
      break;
    case 1:
      for(const auto &category: categories) {
        if(category.barcode == items.at(index.row()).category) {
          return category.id;
        }
      }
      break;
    case 2:
      return items.at(index.row()).price;
      break;
    case 3:
      return items.at(index.row()).discount;
      break;
    case 4:
      return items.at(index.row()).stock;
      break;
    case 5:
      return items.at(index.row()).age;
      break;
    case 6:
      return items.at(index.row()).barcode;
      break;
    };
  } else if(role == Qt::DecorationRole) {
    switch(index.column()) {
    case 0:
      return icons[items.at(index.row()).icon].pixmap(48, 48);
      break;
    case 1:
      for(const auto &category: categories) {
        if(category.barcode == items.at(index.row()).category) {
          return icons[category.icon].pixmap(48, 48);
        }
      }
      break;
    }
  } else if(role == Qt::EditRole) {
      //itemsTab->refreshItems();
    /*
    switch(index.column()) {
    case 0:
      return items.at(index.row()).id;
      break;
    case 1:
      return items.at(index.row()).category;
      break;
    case 2:
      return items.at(index.row()).price;
      break;
    case 3:
      return items.at(index.row()).discount;
      break;
    case 4:
      return items.at(index.row()).stock;
      break;
    case 5:
      return items.at(index.row()).age;
      break;
    case 6:
      return items.at(index.row()).barcode;
      break;
    };
    */
    //dataChanged();
  }
  /*
  } else if(role == Qt::ForegroundRole) {
    if(items[index.row()].values[index.column()].writeable) {
      if(items[index.row()].values[index.column()].eye == DB::E_OD) {
        return QVariant(QBrush(QColor(0, 80, 0)));
      } else if(items[index.row()].values[index.column()].eye == DB::E_OS) {
        return QVariant(QBrush(QColor(128, 0, 0)));
      }
    } else {
      return QVariant(QBrush(QColor(128, 128, 128)));
    }
  } else if(role == Qt::BackgroundRole) {
    if(items[index.row()].values[index.column()].eye == DB::E_OD) {
      return QVariant(QBrush(QColor(242, 255, 242)));
    } else if(items[index.row()].values[index.column()].eye == DB::E_OS) {
      return QVariant(QBrush(QColor(255, 242, 242)));
    }
  } else if(role == Qt::ToolTipRole &&
            items[index.row()].values[index.column()].conclusionUid != DB::NONE) {
    return QVariant(items[index.row()].values[index.column()].conclusionExaminer);
    */
  return QVariant();
}

/*
bool ItemsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if(!index.isValid() ||
     role != Qt::EditRole) {
    return false;
  }
    return true;
}
*/

QVariant ItemsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if(!items.isEmpty()) {
    if(role == Qt::DisplayRole) {
      if(orientation == Qt::Horizontal) {
        switch(section) {
        case 0:
          return tr("Item");
          break;
        case 1:
          return tr("Category");
          break;
        case 2:
          return tr("Price");
          break;
        case 3:
          return tr("Discount");
          break;
        case 4:
          return tr("Stock");
          break;
        case 5:
          return tr("Age");
          break;
        case 6:
          return tr("Barcode");
          break;
        default:
          return tr("Unknown");
        };
      }
    } else if(role == Qt::DecorationRole) {
      if(orientation == Qt::Vertical) {
        //return icons[items.at(section).icon].pixmap(60, 60);
      }
    }
  }
  return QVariant();
}

Qt::ItemFlags ItemsModel::flags(const QModelIndex &index) const
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
