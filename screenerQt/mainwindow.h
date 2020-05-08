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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <preview.h>
#include <dialog.h>
#include <command.h>
#include <QMainWindow>
#include <QCloseEvent>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent * event);

private slots:
    void actionItemTray();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    Command *command;

    QSystemTrayIcon *trayIcon;

    QAction * viewWindow;

    QAction * actionSelect;
    QAction * actionDelay;
    QAction * actionDesktop;
    QAction * actionSearch;
    QAction * actionVision;
    QAction * actionTranslate;
    QAction * actionQuit;
};
#endif // MAINWINDOW_H
