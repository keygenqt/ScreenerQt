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

#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>
#include <QProcess>

class Command : public QObject
{
    Q_OBJECT
public:
    explicit Command(QObject *parent = nullptr);
    QString getVersion();
    QString getSelect();
    QString getDelay();
    QString getDesktop();
    bool getSearch();
    QString getVision();
    QString getTranslate();

private:
    QProcess *p;

//    QString command = "java -jar ~/Documents/Other/applications/screener/build/libs/screener.jar";
    QString command = "java -jar /snap/screenerqt/current/jar/screener.jar"; // snap

    QString select = "--select";
    QString delay = "--select-delay=3";
    QString desktop = "--desktop";
    QString search = "--search";
    QString vision = "--vision";
    QString translate = "--translate";
    QString version = "--version";
};

#endif // COMMAND_H
