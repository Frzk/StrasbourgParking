#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include "ParkingModel.h"
#include "ParkingListModel.h"
#include "SortedParkingListModel.h"
#include "FavoritesStorage.h"
#include "JSONRequest.h"

class Controller : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isRefreshing READ isRefreshing NOTIFY isRefreshingChanged)
    Q_PROPERTY(QDateTime lastUpdate READ lastUpdate NOTIFY lastUpdateChanged)

    public:
        explicit Controller(QObject *parent = 0);

        ~Controller();

        SortedParkingListModel*             model() const;
        static QHash<int, QJsonObject>      jsonArrayToHashMap(const QJsonArray &a);
        bool                                canRefresh() const;
        bool                                isRefreshing() const;
        QDateTime                           lastUpdate() const;
        Q_INVOKABLE bool                    isFavorite(const int row) const;

    private:
        static const int            refreshInterval = 180;  // Only allow refresh after 3 minutes.
        SortedParkingListModel      *m_model;
        FavoritesStorage            *m_fav;
        JSONRequest                 *m_req1;
        JSONRequest                 *m_req2;
        bool                        m_isRefreshing;
        QDateTime                   m_lastSuccessfulRefresh;

    signals:
        void    modelFilled();
        void    dataRefreshed();
        void    isRefreshingChanged();  // Needed for isRefreshing Q_PROPERTY.
        void    lastUpdateChanged();    // Needed for lastUpdate Q_PROPERTY.

    public slots:
        void        triggerUpdate();

    private slots:
        void    fillModel(const QJsonDocument &d);
        void    refresh(const QJsonDocument &d);
        void    updateData();
        void    handleNetworkError(const QNetworkReply::NetworkError &errcode);
        bool    updateFavorite(int id, bool value);
};

#endif // CONTROLLER_H
