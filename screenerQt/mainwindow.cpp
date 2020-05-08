/*
 * This file is part of ScreenerQt.
 *
 * ScreenerQt is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ScreenerQt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ScreenerQt.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdebug.h>
#include <QMessageBox>
#include <QStyle>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QProcess>
#include <QCloseEvent>
#include <QAction>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->command = new Command(parent);
    QString version = command->getVersion();

    ui->setupUi(this);
    ui->statusbar->showMessage("ScreenerQt v0.0.1; Screener v" + version);

    this->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            this->size(),
            qApp->desktop()->availableGeometry()
        )
    );

    // status bar
    trayIcon = new QSystemTrayIcon(this);

    trayIcon->setIcon(QIcon(":data/logo.png"));

    QMenu * menu = new QMenu(this);
    viewWindow = new QAction(QString("Hide App"), this);
    actionSelect = new QAction(QString("Select"), this);
    actionDelay = new QAction(QString("Select Delay"), this);
    actionDesktop = new QAction(QString("Desktop"), this);
    actionSearch = new QAction(QString("Search"), this);
    actionVision = new QAction(QString("Vision"), this);
    actionTranslate = new QAction(QString("Translate"), this);
    actionQuit = new QAction(QString("Quit"), this);

    connect(viewWindow, SIGNAL(triggered()), this, SLOT(actionItemTray()));
    connect(actionSelect, SIGNAL(triggered()), this, SLOT(actionItemTray()));
    connect(actionDelay, SIGNAL(triggered()), this, SLOT(actionItemTray()));
    connect(actionDesktop, SIGNAL(triggered()), this, SLOT(actionItemTray()));
    connect(actionSearch, SIGNAL(triggered()), this, SLOT(actionItemTray()));
    connect(actionVision, SIGNAL(triggered()), this, SLOT(actionItemTray()));
    connect(actionTranslate, SIGNAL(triggered()), this, SLOT(actionItemTray()));
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(actionItemTray()));

    menu->addAction(viewWindow);
    menu->addAction(actionSelect);
    menu->addAction(actionDelay);
    menu->addAction(actionDesktop);
    menu->addAction(actionSearch);
    menu->addSeparator();
    menu->addAction(actionVision);
    menu->addAction(actionTranslate);
    menu->addSeparator();
    menu->addAction(actionQuit);

    actionVision->setDisabled(true);
    actionTranslate->setDisabled(true);

    trayIcon->setContextMenu(menu);
    trayIcon->show();
    ///////////////////

    if (version == "") {
        this->hide();
        QMessageBox::information(this, tr("ScreenerQt"), "ScreenerQt not found<br><br>Install ScreenerQt app:<br><b>sudo snap install screener</b>");
        this->close();
    } else {
        this->show();
    }

    this->setFixedSize(680, 365);
    this->statusBar()->setSizeGripEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::actionItemTray() {
    QAction* action = qobject_cast<QAction *>(QObject::sender());

    if (action->text() == QString("Show App")) {
        viewWindow->setText(QString("Hide App"));
        this->show();
    }
    else if (action->text() == QString("Hide App")) {
        viewWindow->setText(QString("Show App"));
        this->hide();
    }
    else if (action->text() == QString("Quit")) {
        this->hide();
        this->close();
    }
    else if (action->text() == QString("Select")) {
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
        trayIcon->showMessage(tr("Screener"), QString(command->getSelect()).toUtf8(), icon, 1000);
    }
    else if (action->text() == QString("Select Delay")) {
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
        trayIcon->showMessage(tr("Screener"), QString(command->getDelay()).toUtf8(), icon, 1000);
    }
    else if (action->text() == QString("Desktop")) {
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
        trayIcon->showMessage(tr("Screener"), QString(command->getDesktop()).toUtf8(), icon, 1000);
    }
    else if (action->text() == QString("Search")) {
        if (command->getSearch()) {
            QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
            trayIcon->showMessage(tr("Screener"), QString("Link for search ready. It is placed on the clipboard.").toUtf8(), icon, 1000);
        }
    }
    else if (action->text() == QString("Vision")) {
        qDebug() << command->getVision();
    }
    else if (action->text() == QString("Translate")) {
        qDebug() << command->getTranslate();
    }
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    if(this->isVisible()){
        event->ignore();
        this->hide();
        viewWindow->setText(QString("Show App").toUtf8());
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
        trayIcon->showMessage(tr("Screener"), QString("The application is minimized to tray.").toUtf8(), icon, 1000);
    }
}
