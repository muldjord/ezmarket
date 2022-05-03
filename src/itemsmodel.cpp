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
#include "globaldefs.h"
#include "mainwindow.h"

#include <math.h>

#include <QMainWindow>
#include <QMessageBox>
#include <QRegularExpression>

extern MainWindow *mainWindow;
extern ConnStatus *connStatus;

ItemsModel::ItemsModel(QList<Item> &items, QObject *parent)
  : QAbstractTableModel(parent), items(items)
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
  return items[0].values.size();
}

QVariant ItemsModel::data(const QModelIndex &index, int role) const
{
  if(!index.isValid()) {
    return QVariant();
  }

  if(role == Qt::DisplayRole) {
    return QVariant(items[index.row()].values[index.column()].value.toString());
  } else if(role == Qt::EditRole) {
    return QVariant(items[index.row()].values[index.column()].value.toString());
  } else if(role == Qt::ForegroundRole) {
    if(!items[index.row()].values[index.column()].writeable) {
      return QVariant(QBrush(QColor(128, 128, 128)));
    }
  } else if(role == Qt::BackgroundRole) {
    /*
    if(items[index.row()].values[index.column()].eye == MMK::E_OD) {
      return QVariant(QBrush(QColor(242, 255, 242)));
    } else if(items[index.row()].values[index.column()].eye == MMK::E_OS) {
      return QVariant(QBrush(QColor(255, 242, 242)));
    }
    */
  } else if(role == Qt::ToolTipRole) {
    //return QVariant(items[index.row()].values[index.column()].conclusionExaminer);
  }
  return QVariant();
}

QVariant ItemsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if(!items.isEmpty()) {
    if(role == Qt::DisplayRole) {
      if(orientation == Qt::Horizontal) {
        switch(section) {
        case 0:
          return tr("Icon");
        case 1:
          return tr("Id");
        case 2:
          return tr("Category");
        case 3:
          return tr("Price");
        case 4:
          return tr("Discount");
        case 5:
          return tr("Stock");
        case 6:
          return tr("Icon");
        case 7:
          return tr("Barcode");
        default:
          return "NA";
        };
      } else {
        //return items[section].getDateTime().toString("yyyy-MM-dd");
        return QVariant();
      }
    } else if(role == Qt::ToolTipRole) {
      /*
      if(orientation == Qt::Vertical) {
        return items[section].getExaminer();
      }
      */
    }
  }
  return QVariant();
}

Qt::ItemFlags ItemsModel::flags(const QModelIndex &index) const
{
  if(!index.isValid()) {
    return Qt::ItemIsEnabled;
  }

  if(items[index.row()].values[index.column()].writeable) {
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
  }

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool ItemsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if(!index.isValid() ||
     role != Qt::EditRole ||
     !items[index.row()].values[index.column()].writeable ||
     items[index.row()].values[index.column()].undoValue == value) {
    return false;
  }

  printf("TYPE: %d\n", value.type());
  
  QVariant oldValue = items[index.row()].values[index.column()].value;
  
  int cellType = ItemsData::getTypeFromCell(items[index.row()].values[index.column()]);
  if(cellType != MMK::NONE) {
    if(value.toString().isEmpty()) {
      items[index.row()].values[index.column()].value = QVariant();
    } else {
      if(cellType == QMetaType::Int &&
         QRegularExpression("^[0-9]{0,4}$").match(value.toString()).hasMatch()) {
        items[index.row()].values[index.column()].value = QVariant(value.toInt());
      } else if(cellType == QMetaType::Double &&
                QRegularExpression("^[0-9]{1,4}(\\.|,){0,1}[0-9]{0,4}$").match(value.toString()).hasMatch()) {
        items[index.row()].values[index.column()].value = QVariant(value.toString().replace(",", ".").toDouble());
      } else if(cellType == QMetaType::QString &&
                QRegularExpression("^.{0,42}$").match(value.toString()).hasMatch()) {
        items[index.row()].values[index.column()].value = QVariant(value.toString());
      } else if(cellType == QMetaType::QDate &&
                QRegularExpression("^[12]{1}[09]{1}[0-9]{2}-[01]{1}[0-9]{1}-[0-3]{1}[0-9]{1}$").match(value.toString()).hasMatch() &&
                QDate::fromString(value.toString(), "yyyy-MM-dd").isValid()) {
        items[index.row()].values[index.column()].value = QVariant(value.toString());
      } else {
        QString typeHelp = "";
        if(cellType == QMetaType::Int) {
          typeHelp = tr("An integer value is required (eg '42').");
        } else if(cellType == QMetaType::Double) {
          typeHelp = tr("A decimal value is required. (eg '42.6').");
        } else if(cellType == QMetaType::QString) {
          typeHelp = tr("A string value is required (eg 'Some text'). The maximum length is 42.");
        } else if(cellType == QMetaType::QDate) {
          typeHelp = tr("A date of the format 'yyyy-mm-dd' is required (eg '2019-06-31').");
        }
        QMessageBox::critical(nullptr, tr("Invalid data format"), tr("The entered format is invalid.") + " " + typeHelp + "\n\n" + tr("Please try again."), QMessageBox::Ok);
        return false;
      }
    }
    items[index.row()].values[index.column()].dirty = true;

    mainWindow->setEnabled(false);
    NetBlocker blocker;
    connect(connStatus, &ConnStatus::gotItemsUpdate, &blocker, &NetBlocker::gotItemsUpdate);

    emit updateItems(items[index.row()]);

    // Reset all to undirty now that the update request has been sent
    for(auto &otherValue: items[index.row()].values) {
      otherValue.dirty = false;
    }
    //items[index.row()].values[index.column()].dirty = false;

    blocker.exec(); // Wait for answer from server
    if(blocker.success()) {
      emit dataChanged(index, index); // Inform the model that this index has changed
      mainWindow->setEnabled(true);
      // Update undoValue so cell is only updated if it's changed, see top of this function
      items[index.row()].values[index.column()].undoValue = value;
      return true;
    }
  }
  qWarning("Cell value update failed!\n");
  QMessageBox::critical(nullptr, tr("Value update failed"), tr("Cell value could not be updated, probably due to server communication problems. Please contact a system administrator before continuing"), QMessageBox::Ok);
  // Revert value, something went wrong!
  items[index.row()].values[index.column()].value = oldValue;
  return false;
}

QList<ItemsData> ItemsModel::getRows(const QModelIndexList &indexes) const
{
  QList<ItemsData> rows;
  for(const auto &index: indexes) {
    // Only process each row once, so only process when at column == 0
    if(index.column() == 0) {
      rows.append(items.at(index.row()));
      continue;
    }
  }
  return rows;
}

int ItemsModel::getFocusRow(const int &itemsUid) const
{
  if(itemsUid != MMK::NONE) {
    for(int a = 0; a < items.count(); ++a) {
      if(items.at(a).getItemsUid() == itemsUid) {
        return a;
      }
    }
  }
  return MMK::NONE;
}
