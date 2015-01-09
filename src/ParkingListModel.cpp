#include "ParkingListModel.h"

const QString ParkingListModel::source1 = QString("http://carto.strasmap.eu/remote.amf.json/Parking.geometry");
const QString ParkingListModel::source2 = QString("http://carto.strasmap.eu/remote.amf.json/Parking.status");


ParkingListModel::ParkingListModel(QObject *parent) : QAbstractListModel(parent)
{
    //FIXME:
    this->m_prototype = new ParkingModel();
    this->m_parkings = QList<ParkingModel *>();

    //QObject::connect(this, SIGNAL(refreshNeeded()), this, SLOT(refresh()));
    //QObject::connect(this, SIGNAL(listUpToDate()), this, SLOT(fillList()));
}

ParkingListModel::~ParkingListModel()
{
    delete this->m_prototype;
    this->m_prototype = NULL;
    this->clear();
}

QHash<int,QByteArray> ParkingListModel::roleNames() const
{
    return this->m_prototype->roleNames();
}

int ParkingListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return this->m_parkings.size();
}

QVariant ParkingListModel::data(const QModelIndex &index, int role) const
{
    QVariant r = QVariant();

    if(index.isValid() && index.row() >=0 && index.row() < this->m_parkings.size())
        r = this->m_parkings.at(index.row())->data(role);

    return r;
}

QVariant ParkingListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant r = QVariant();

    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
            r = QString("Column %1").arg(section);
        else
            r = QString("Row %1").arg(section);
    }

    return r;
}

Qt::ItemFlags ParkingListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags r = Qt::ItemIsEnabled;

    if(index.isValid())
        r = QAbstractItemModel::flags(index) | Qt::ItemIsEditable;

    return r;
}

bool ParkingListModel::appendRow(ParkingModel *item)
{
    bool r = false;

    if(item != NULL)
    {
        this->appendRows(QList<ParkingModel *>() << item);

        emit countChanged(this->rowCount());

        r = true;
    }

    return r;
}

bool ParkingListModel::appendRows(QList<ParkingModel *> &items)
{
    bool r = false;

    if(items.size() > 0)
    {
        this->beginInsertRows(QModelIndex(), this->rowCount(), this->rowCount() + items.size() - 1);

        foreach(ParkingModel *item, items)
        {
            QObject::connect(item, SIGNAL(dataChanged()), this, SLOT(updateItem()));
            this->m_parkings.append(item);
        }

        this->endInsertRows();

        emit countChanged(this->rowCount());

        r = true;
    }

    return r;
}

bool ParkingListModel::insertRow(int row, ParkingModel *item)
{
    bool r = false;

    if(item != NULL)
    {
        this->beginInsertRows(QModelIndex(), row, row);

        QObject::connect(item, SIGNAL(dataChanged()), this, SLOT(updateItem()));
        this->m_parkings.insert(row, item);

        this->endInsertRows();

        emit countChanged(this->rowCount());

        r = true;
    }

    return r;
}

bool ParkingListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    this->beginInsertRows(parent, row, row + count - 1);

    for(int i=row ; i<(row + count -1) ; i++)
    {
        //FIXME: is this ok ?
        this->m_parkings.insert(i, new ParkingModel(this));
    }

    this->endInsertRows();

    return true;
}

bool ParkingListModel::removeRow(int row, const QModelIndex &parent)
{
    bool r = false;

    if(row >= 0 && row < this->rowCount())
    {
        this->beginRemoveRows(parent, row, row);

        this->m_parkings.removeAt(row);

        this->endRemoveRows();

        emit countChanged(this->rowCount());

        r = true;
    }

    return r;
}

bool ParkingListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    bool r = false;

    if(row >= 0 && count > 0 && (row + count) <= this->rowCount())
    {
        this->beginRemoveRows(parent, row, row + count - 1);

        for(int i=(row + count - 1) ; i <= row ; i--)
        {
            this->m_parkings.removeAt(i);
        }

        this->endRemoveRows();

        emit countChanged(this->rowCount());

        r = true;
    }

    return r;
}

void ParkingListModel::clear()
{
    if(this->rowCount() > 0)
    {
        this->removeRows(0, this->rowCount());

        emit(countChanged(this->rowCount()));
    }
}

bool ParkingListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool r = false;

    if(index.isValid() && index.row() < this->rowCount())
    {
        this->m_parkings.at(index.row())->setData(value, role);

        emit dataChanged(index, index);

        r = true;
    }

    return r;
}

void ParkingListModel::updateItem()
{
    /*
    ParkingModel *item = static_cast<ParkingModel *>(sender());
    QModelIndex index = this->indexFromItem(item);

    if(index.isValid())
        emit dataChanged(index, index);
    */
}


void ParkingListModel::refresh()
{

}

void ParkingListModel::fillList()
{

}
