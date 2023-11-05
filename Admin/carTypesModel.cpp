#include "CarTypesModel.h"
#include <QSqlQuery.h>
#include <QSqlRecord.h>
#include <QDebug.h>

const QString CarTypesModel::locTableName = "car_types";

//--------------------------------------------------------------------------------
void CarTypesModel::init() {
    this->setTable(locTableName);
    this->setEditStrategy(QSqlTableModel::OnManualSubmit);
    if (m_view) {
        m_view->setModel(this);
        m_view->setWindowTitle("loc");
        m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    }

};
//--------------------------------------------------------------------------------
bool CarTypesModel::getList(QSqlDatabase db, QComboBox* cmb)
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
bool CarTypesModel::selectType(QSqlDatabase db, const QString& name, int& price, int& speed, int& max_persons, int& service_km, int& service_price)
{
    QSqlQuery query(db);

    QString q = QString("SELECT * from %1  WHERE name = :name ").arg(locTableName);

    query.prepare(q);
    query.bindValue(":name", name);

    if (query.exec()) {

        query.next();
        QSqlRecord reci = query.record();

        price = reci.value(1).toInt();
        speed = reci.value(2).toInt();
        max_persons = reci.value(3).toInt();
        service_km = reci.value(4).toInt();
        service_price = reci.value(5).toInt();

        qDebug() << name << price << "\n";

        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------

