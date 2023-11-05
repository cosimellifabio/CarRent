#include "CarModel.h"
#include <QSqlQuery.h>
#include <QSqlRecord.h>
#include <QDebug.h>

const QString CarModel::CarTableName = "cars";

//--------------------------------------------------------------------------------
void CarModel::init() {
    this->setTable(CarTableName);
    this->setEditStrategy(QSqlTableModel::OnManualSubmit);
    this->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    //this->setHeaderData(1, Qt::Horizontal, QObject::tr("First name"));
    //this->setHeaderData(2, Qt::Horizontal, QObject::tr("Last name"));
    m_view->setModel(this);
    m_view->setWindowTitle("CARS");
    m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
};
//--------------------------------------------------------------------------------
void CarModel::show() {

    m_view->show();
};
//--------------------------------------------------------------------------------
bool CarModel::create(QSqlDatabase db, const QString& name, const QString& brand, const QString& tail_number, const QString& class1)
{
    QSqlQuery query(db);

    QString q = QString("INSERT INTO %1 "
        "(name,  brand, tail_number, class) VALUES ("
        " :name, :brand, :tail_number, :class )").arg(CarTableName);

    query.prepare(q);
    query.bindValue(":name", name);
    query.bindValue(":brand", brand);
    query.bindValue(":tail_number", tail_number);
    query.bindValue(":class", class1);

    return query.exec();
}
//--------------------------------------------------------------------------------
bool CarModel::selectCar(QSqlDatabase db, int id, QString& name, QString& brand, QString& tail_number, QString& class1, int& next_service_km)
{
    QSqlQuery query(db);

    QString q = QString("SELECT * from %1  WHERE id = :id ").arg(CarTableName);

    query.prepare(q);
    query.bindValue(":id", id);

    if (query.exec()) {
        
        query.next();
        QSqlRecord reci = query.record();

        name = reci.value(1).toString();
        brand = reci.value(2).toString();
        tail_number = reci.value(3).toString();
        class1 = reci.value(4).toString();
        next_service_km = reci.value(5).toInt();

        qDebug() << id << name << "\n";
          
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
bool CarModel::getCar(QSqlDatabase db, const QModelIndex& i, int& id, QString& name, QString& brand, QString& tail_number, QString& class1, int& next_service_km)
{
    id = this->data(i).toInt();
    return selectCar(db, id, name, brand, tail_number, class1, next_service_km);
}
//--------------------------------------------------------------------------------
bool CarModel::edit(QSqlDatabase db, int id, const QString& name, const QString& brand, const QString& tail_number, const QString& class1)
{
    QSqlQuery query(db);

    QString q = QString("UPDATE %1 SET name=:name, brand=:brand, tail_number=:tail_number, class=:class1, updatedAt=now() WHERE id = :id ").arg(CarTableName);

    query.prepare(q);
    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.bindValue(":brand", brand);
    query.bindValue(":tail_number", tail_number);
    query.bindValue(":class1", class1);
    qDebug() << q;
    if (query.exec()) {

        query.next();
        qDebug() << id << name << "\n";

        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
bool CarModel::deleteCar(QSqlDatabase db, int id)
{
    QSqlQuery query(db);

    QString q = QString("DELETE from %1  WHERE id = :id ").arg(CarTableName);

    query.prepare(q);
    query.bindValue(":id", id);

    if (query.exec()) {

        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------

bool CarModel::updateService(QSqlDatabase db, int id, int next_service_km)
{
    QSqlQuery query(db);

    QString q = QString("UPDATE %1 SET next_service_km=:next_service_km, updatedAt=now() WHERE id = :id ").arg(CarTableName);

    query.prepare(q);
    query.bindValue(":id", id);
    query.bindValue(":next_service_km", next_service_km);
    if (query.exec()) {

        query.next();
        return true;
    }
    return false;
}

//--------------------------------------------------------------------------------
