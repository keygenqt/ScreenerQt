#include "settings.h"
#include <qdebug.h>
#include <QFile>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QDir>

Settings::Settings(QObject *parent) : QObject(parent)
{
    this->path = this->path.replace("~/", QDir::homePath() + "/");
}

QString Settings::getDir()
{
    QJsonObject params = this->getSettings();
    QJsonValue value = params.value(QString("Folder path for save screenshot"));
    return value.toString().replace("{HOME}", QDir::homePath());
}

QString Settings::getCredPath()
{
    QJsonObject params = this->getSettings();
    QJsonValue value = params.value(QString("Cloud credentials path to file"));
    return value.toString();
}

QString Settings::getLanguage()
{
    QJsonObject params = this->getSettings();
    QJsonValue value = params.value(QString("Cloud translate language"));
    return value.toString();
}

QString Settings::getStatusImgur()
{
    QJsonObject params = this->getSettings();
    QJsonValue value = params.value(QString("Upload screenshot to Imgur"));
    return value.toString();
}

QJsonObject Settings::getSettings()
{
    QFile file;
    file.setFileName(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString val = file.readAll();
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    return  d.object();
}

void Settings::save(QString dir, QString cred, QString lang, QString imgur) {

    QJsonObject params = this->getSettings();

    params["Folder path for save screenshot"] = dir;
    params["Cloud credentials path to file"] = cred;
    params["Cloud translate language"] = lang;
    params["Upload screenshot to Imgur"] = imgur;

    QFile file;
    file.setFileName(path);
    file.open(QIODevice::WriteOnly);
    QJsonDocument doc(params);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}
