/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            mainwindow.cpp
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

#include "mainwindow.h"
#include "aboutbox.h"
#include "newentry.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QApplication>
#include <QFile>
#include <QSound>
#include <QRandomGenerator>
#include <QDir>
#include <QToolBar>

MainWindow::MainWindow()
{
  setWindowTitle("EZMarket v" VERSION);
  showFullScreen();

  setStyleSheet("QLabel {font-size: 25px;}"
                "QLineEdit {font-size: 25px;}"
                "QListWidget {font-size: 25px;}"
                "QGroupBox{font-size: 30px; padding-left: 20px; padding-top: 40px; padding-bottom: 20px; padding-right: 20px;}");
  /*
  setStyleSheet("QLabel {font-size: 25px;}"
                "QLineEdit {font-size: 25px; border-image: url(:translucent_background.png);}"
                "QListWidget {font-size: 25px; border-image: url(:translucent_background.png);}"
                "QGroupBox{font-size: 30px; padding-left: 20px; padding-top: 40px; padding-bottom: 20px; padding-right: 20px;}");
  */
  createActions();
  createMenus();
  createToolBar();
  
  loadDatabase();
  
  QLabel *personBarcodeLabel = new QLabel(tr("Barcode:"));
  personBarcodeLineEdit = new QLineEdit(this);
  QLabel *personNameLabel = new QLabel(tr("Name:"));
  personNameLineEdit = new QLineEdit(this);
  QLabel *personOnLoanLabel = new QLabel(tr("On loan:"));
  personOnLoanList = new QListWidget(this);
  QLabel *personReservedLabel = new QLabel(tr("Reserved:"));
  personReservedList = new QListWidget(this);
  
  QVBoxLayout *personLayout = new QVBoxLayout();
  personLayout->addWidget(personBarcodeLabel);
  personLayout->addWidget(personBarcodeLineEdit);
  personLayout->addWidget(personNameLabel);
  personLayout->addWidget(personNameLineEdit);
  personLayout->addWidget(personOnLoanLabel);
  personLayout->addWidget(personOnLoanList);
  personLayout->addWidget(personReservedLabel);
  personLayout->addWidget(personReservedList);

  personGroup = new QGroupBox(tr("Person"));
  personGroup->setStyleSheet("QGroupBox {background-image: url(:person_background.png);}");
  personGroup->setLayout(personLayout);
  
  QLabel *bookBarcodeLabel = new QLabel(tr("Barcode:"));
  bookBarcodeLineEdit = new QLineEdit(this);
  QLabel *bookTitleLabel = new QLabel(tr("Title:"));
  bookTitleLineEdit = new QLineEdit(this);
  QLabel *bookLoanedByLabel = new QLabel(tr("Loaned by:"));
  bookLoanedByLineEdit = new QLineEdit(this);
  QLabel *bookReservedByLabel = new QLabel(tr("Reserved by:"));
  bookReservedByLineEdit = new QLineEdit(this);
  
  QVBoxLayout *bookLayout = new QVBoxLayout();
  bookLayout->addWidget(bookBarcodeLabel);
  bookLayout->addWidget(bookBarcodeLineEdit);
  bookLayout->addWidget(bookTitleLabel);
  bookLayout->addWidget(bookTitleLineEdit);
  bookLayout->addWidget(bookLoanedByLabel);
  bookLayout->addWidget(bookLoanedByLineEdit);
  bookLayout->addWidget(bookReservedByLabel);
  bookLayout->addWidget(bookReservedByLineEdit);
  //bookLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Ignored, QSizePolicy::Expanding));

  bookGroup = new QGroupBox(tr("Book"));
  bookGroup->setStyleSheet("QGroupBox {background-image: url(:book_background.png);}");
  bookGroup->setLayout(bookLayout);

  allBooksList = new BookList(books, this);
  connect(allBooksList, &BookList::focusBook, this, &MainWindow::focusBook);

  QVBoxLayout *allBooksLayout = new QVBoxLayout();
  //personLayout->addWidget(personBarcodeLabel);
  //personLayout->addWidget(personBarcodeLineEdit);
  allBooksLayout->addWidget(allBooksList);

  allBooksGroup = new QGroupBox(tr("All books"));
  allBooksGroup->setStyleSheet("QGroupBox {background-image: url(:book_background.png);}");
  allBooksGroup->setLayout(allBooksLayout);
  
  /*
  QTabWidget *tabs = new QTabWidget(this);
  tabs->addTab(personGroup, tr("Person"));
  tabs->addTab(bookGroup, tr("Book"));
  */

  QVBoxLayout *booksLayout = new QVBoxLayout();
  booksLayout->addWidget(bookGroup);
  booksLayout->addWidget(allBooksGroup);

  QHBoxLayout *dataLayout = new QHBoxLayout();
  dataLayout->addWidget(personGroup);
  dataLayout->addLayout(booksLayout);

  QVBoxLayout *layout = new QVBoxLayout();
  //layout->addWidget(barcodeLineEdit);
  layout->addLayout(dataLayout);

  setCentralWidget(new QWidget());
  centralWidget()->setLayout(layout);

  randomTimer.setSingleShot(false);
  connect(&randomTimer, &QTimer::timeout, this, &MainWindow::initRandomSound);
  initRandomSound();

  clearTimer.setInterval(5000);
  clearTimer.setSingleShot(true);
  connect(&clearTimer, &QTimer::timeout, this, &MainWindow::clearAll);

  focusTimer.setInterval(500);
  focusTimer.setSingleShot(false);
  connect(&focusTimer, &QTimer::timeout, this, &MainWindow::focusBarcodeLineEdit);
  focusTimer.start();

  show();
}

MainWindow::~MainWindow()
{
  saveDatabase();
}

void MainWindow::createActions()
{
  quitAct = new QAction("&Quit", this);
  quitAct->setIcon(QIcon("graphics/quit.png"));
  connect(quitAct, &QAction::triggered, qApp, &QApplication::quit);

  aboutAct = new QAction(tr("&About..."), this);
  aboutAct->setIcon(QIcon("graphics/about.png"));
  connect(aboutAct, &QAction::triggered, this, &MainWindow::showAbout);
}

void MainWindow::createMenus()
{
  fileMenu = new QMenu(tr("&File"), this);
  fileMenu->addAction(quitAct);

  helpMenu = new QMenu(tr("&Help"), this);
  helpMenu->addAction(aboutAct);

  menuBar = new QMenuBar();
  menuBar->addMenu(fileMenu);
  menuBar->addMenu(helpMenu);

  setMenuBar(menuBar);
}

void MainWindow::createToolBar()
{
  barcodeLineEdit = new QLineEdit(this);
  barcodeLineEdit->setStyleSheet("QLineEdit {background-image: url(graphics/barcode_background.png);}");
  barcodeLineEdit->setMaximumHeight(48);
  barcodeLineEdit->setMaximumWidth(300);
  barcodeLineEdit->setPlaceholderText(tr("Barcode"));
  connect(barcodeLineEdit, &QLineEdit::returnPressed, this, &MainWindow::checkBarcode);

  /*
  QPushButton *chooseWorkDirButton = new QPushButton();
  chooseWorkDirButton->setIcon(QIcon("graphics/choose_workdir.png"));
  chooseWorkDirButton->setIconSize(QSize(64, 64));
  chooseWorkDirButton->setToolTip(tr("Choose working directory"));
  */

  QToolBar *toolBar = new QToolBar(tr("Main functions"));
  toolBar->setMinimumHeight(64);
  toolBar->setStyleSheet("QToolBar {background-image: url(graphics/marquee.png); border: 0px;}");

  /*
  QPushButton *chooseWorkDirButton = new QPushButton();
  chooseWorkDirButton->setIcon(QIcon("graphics/choose_workdir.png"));
  chooseWorkDirButton->setIconSize(QSize(64, 64));
  chooseWorkDirButton->setToolTip(tr("Choose working directory"));
  
  //connect(chooseWorkDirButton, &QPushButton::clicked, this, &MainWindow::);
  
  toolBar->addWidget(chooseWorkDirButton);
  */
  toolBar->addWidget(barcodeLineEdit);
  toolBar->addSeparator();

  addToolBar(Qt::TopToolBarArea, toolBar);
}

void MainWindow::loadDatabase()
{
  QFile database("database.dat");
  int state = -1;
  if(database.open(QIODevice::ReadOnly)) {
    while(!database.atEnd()) {
      QString string = QString::fromUtf8(database.readLine().trimmed());
      if(string.isEmpty() ||
         string.left(1) == "#") {
        continue;
      } else if(string == "persons:") {
        state = 0;
        continue;
      } else if(string == "books:") {
        state = 1;
        continue;
      }
      if(state == 0) {
        parsePerson(string);
      } else if(state == 1) {
        parseBook(string);
      }
    }
    database.close();
  }
}

void MainWindow::parsePerson(const QString &string)
{
  Person person;
  QMap<QString, QString> vars;
  for(const auto &snippet: string.split(";")) {
    if(snippet.contains("=")) {
      vars[snippet.split("=").first()] = snippet.split("=").last();
    }
  }
  if(vars.contains("name")) {
    person.name = vars["name"];
  }
  if(vars.contains("barcode")) {
    person.barcode = vars["barcode"];
  }

  printf("Person:\n  barcode=%s\n  name=%s\n",
         qPrintable(person.barcode),
         qPrintable(person.name));
  
  persons.append(person);
}

void MainWindow::parseBook(const QString &string)
{
  Book book;
  QMap<QString, QString> vars;
  for(const auto &snippet: string.split(";")) {
    if(snippet.contains("=")) {
      vars[snippet.split("=").first()] = snippet.split("=").last();
    }
  }
  if(vars.contains("barcode")) {
    book.barcode = vars["barcode"];
  }
  if(vars.contains("title")) {
    book.title = vars["title"];
  }
  if(vars.contains("loanedby")) {
    book.loanedBy = vars["loanedby"];
  }
  if(vars.contains("loanedtimer")) {
    book.loanedTimer = vars["loanedtimer"].toInt();
  }
  if(vars.contains("reservedby")) {
    book.reservedBy = vars["reservedby"];
  }

  printf("Book:\n  barcode=%s\n  title=%s\n  loanedBy=%s\n  loanedTimer=%d\n  reservedBy=%s\n",
         qPrintable(book.barcode),
         qPrintable(book.title),
         qPrintable(book.loanedBy),
         book.loanedTimer / 1000,
         qPrintable(book.reservedBy));

  books.append(book);
}

void MainWindow::saveDatabase()
{
  if(persons.isEmpty() && books.isEmpty()) {
    return;
  }

  if(QFile::exists("database.dat")) {
    QString baseName = "database0000";
    int serial = -1;
    do {
      serial++;
      baseName = baseName.left(baseName.length() - 4);
      QString serialString = QString::number(serial);
      while(serialString.length() < 4) {
        serialString.prepend("0");
      }
      baseName.append(serialString);
    } while(QFile::exists(baseName + ".dat"));
    QFile::rename("database.dat", baseName + ".dat");
  }
  QFile database("database.dat");
  if(database.open(QIODevice::WriteOnly)) {
    database.write("persons:\n");
    for(const auto &person: persons) {
      database.write(QString("barcode=" + person.barcode + ";name=" + person.name + "\n").toUtf8());
    }
    database.write("\n");
    database.write("books:\n");
    for(const auto &book: books) {
      database.write(QString("barcode=" + book.barcode + ";title=" + book.title + ";loanedby=" + book.loanedBy + ";loanedtimer=" + QString::number(book.loanedTimer) + ";reservedby=" + book.reservedBy + "\n").toUtf8());
    }
    database.close();
  }
}

void MainWindow::showAbout()
{
  // Spawn about window
  AboutBox aboutBox(this);
  aboutBox.exec();
}

void MainWindow::checkBarcode()
{
  clearTimer.stop();
  
  QString barcode = barcodeLineEdit->text().toLower();
  barcodeLineEdit->setText("");
  if(barcode.isEmpty()) {
    return;
  }
  
  printf("CHECKING BARCODE: '%s'\n", qPrintable(barcode));
  
  bool barcodeFound = false;
  for(const auto &person: persons) {
    if(barcode == person.barcode) {
      printf("THIS IS A PERSON!\n");
      barcodeFound = true;
      personBarcodeLineEdit->setText(person.barcode);
      personNameLineEdit->setText(person.name);
      personOnLoanList->clear();
      personReservedList->clear();
      clearBook();
      for(const auto &book: books) {
        if(book.loanedBy == person.barcode) {
          personOnLoanList->addItem(book.title);
        } else if(book.reservedBy == person.barcode) {
          personReservedList->addItem(book.title);
        }
      }
    }
  }
  for(auto &book: books) {
    if(barcode == book.barcode) {
      printf("THIS IS A BOOK!\n");
      barcodeFound = true;
      bookBarcodeLineEdit->setText(book.barcode);
      bookTitleLineEdit->setText(book.title);
      bookLoanedByLineEdit->setText(getPersonFromBarcode(book.loanedBy)); 
      bookReservedByLineEdit->setText(getPersonFromBarcode(book.reservedBy)); 
      if(book.title.isEmpty()) {
        printf("MISSING TITLE, NEW BOOK?\n");
        break;
      }
      if(!personBarcodeLineEdit->text().isEmpty()) {
        for(const auto &person: persons) {
          if(personBarcodeLineEdit->text() == person.barcode) {

            if(book.reservedBy.isEmpty() && book.loanedBy.isEmpty()) {
              QSound::play("sounds/laant.wav");
              printf("BOOK LOANED!\n");
              book.loanedBy = person.barcode;

            } else if(!book.reservedBy.isEmpty() && book.loanedBy.isEmpty()) {
              if(person.barcode == book.reservedBy) {
                QSound::play("sounds/laant.wav");
                printf("BOOK LOANED! HAD IT RESERVED!\n");
                book.reservedBy.clear();
                book.loanedBy = person.barcode;
              } else {
                QSound::play("sounds/reserveret_til_anden.wav");
                printf("RESERVED BY OTHER!\n");
              }

            } else if(!book.reservedBy.isEmpty() && !book.loanedBy.isEmpty()) {
              if(person.barcode == book.loanedBy) {
                QSound::play("sounds/afleveret-reserveret_til_anden.wav");
                printf("BOOK RETURNED! RESERVED BY OTHER!\n");
                book.loanedBy.clear();
              } else if(person.barcode == book.reservedBy) {
                QSound::play("sounds/udlaant-allerede_reserveret_til_dig.wav");
                printf("ON LOAN, ALREADY RESERVED!\n");
              }

            } else if(book.reservedBy.isEmpty() && !book.loanedBy.isEmpty()) {
              if(book.loanedBy != person.barcode) {
                QSound::play("sounds/udlaant-reserveret_til_dig.wav");
                printf("BOOK RESERVED!\n");
                book.reservedBy = person.barcode;
              } else if(book.loanedBy == person.barcode) {
                QSound::play("sounds/afleveret.wav");
                printf("BOOK RETURNED!\n");
                book.loanedBy.clear();
              }
            }

          }
        }
      }
    }
  }
  if(!barcodeFound) {
    QSound::play("sounds/ny_person_eller_bog.wav");
    printf("NEW BARCODE!\n");
    NewEntry newEntry(barcode, this);
    newEntry.exec();
    if(newEntry.result() == QDialog::Accepted) {
      if(newEntry.getType() == "person") {
        QSound::play("sounds/person_registreret.wav");
        printf("ADDING NEW PERSON!\n");
        Person person;
        person.barcode = barcode;
        person.name = newEntry.getNameTitle();
        persons.append(person);
      } else if(newEntry.getType() == "book") {
        QSound::play("sounds/bog_registreret.wav");
        printf("ADDING NEW BOOK!\n");
        Book book;
        book.barcode = barcode;
        book.title = newEntry.getNameTitle();
        books.append(book);
        std::sort(books.begin(), books.end(), [](const Book a, const Book b) -> bool { return a.title.toLower() < b.title.toLower(); });

      }
      clearAll();
      barcodeLineEdit->setText(barcode);
      checkBarcode();
    }
  } else {
    clearTimer.start();
  }
}

void MainWindow::clearAll()
{
  clearPerson();
  clearBook();
}

void MainWindow::clearPerson()
{
  for(auto *child: personGroup->findChildren<QLineEdit *>("")) {
    child->clear();
  }
  for(auto *child: personGroup->findChildren<QListWidget *>("")) {
    child->clear();
  }
}

void MainWindow::clearBook()
{
  for(auto *child: bookGroup->findChildren<QLineEdit *>("")) {
    child->clear();
  }
  for(auto *child: bookGroup->findChildren<QListWidget *>("")) {
    child->clear();
  }
}

QString MainWindow::getPersonFromBarcode(const QString &barcode)
{
  for(const auto &person: persons) {
    if(person.barcode == barcode) {
      return person.name;
    }
  }
  return "";
}

QString MainWindow::getBookFromBarcode(const QString &barcode)
{
  for(const auto &book: books) {
    if(book.barcode == barcode) {
      return book.title;
    }
  }
  return "";
}

void MainWindow::initRandomSound()
{
  int fixedInterval = 6 * 60 * 1000;
  int randomInterval = 4 * 60 * 1000;
  randomTimer.setInterval(fixedInterval + QRandomGenerator::global()->bounded(randomInterval));
  if(!randomTimer.isActive()) {
    randomTimer.start();
    return;
  }
  playRandomSound();
}

void MainWindow::playRandomSound()
{
  QDir randomsDir("sounds/randoms", "*.wav", QDir::Name, QDir::Files);
  QList<QFileInfo> soundInfos = randomsDir.entryInfoList();
  if(soundInfos.isEmpty()) {
    return;
  }
  QSound::play(soundInfos.at(QRandomGenerator::global()->bounded(soundInfos.length())).absoluteFilePath());
}

void MainWindow::focusBook(const QString &barcode)
{
  clearAll();
  barcodeLineEdit->setText(barcode);
  checkBarcode();
}

void MainWindow::focusBarcodeLineEdit()
{
  barcodeLineEdit->setFocus();
}
