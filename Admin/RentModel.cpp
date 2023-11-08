#include "RentModel.h"
#include <QSqlQuery.h>
#include <QSqlRecord.h>
#include <QDebug.h>

double minAngle(int from, int to) {
    int r = abs(from - to);
    if (360 - r < r)
        r = 360 - r;
    return r;
}

double arc(int from, int to, int radius) {
    return ((double)minAngle(from, to)) * radius * M_PI / 360.;
}

const QString RentModel::RentTableName = "rents";
const QString RentModel::DateTimeFormat = "yyyy-MM-ddThh:mm:ss.zzz";// "yyyy-MM-dd hh:mm:ss";

//--------------------------------------------------------------------------------
void RentModel::init() {
    this->setTable(RentTableName);
    this->setEditStrategy(QSqlTableModel::OnManualSubmit);
    this->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    //this->setHeaderData(1, Qt::Horizontal, QObject::tr("First name"));
    //this->setHeaderData(2, Qt::Horizontal, QObject::tr("Last name"));
    m_view->setModel(this);
    m_view->setWindowTitle("RENTS");
    m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
};
//--------------------------------------------------------------------------------
void RentModel::show() {

    m_view->show();
};
//--------------------------------------------------------------------------------
bool RentModel::create(QSqlDatabase db, const QString& name, int userid, int car, int fromloc, int toloc, int fromangle, int toangle, int price, int km,
    int passengers, const QDateTime& date_from, const QDateTime& date_to)
{
    QSqlQuery query(db);

    QString q = QString("INSERT INTO %1 "
        "(name, userid, car, fromloc, toloc, from_angle, to_angle, price, km, passengers, date_from, date_to) VALUES ("
        " :name, :userid, :car, :fromloc, :toloc, :fromangle, :toangle, :price, :km, :passengers, :date_from, :date_to )").arg(RentTableName);

    qDebug() << q;
    query.prepare(q);
    query.bindValue(":name", name);
    query.bindValue(":userid", userid);
    query.bindValue(":car", car);
    query.bindValue(":fromloc", fromloc);
    query.bindValue(":toloc", toloc);
    query.bindValue(":fromangle", fromangle);
    query.bindValue(":toangle", toangle);
    query.bindValue(":price", price);
    query.bindValue(":km", km);
    query.bindValue(":passengers", passengers);
    query.bindValue(":date_from", date_from.toString(DateTimeFormat));
    query.bindValue(":date_to", date_to.toString(DateTimeFormat));

    return query.exec();
}
//--------------------------------------------------------------------------------
bool RentModel::selectRent(QSqlDatabase db, int id, int& fromloc, int& toloc, int& fromangle, int& toangle)
{
    QSqlQuery query(db);

    QString q = QString("SELECT * from %1  WHERE id = :id ").arg(RentTableName);

    query.prepare(q);
    query.bindValue(":id", id);

    if (query.exec()) {

        query.next();
        QSqlRecord reci = query.record();

        fromloc = reci.value(4).toInt();
        toloc = reci.value(5).toInt();
        fromangle = reci.value(6).toInt();
        toangle = reci.value(7).toInt();

       // qDebug() << id << name << "\n";

        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
bool RentModel::getRent(QSqlDatabase db, const QModelIndex& i, int& id, int& fromloc, int& toloc, int& fromangle, int& toangle)
{
    id = this->data(i).toInt();
    return selectRent(db, id,  fromloc, toloc, fromangle, toangle);
}
//--------------------------------------------------------------------------------
bool RentModel::edit(QSqlDatabase db, int id, const QString& name, const QString& brand, const QString& tail_number, const QString& class1)
{
    QSqlQuery query(db);

    QString q = QString("UPDATE %1 SET name=:name, brand=:brand, tail_number=:tail_number, class=:class1, updatedAt=now() WHERE id = :id ").arg(RentTableName);

    query.prepare(q);
    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.bindValue(":brand", brand);
    query.bindValue(":tail_number", tail_number);
    query.bindValue(":class1", class1);
    qDebug() << q;
    if (query.exec()) {

        query.next();
        QSqlRecord reci = query.record();
        qDebug() << id << name << "\n";

        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
bool RentModel::deleteRent(QSqlDatabase db, int id)
{
    QSqlQuery query(db);

    QString q = QString("DELETE from %1  WHERE id = :id ").arg(RentTableName);

    query.prepare(q);
    query.bindValue(":id", id);

    if (query.exec()) {

        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------

bool RentModel::checkCarUsed(QSqlDatabase db, const QDateTime& fromDate, const QDateTime& toDate, QStringList& ids)
{
    QSqlQuery query(db);

    QString q = QString("SELECT DISTINCT car from %1  WHERE (date_to >= :from) AND  (date_from <= :to) ").arg(RentTableName);

    query.prepare(q);
    query.bindValue(":from", fromDate.toString(DateTimeFormat));
    query.bindValue(":to", toDate.toString(DateTimeFormat));

    if (query.exec()) {

        while (query.next()) {
            QSqlRecord reci = query.record();

            ids.append(reci.value(0).toString());

        }

        qDebug() << ids.join(",") << "\n";

        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------

bool RentModel::getLastDate(QSqlDatabase db, int carid, QDateTime& last)
{
    QSqlQuery query(db);

    QString q = QString("SELECT date_to from %1  WHERE (car = :carid) ORDER BY date_to DESC LIMIT 1; ").arg(RentTableName);

    query.prepare(q);
    query.bindValue(":carid", carid);
    if (query.exec()) {

        if (query.next()) {
            QSqlRecord reci = query.record();

            QString strValue = reci.value(0).toString();
            last = QDateTime::fromString(strValue, DateTimeFormat);

            qDebug() << strValue << "\n";
        }
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
int RentModel::getRentArcLength(int fromloc, int toloc, int fromangle, int toangle)
{
    // from-to or to-from have the same price, calc min length
    int n_jump = abs(fromloc - toloc);
    int min_id = (fromloc);
    if (min_id > toloc)
        min_id = toloc;

    return (int)(arc(fromangle, toangle, min_id * 5));
}
//--------------------------------------------------------------------------------
int RentModel::getRentLineLength(int fromloc, int toloc, int fromangle, int toangle)
{
    // from-to or to-from have the same price, calc min length
    int n_jump = abs(fromloc - toloc);
    int min_id = (fromloc);
    if (min_id > toloc)
        min_id = toloc;
    if (fromloc == 1) // this is an exception
        n_jump++;

    return (int)(n_jump * 5 + 5);
}
