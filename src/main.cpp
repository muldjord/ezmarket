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
#include <QSettings>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QDir::setCurrent(QApplication::applicationDirPath());

  QString locale = "en_US";
  QSettings settings("config.ini", QSettings::IniFormat);
  if(!settings.contains("main/locale")) {
    settings.setValue("main/locale", QLocale::system().name());
  } else {
    locale = settings.value("main/locale", "en_US").toString();
  }
  if(!settings.contains("ui/iconSize")) {
    settings.setValue("ui/iconSize", 48);
  }
  if(!settings.contains("ui/iconSizeSmall")) {
    settings.setValue("ui/iconSizeSmall", 35);
  }
  if(!settings.contains("ui/fontSize")) {
    settings.setValue("ui/fontSize", 35);
  }
  if(!settings.contains("ui/fontSizeSmall")) {
    settings.setValue("ui/fontSizeSmall", 30);
  }
  if(!settings.contains("ui/editorWidth")) {
    settings.setValue("ui/editorWidth", 700);
  }
  if(!settings.contains("ui/editorHeight")) {
    settings.setValue("ui/editorHeight", 450);
  }

  QTranslator translator;
  if(translator.load("translations/ezmarket-" + locale)) {
    app.installTranslator(&translator);
  }

  QTranslator qtBaseTranslator;
  if(qtBaseTranslator.load("qtbase_" + locale, QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
    app.installTranslator(&qtBaseTranslator);
  }

  MainWindow window(settings);
  window.show();
  return app.exec();
}

