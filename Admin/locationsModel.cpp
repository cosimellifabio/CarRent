#include "LocationsModel.h"
#include <QSqlQuery.h>
#include <QSqlRecord.h>
#include <QDebug.h>

const QString LocationsModel::locTableName = "destinations";

//--------------------------------------------------------------------------------
void LocationsModel::init() {
    this->setTable(locTableName);
    this->setEditStrategy(QSqlTableModel::OnManualSubmit);
    if (m_view) {
        m_view->setModel(this);
        m_view->setWindowTitle("loc");
        m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    }

};
//--------------------------------------------------------------------------------
bool LocationsModel::getList(QSqlDatabase db, QComboBox* cmb)
{
    QSqlQuery query(db);
    QString q = QString("SELECT public.destinations.name from %1  ").arg(locTableName);
    query.prepare(q);

    if (query.exec()) {

        while (query.next()) {
            QSqlRecord reci = query.record();

            cmb->addItem(reci.value(0).toString());
        }
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
bool LocationsModel::selectLocation(QSqlDatabase db, int id, QString& name, int& before, int& after, int& hops, int& price)
{
    QSqlQuery query(db);

    QString q = QString("SELECT * from %1  WHERE id = :id ").arg(locTableName);

    query.prepare(q);
    query.bindValue(":id", id);

    if (query.exec()) {

        query.next();
        QSqlRecord reci = query.record();

        name = reci.value(1).toString();
        before = reci.value(2).toInt();
        after = reci.value(3).toInt();
        hops = reci.value(4).toInt();
        price = reci.value(5).toInt();

        qDebug() << id << name << "\n";

        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------

