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

#include "command.h"
#include <qdebug.h>
#include <QProcess>
#include <QRegularExpression>
#include <QDir>

Command::Command(QObject *parent) : QObject(parent)
{
    this->command = this->command.replace("~/", QDir::homePath() + "/");
    this->p = new QProcess(this);
}

QString Command::getVersion()
{
    p->start(command + " " + version);
    p->waitForFinished();
    return p->readAllStandardOutput().replace("Screener Ver ", "").replace("\n", "").trimmed();
}

QString Command::getSelect()
{
    p->start(command + " " + select);
    p->waitForFinished();
    QString value = p->readAllStandardOutput();
    return value.replace("\n", "").replace(QRegularExpression("\\s+"), "").replace("Urlsaveinclipboard:", "").replace("----", "").trimmed();
}

QString Command::getDelay()
{
    p->start(command + " " + delay);
    p->waitForFinished();
    QString value = p->readAllStandardOutput();
    QString value2 = value.replace("\n", "").replace(QRegularExpression("\\s+"), "").replace(QRegularExpression(".+\\:----"), "").replace("----", "").trimmed();
    if (value2.contains("timer")) {
        return "";
    } else {
        return value2;
    }
}

QString Command::getDesktop()
{
    p->start(command + " " + desktop);
    p->waitForFinished();
    QString value = p->readAllStandardOutput();
    return value.replace("\n", "").replace(QRegularExpression("\\s+"), "").replace(QRegularExpression(".+----h"), "h").trimmed();
}

bool Command::getSearch()
{
    p->start(command + " " + search);
    p->waitForFinished();
    QString value = p->readAllStandardOutput();
    return value.replace("\n", "").replace(QRegularExpression("\\s+"), "").contains("clipboard");
}

QString Command::getVision()
{
    p->start(command + " " + vision);
    p->waitForFinished();
    QString value = p->readAllStandardOutput();
    if (!value.contains("success")) {
        return "";
    }
    return value.replace("\n", "").replace(QRegularExpression(".+\\:----"), "").replace("----", "").trimmed();
}

QString Command::getTranslate()
{
    p->start(command + " " + translate);
    p->waitForFinished();
    QString value = p->readAllStandardOutput();
    if (!value.contains("success")) {
        return "";
    }
    return value.replace("\n", "").replace(QRegularExpression(".+\\:----"), "").replace("----", "").trimmed();
}
