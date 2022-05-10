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
#include <QPainter>
#include <QBrush>

ItemsModel::ItemsModel(Data &data,
                       QObject *parent)
  : QAbstractTableModel(parent), allData(data)
{
}

int ItemsModel::rowCount(const QModelIndex &) const
{
  return allData.items.size();
}

int ItemsModel::columnCount(const QModelIndex &) const
{
  return 6;
}

QVariant ItemsModel::data(const QModelIndex &index, int role) const
{
  if(!index.isValid()) {
    return QVariant();
  }

  if(role == Qt::DisplayRole) {
    switch(index.column()) {
    case 0:
      return allData.items.at(index.row()).id;
      break;
    case 1:
      for(const auto &category: allData.categories) {
        if(category.barcode == allData.items.at(index.row()).category) {
          return category.id;
        }
      }
      break;
    case 2:
      return allData.items.at(index.row()).price;
      break;
    case 3:
      return allData.items.at(index.row()).discount;
      break;
    case 4:
      return allData.items.at(index.row()).stock;
      break;
    case 5:
      return allData.items.at(index.row()).age;
      break;
      /*
    case 6:
      return allData.items.at(index.row()).barcode;
      break;
      */
    };
  } else if(role == Qt::DecorationRole) {
    if(index.column() == 0) {
      return getPreparedIcon(allData.icons[allData.items.at(index.row()).icon]);
    } else if(index.column() == 1) {
      for(const auto &category: allData.categories) {
        if(category.barcode == allData.items.at(index.row()).category) {
          return getPreparedIcon(allData.icons[category.icon]);
        }
      }
    }
    /*
  } else if(role == Qt::ForegroundRole) {
    switch(index.column()) {
    case 0:
      return QBrush(QColor(255, 255, 255));
      break;
    }
  } else if(role == Qt::BackgroundRole) {
    switch(index.column()) {
    case 0:
      return QBrush(QColor(0, 0, 0));
      break;
    }
    */
  }
  /*
  } else if(role == Qt::ForegroundRole) {
    if(allData.items[index.row()].values[index.column()].writeable) {
      if(allData.items[index.row()].values[index.column()].eye == DB::E_OD) {
        return QVariant(QBrush(QColor(0, 80, 0)));
      } else if(allData.items[index.row()].values[index.column()].eye == DB::E_OS) {
        return QVariant(QBrush(QColor(128, 0, 0)));
      }
    } else {
      return QVariant(QBrush(QColor(128, 128, 128)));
    }
  } else if(role == Qt::BackgroundRole) {
    if(allData.items[index.row()].values[index.column()].eye == DB::E_OD) {
      return QVariant(QBrush(QColor(242, 255, 242)));
    } else if(allData.items[index.row()].values[index.column()].eye == DB::E_OS) {
      return QVariant(QBrush(QColor(255, 242, 242)));
    }
  } else if(role == Qt::ToolTipRole &&
            allData.items[index.row()].values[index.column()].conclusionUid != DB::NONE) {
    return QVariant(allData.items[index.row()].values[index.column()].conclusionExaminer);
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
        /*
      case 6:
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

void ItemsModel::beginInsertRow(int row)
{
  beginInsertRows(QModelIndex(), row, row);
}

bool ItemsModel::insertRows(int, int, const QModelIndex &)
{
  endInsertRows();
  return true;
}

void ItemsModel::refreshAll()
{
  beginResetModel();
  endResetModel();
}

QPixmap ItemsModel::getPreparedIcon(const QPixmap &icon) const
{
  QImage image(icon.width() + 2, icon.height() + 2, QImage::Format_ARGB32);
  image.fill(Qt::transparent);
  QPainter painter(&image);
  QBrush brush(Qt::SolidPattern);
  brush.setColor(Qt::black);
  painter.setBrush(brush);
  painter.drawRoundedRect(0, 0, icon.width() + 2, icon.height() + 2, 30, 30, Qt::RelativeSize);
  painter.drawPixmap(1, 1, icon);
  painter.end();
  /*
  QImage image(96, 96, QImage::Format_ARGB32);
  image.fill(Qt::transparent);
  QPainter painter(&image);
  QBrush brush(Qt::SolidPattern);
  brush.setColor(Qt::black);
  painter.setBrush(brush);
  painter.drawEllipse(0, 0, 95, 95);
  painter.end();
  image = image.scaled(data.iconSize8, data.iconSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  painter.begin(&image);
  painter.drawPixmap(0, 0, icon.pixmap(data.iconSize, data.iconSize));
  painter.end();
  */

  return QPixmap::fromImage(image);
}
