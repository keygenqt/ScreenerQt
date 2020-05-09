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
#include "messagebox.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->msgBox = new MessageBox(this);
    this->settings = new Settings(parent);
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
    viewWindow = new QAction(QString("Show Settings"), this);
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
    menu->addSeparator();
    menu->addAction(actionSelect);
    menu->addAction(actionDelay);
    menu->addAction(actionDesktop);
    menu->addAction(actionSearch);
    menu->addSeparator();
    menu->addAction(actionVision);
    menu->addAction(actionTranslate);
    menu->addSeparator();
    menu->addAction(actionQuit);

    bool disable = this->settings->getCredPath() == "";
    actionVision->setDisabled(disable);
    actionTranslate->setDisabled(disable);

    trayIcon->setContextMenu(menu);
    trayIcon->show();
    ///////////////////

    // language
    this->initLanguageBox();

    // default
    ui->comboBox_2->addItem("Upload to Imgur", "true");
    ui->comboBox_2->addItem("Add to clipboard", "false");

    if (version == "") {
        this->hide();
        QMessageBox::information(this, tr("ScreenerQt"), "ScreenerQt not found<br><br>Install ScreenerQt app:<br><b>sudo snap install screener</b>");
        this->close();
    }

    this->setFixedSize(680, 340);
    this->statusBar()->setSizeGripEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::actionItemTray() {
    QAction* action = qobject_cast<QAction *>(QObject::sender());

    if (action->text() == QString("Show Settings")) {
        viewWindow->setText(QString("Hide Settings"));
        ui->input_dir->setText(this->settings->getDir());
        ui->input_cloud->setText(this->settings->getCredPath());
        ui->comboBox->setCurrentIndex(ui->comboBox->findData(this->settings->getLanguage()));
        ui->comboBox_2->setCurrentIndex(ui->comboBox_2->findData(this->settings->getStatusImgur()));
        this->show();
    }
    else if (action->text() == QString("Hide Settings")) {
        viewWindow->setText(QString("Show Settings"));
        this->hide();
    }
    else if (action->text() == QString("Quit")) {
        this->hide();
        this->close();
    }
    else if (action->text() == QString("Select")) {
        QString res = command->getSelect();
        if (res != "") {
            QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
            trayIcon->showMessage(tr("Screener"), res, icon, 1000);
        }
    }
    else if (action->text() == QString("Select Delay")) {
        QString res = command->getDelay();
        if (res != "") {
            QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
            trayIcon->showMessage(tr("Screener"), res, icon, 1000);
        }
    }
    else if (action->text() == QString("Desktop")) {
        QString res = command->getDesktop();
        if (res != "") {
            QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
            trayIcon->showMessage(tr("Screener"), res, icon, 1000);
        }
    }
    else if (action->text() == QString("Search")) {
        if (command->getSearch()) {
            QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
            trayIcon->showMessage(tr("Screener"), QString("Link for search ready. It is placed on the clipboard.").toUtf8(), icon, 1000);
        }
    }
    else if (action->text() == QString("Vision")) {
        QString res = command->getVision();
        if (res == "") {
            QMessageBox::information(this, tr("ScreenerQt"), "Error parse vision");
        } else {
            this->msgBox->setText(res.trimmed());
            this->msgBox->show();
        }
    }
    else if (action->text() == QString("Translate")) {
        QString res = command->getTranslate();
        if (res == "") {
            QMessageBox::information(this, tr("ScreenerQt"), "Error translate vision");
        } else {
            this->msgBox->setText(res.trimmed());
            this->msgBox->show();
        }
    }
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    if(this->isVisible()){
        event->ignore();
        this->hide();
        viewWindow->setText(QString("Show Settings").toUtf8());
        if (showMinimize) {
            QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
            trayIcon->showMessage(tr("ScreenerQt"), QString("The application is minimized to tray.").toUtf8(), icon, 1000);
        }
        showMinimize = false;
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 QDir::homePath(),
                                                 QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir != "") {
        ui->input_dir->setText(dir);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open Cloud Credentials"),
                                                 QDir::homePath(),
                                                 tr("Cloud credentials (*.json)"));
    if (path != "") {
        ui->input_cloud->setText(path);
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    this->settings->save(
                ui->input_dir->text(),
                ui->input_cloud->text(),
                ui->comboBox->currentData().toString(),
                ui->comboBox_2->currentData().toString());

    bool disable = this->settings->getCredPath() == "";
    actionVision->setDisabled(disable);
    actionTranslate->setDisabled(disable);

    QMessageBox::information(this, tr("ScreenerQt"), "Save success");
}

void MainWindow::initLanguageBox()
{
    ui->comboBox->addItem("Afrikaans", "af");
    ui->comboBox->addItem("Albanian", "sq");
    ui->comboBox->addItem("Amharic", "am");
    ui->comboBox->addItem("Arabic", "ar");
    ui->comboBox->addItem("Armenian", "hy");
    ui->comboBox->addItem("Azerbaijani", "az");
    ui->comboBox->addItem("Basque", "eu");
    ui->comboBox->addItem("Belarusian", "be");
    ui->comboBox->addItem("Bengali", "bn");
    ui->comboBox->addItem("Bosnian", "bs");
    ui->comboBox->addItem("Bulgarian", "bg");
    ui->comboBox->addItem("Catalan", "ca");
    ui->comboBox->addItem("Corsican", "co");
    ui->comboBox->addItem("Croatian", "hr");
    ui->comboBox->addItem("Czech", "cs");
    ui->comboBox->addItem("Danish", "da");
    ui->comboBox->addItem("Dutch", "nl");
    ui->comboBox->addItem("English", "en");
    ui->comboBox->addItem("Esperanto", "eo");
    ui->comboBox->addItem("Estonian", "et");
    ui->comboBox->addItem("Finnish", "fi");
    ui->comboBox->addItem("French", "fr");
    ui->comboBox->addItem("Frisian", "fy");
    ui->comboBox->addItem("Galician", "gl");
    ui->comboBox->addItem("Georgian", "ka");
    ui->comboBox->addItem("German", "de");
    ui->comboBox->addItem("Greek", "el");
    ui->comboBox->addItem("Gujarati", "gu");
    ui->comboBox->addItem("Haitian Creole", "ht");
    ui->comboBox->addItem("Hausa", "ha");
    ui->comboBox->addItem("Hindi", "hi");
    ui->comboBox->addItem("Hungarian", "hu");
    ui->comboBox->addItem("Icelandic", "is");
    ui->comboBox->addItem("Igbo", "ig");
    ui->comboBox->addItem("Indonesian", "id");
    ui->comboBox->addItem("Irish", "ga");
    ui->comboBox->addItem("Italian", "it");
    ui->comboBox->addItem("Japanese", "ja");
    ui->comboBox->addItem("Javanese", "jv");
    ui->comboBox->addItem("Kannada", "kn");
    ui->comboBox->addItem("Kazakh", "kk");
    ui->comboBox->addItem("Khmer", "km");
    ui->comboBox->addItem("Korean", "ko");
    ui->comboBox->addItem("Kurdish", "ku");
    ui->comboBox->addItem("Kyrgyz", "ky");
    ui->comboBox->addItem("Lao", "lo");
    ui->comboBox->addItem("Latin", "la");
    ui->comboBox->addItem("Latvian", "lv");
    ui->comboBox->addItem("Lithuanian", "lt");
    ui->comboBox->addItem("Luxembourgish", "lb");
    ui->comboBox->addItem("Macedonian", "mk");
    ui->comboBox->addItem("Malagasy", "mg");
    ui->comboBox->addItem("Malay", "ms");
    ui->comboBox->addItem("Malayalam", "ml");
    ui->comboBox->addItem("Maltese", "mt");
    ui->comboBox->addItem("Maori", "mi");
    ui->comboBox->addItem("Marathi", "mr");
    ui->comboBox->addItem("Mongolian", "mn");
    ui->comboBox->addItem("Myanmar (Burmese)", "my");
    ui->comboBox->addItem("Nepali", "ne");
    ui->comboBox->addItem("Norwegian", "no");
    ui->comboBox->addItem("Nyanja (Chichewa)", "ny");
    ui->comboBox->addItem("Pashto", "ps");
    ui->comboBox->addItem("Persian", "fa");
    ui->comboBox->addItem("Polish", "pl");
    ui->comboBox->addItem("Portuguese (Portugal, Brazil)", "pt");
    ui->comboBox->addItem("Punjabi", "pa");
    ui->comboBox->addItem("Romanian", "ro");
    ui->comboBox->addItem("Russian", "ru");
    ui->comboBox->addItem("Samoan", "sm");
    ui->comboBox->addItem("Scots Gaelic", "gd");
    ui->comboBox->addItem("Serbian", "sr");
    ui->comboBox->addItem("Sesotho", "st");
    ui->comboBox->addItem("Shona", "sn");
    ui->comboBox->addItem("Sindhi", "sd");
    ui->comboBox->addItem("Sinhala (Sinhalese)", "si");
    ui->comboBox->addItem("Slovak", "sk");
    ui->comboBox->addItem("Slovenian", "sl");
    ui->comboBox->addItem("Somali", "so");
    ui->comboBox->addItem("Spanish", "es");
    ui->comboBox->addItem("Sundanese", "su");
    ui->comboBox->addItem("Swahili", "sw");
    ui->comboBox->addItem("Swedish", "sv");
    ui->comboBox->addItem("Tagalog (Filipino)", "tl");
    ui->comboBox->addItem("Tajik", "tg");
    ui->comboBox->addItem("Tamil", "ta");
    ui->comboBox->addItem("Telugu", "te");
    ui->comboBox->addItem("Thai", "th");
    ui->comboBox->addItem("Turkish", "tr");
    ui->comboBox->addItem("Ukrainian", "uk");
    ui->comboBox->addItem("Urdu", "ur");
    ui->comboBox->addItem("Uzbek", "uz");
    ui->comboBox->addItem("Vietnamese", "vi");
    ui->comboBox->addItem("Welsh", "cy");
    ui->comboBox->addItem("Xhosa", "xh");
    ui->comboBox->addItem("Yiddish", "yi");
    ui->comboBox->addItem("Yoruba", "yo");
    ui->comboBox->addItem("Zulu", "zu");
}
