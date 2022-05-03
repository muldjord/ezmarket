/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            itemdata.h
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

#ifndef __ITEMDATA_H__
#define __ITEMDATA_H__

#include <QObject>
#include <QVariant>
#include <QDateTime>
#include <QTime>

class ItemCellData : public QObject
{
Q_OBJECT

public:
  ItemCellData(const ItemCellData &data);
  void operator=(const ItemCellData &data);
  ItemCellData();
  QString key = QString();
  QVariant value = QString();
  QVariant undoValue = QString();
  bool writeable = false;
  //int eye = MMK::NONE;
  //int conclusionUid = MMK::NONE;
  //QString conclusionExaminer = QString();
  //int conclusionNextItem = MMK::NONE;
  bool dirty = false;
};

class ItemData : public QObject
{
Q_OBJECT

public:
  ItemData(const ItemData &data);
  void operator=(const ItemData &data);
  ItemData(const int &itemUid);
  //void setOdUid(const int &eyeUid);
  //void setOsUid(const int &eyeUid);
  void setDateTime(const QDateTime &dateTime);
  //void setExaminer(const QString &examiner);
  void addValue(const ItemCellData &data);
  int getItemUid() const;
  //int getOdUid() const;
  //int getOsUid() const;
  QDateTime getDateTime() const;
  //QString getExaminer() const;
  //QVariant getValue(const QString &key);
  static int getTypeFromCell(const ItemCellData &cell);
  static QString getHeaderFromCell(const ItemCellData &cell);

  QList<ItemCellData> values;

private:
  int itemUid = MMK::NONE;
  //int odUid = MMK::NONE;
  //int osUid = MMK::NONE;
  QDateTime dateTime = QDateTime(QDate(1800, 1, 1), QTime(12, 0, 0, 0));
  //QString examiner = QString();
  //QList<QString> projects;
  
};

#endif // __ITEMDATA_H__
