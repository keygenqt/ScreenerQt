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

#include "dialog.h"
#include "ui_dialog.h"
#include <qdebug.h>
#include <QStringListModel>
#include <QtWidgets>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::showLines(QString params)
{
    QStringList list = QStringList();
    QStringList items = params.split( "------------", QString::SkipEmptyParts);
    for ( const auto& item : items ) {
        QString tr = item.trimmed();
        if (tr == "") {
            continue;
        }
        list.append("\n" + tr + "\n");
    }
    QStringListModel *model = new QStringListModel(list);
    ui->listView->setModel(model);
    ui->listView->scrollToBottom();
    ui->listView->setCurrentIndex(model->index(model->rowCount()-1));
    ui->listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    this->show();
}
