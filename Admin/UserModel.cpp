#include "UserModel.h"
#include <QSqlQuery.h>
#include <QSqlRecord.h>
#include <QDebug.h>

const QString UserModel::userTableName = "users";

//--------------------------------------------------------------------------------
void UserModel::init() {
    this->setTable(userTableName);
    this->setEditStrategy(QSqlTableModel::OnManualSubmit);
    this->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    this->setHeaderData(1, Qt::Horizontal, QObject::tr("First name"));
    this->setHeaderData(2, Qt::Horizontal, QObject::tr("Last name"));
    if (m_view) {
        m_view->setModel(this);
        m_view->setWindowTitle("USERS");
        m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    }
};
//--------------------------------------------------------------------------------
void UserModel::show() {

    select();
    if (m_view) m_view->show();
};
//--------------------------------------------------------------------------------
bool UserModel::create(QSqlDatabase db, const QString& name, const QString& surname, const QString& address, const QString& credit, const QString& driving)
{
    QSqlQuery query(db);

    QString q = QString("INSERT INTO %1 "
        "(name, surname, address, credit_card, driving_license) VALUES ("
        " :name, :surname, :address, :credit, :driving  )").arg(userTableName);

    query.prepare(q);
    query.bindValue(":name", name);
    query.bindValue(":surname", surname);
    query.bindValue(":address", address);
    query.bindValue(":credit", credit);
    query.bindValue(":driving", driving);

    return query.exec();
}
//--------------------------------------------------------------------------------
bool UserModel::selectUser(QSqlDatabase db, int id, QString& name, QString& surname, QString& address, QString& credit, QString& driving)
{
    QSqlQuery query(db);

    QString q = QString("SELECT * from %1  WHERE id = :id ").arg(userTableName);

    query.prepare(q);
    query.bindValue(":id", id);

    if (query.exec()) {
        
        query.next();
        QSqlRecord reci = query.record();

        name = reci.value(1).toString();
        surname = reci.value(2).toString();
        address = reci.value(3).toString();
        credit = reci.value(4).toString();
        driving = reci.value(5).toString();

        qDebug() << id << name << "\n";
          
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
bool UserModel::getUser(QSqlDatabase db, const QModelIndex& i, int& id, QString& name, QString& surname, QString& address, QString& credit, QString& driving)
{
    id = this->data(i).toInt();
    return selectUser(db, id, name, surname, address, credit, driving);
}
//--------------------------------------------------------------------------------
bool UserModel::edit(QSqlDatabase db, int id, const QString& name, const QString& surname, const QString& address, const QString& credit, const QString& driving)
{
    QSqlQuery query(db);

    QString q = QString("UPDATE %1 SET name=:name, surname=:surname, address=:address, credit_card=:credit, driving_license=:driving, updatedAt=now() WHERE id = :id ").arg(userTableName);

    query.prepare(q);
    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.bindValue(":surname", surname);
    query.bindValue(":address", address);
    query.bindValue(":credit", credit);
    query.bindValue(":driving", driving);
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
bool UserModel::deleteUser(QSqlDatabase db, int id)
{
    QSqlQuery query(db);

    QString q = QString("DELETE from %1  WHERE id = :id ").arg(userTableName);

    query.prepare(q);
    query.bindValue(":id", id);

    if (query.exec()) {

        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
bool UserModel::getLogin(QSqlDatabase db, const QString& name, const QString& surname, int &id, QString& address, QString& credit, QString& driving)
{
    QSqlQuery query(db);

    QString q = QString("SELECT * from %1  WHERE name = :name AND  surname = :surname ").arg(userTableName);

    query.prepare(q);
    query.bindValue(":name", name);
    query.bindValue(":surname", surname);

    if (query.exec()) {

        if (query.next()) {
            QSqlRecord reci = query.record();

            id = reci.value(0).toInt();
            address = reci.value(3).toString();
            credit = reci.value(4).toString();
            driving = reci.value(5).toString();

            qDebug() << id << name << "\n";
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------

