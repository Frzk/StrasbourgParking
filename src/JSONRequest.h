#ifndef JSONREQUEST_H
#define JSONREQUEST_H

#include <QObject>

#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QString>

class JSONRequest : public QObject
{
    Q_OBJECT

    public:
        explicit    JSONRequest(QObject *parent = 0);
        ~JSONRequest();
        void        request(const QString &url) const;

    private:
        QNetworkAccessManager *m_qnam;

    signals:
        void documentReady(const QJsonDocument &document);
        void networkError(const QNetworkReply::NetworkError &error);

    private slots:
        void handleReply(QNetworkReply *reply);
};

#endif // JSONREQUEST_H
