/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            itemdata.cpp
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

#include "itemdata.h"

ItemCellData::ItemCellData(const ItemCellData &data) : QObject()
{
  this->key = data.key;
  this->value = data.value;
  this->undoValue = data.undoValue;
  this->writeable = data.writeable;
  //this->eye = data.eye;
  //this->conclusionUid = data.conclusionUid;
  //this->conclusionExaminer = data.conclusionExaminer;
  //int conclusionNextItem = MMK::NONE;
  this->dirty = data.dirty;
}

void ItemCellData::operator=(const ItemCellData &data)
{
  this->key = data.key;
  this->value = data.value;
  this->undoValue = data.undoValue;
  this->writeable = data.writeable;
  //this->eye = data.eye;
  //this->conclusionUid = data.conclusionUid;
  //this->conclusionExaminer = data.conclusionExaminer;
  //int conclusionNextItem = MMK::NONE;
  this->dirty = data.dirty;
}

ItemCellData::ItemCellData()
{
}

ItemData::ItemData(const ItemData &data) : QObject()
{
  this->itemUid = data.itemUid;
  this->dateTime = data.dateTime;
  //this->examiner = data.examiner;
  //this->odUid = data.odUid;
  //this->osUid = data.osUid;
  this->values = data.values;
}

void ItemData::operator=(const ItemData &data)
{
  this->itemUid = data.itemUid;
  this->dateTime = data.dateTime;
  //this->examiner = data.examiner;
  //this->odUid = data.odUid;
  //this->osUid = data.osUid;
  this->values = data.values;
}

ItemData::ItemData(const int &itemUid)
{
  this->itemUid = itemUid;
}

void ItemData::addValue(const ItemCellData &data)
{
  values.append(data);
}

/*
void ItemData::setOdUid(const int &eyeUid)
{
  odUid = eyeUid;
}
*/
 /*
void ItemData::setOsUid(const int &eyeUid)
{
  osUid = eyeUid;
}
 */

void ItemData::setDateTime(const QDateTime &dateTime)
{
  this->dateTime = dateTime;
}

/*
void ItemData::setExaminer(const QString &examiner)
{
  this->examiner = examiner;
}
*/

int ItemData::getItemUid() const
{
  return itemUid;
}

QDateTime ItemData::getDateTime() const
{
  return dateTime;
}

/*
QString ItemData::getExaminer() const
{
  return examiner;
}
*/
/*
int ItemData::getOdUid() const
{
  return odUid;
}
*/
/*
int ItemData::getOsUid() const
{
  return osUid;
}
*/
int ItemData::getTypeFromCell(const ItemCellData &cell)
{
  if(cell.key == "date") {
    return QMetaType::QDate;
  }
  return MMK::NONE;
}

QString ItemData::getHeaderFromCell(const ItemCellData &cell)
{
  if(cell.key == "date") {
    return tr("Due date");
  }
  return cell.key;
}
