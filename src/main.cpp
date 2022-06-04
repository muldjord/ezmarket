/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            main.cpp
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

#include <QApplication>
#include <QDir>
#include <QTranslator>
#include <QStyleFactory>
#include <QLibraryInfo>
#include <QSharedMemory>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  // Only allow one instance
  QSharedMemory shared("62d60669-bb94-4a94-88bb-b964890a7e04");
  if(!shared.create(512, QSharedMemory::ReadWrite)) {
    exit(0);
  }

  app.setStyle(QStyleFactory::create("Fusion"));
  
  QDir::setCurrent(QApplication::applicationDirPath());

  //QLocale::setDefault(QLocale::system().name());
  QLocale::setDefault(QString("da_DK"));

  QTranslator translator;
  translator.load("ezmarket_" + QLocale().name());
  app.installTranslator(&translator);

  QTranslator qtBaseTranslator;
  qtBaseTranslator.load("qtbase_" + QLocale().name(),
                        QLibraryInfo::location(QLibraryInfo::TranslationsPath));
  app.installTranslator(&qtBaseTranslator);

  MainWindow window;
  window.show();
  return app.exec();
}

