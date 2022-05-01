/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            booklist.cpp
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

#include <stdio.h>

#include "booklist.h"

BookList::BookList(QList<Book> &books, QWidget *parent)
  : QListWidget(parent), books(books)
{
  refreshTimer.setInterval(1000);
  refreshTimer.setSingleShot(true);
  connect(&refreshTimer, &QTimer::timeout, this, &BookList::refreshBooks);
  refreshTimer.start();

  elapsedTime.start();
  
  connect(this, &BookList::itemPressed, this, &BookList::bookSelected);
}

BookList::~BookList()
{
}

void BookList::refreshBooks()
{
  printf("REFRESHING BOOKS!\n");
  clear();

  for(auto &book: books) {
    if(!book.loanedBy.isEmpty()) {
      book.loanedTimer += elapsedTime.elapsed();
      printf("Book: %s, loaned time: %d\n", qPrintable(book.title), book.loanedTimer);
    } else {
      book.loanedTimer = 0.0;
    }
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(book.title + " (" + book.barcode + ")");
    if(book.loanedTimer > 240 * 1000) {
      item->setForeground(Qt::red);
    } else if(!book.reservedBy.isEmpty()) {
      item->setForeground(Qt::blue);
    } else if(!book.loanedBy.isEmpty()) {
      item->setForeground(Qt::gray);
    }
    item->setData(Qt::UserRole, book.barcode);
    addItem(item);
  }
  elapsedTime.restart();
  refreshTimer.start();
}

void BookList::bookSelected(QListWidgetItem *item)
{
  emit focusBook(item->data(Qt::UserRole).toString());
  refreshTimer.start();
}
