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

#include "preview.h"
#include "ui_preview.h"
#include <qdebug.h>
#include <QStringListModel>

Preview::Preview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preview)
{
    ui->setupUi(this);
}

Preview::~Preview()
{
    delete ui;
}

void Preview::on_buttonBox_clicked(QAbstractButton *button)
{

}

void Preview::synchronizing(QModelIndexList index)
{
    ui->listViewWill->setCurrentIndex(index.first());
}

void Preview::on_buttonBox_accepted()
{
    emit success();
}


void Preview::showInfo(QString info) {

    QStringList now = QStringList();
    QStringList will = QStringList();

    QStringList items = info.split( "\n" );
    for ( const auto& item : items )
    {
        if (item == "") {
            break;
        }
        QStringList items = item.split( "->" );
        if (items.size() == 2) {
            now.append("\n" + items[0].trimmed() + "\n");
            will.append("\n" + items[1].trimmed() + "\n");
        }
    }

    ui->listViewNow->setModel(new QStringListModel(now));
    ui->listViewWill->setModel(new QStringListModel(will));
}
