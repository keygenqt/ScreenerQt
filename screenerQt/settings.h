#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);
    QString getDir();
    QString getCredPath();
    QString getLanguage();
    QString getStatusImgur();
    void save(QString, QString, QString, QString);

private:
//    QString path = "~/.screener/config.json";
    QString path = "~/snap/screenerqt/common/config.json"; // snap
    QJsonObject getSettings();

signals:

};

#endif // SETTINGS_H
