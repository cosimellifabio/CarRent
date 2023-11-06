#pragma once
#include <QSqlTableModel.h>
#include <QTableView.h>
#include <QDateTime.h>
class RentModel :  public QSqlTableModel
{
public:
    RentModel(QObject* parent ,  QSqlDatabase db, QTableView * t) :QSqlTableModel(parent, db), m_view(t){
    };
    void init();
    void show();
    bool create(QSqlDatabase db, const QString& name, int userid, int car, int fromloc, int toloc, int fromangle, int toangle, int price, int km,
        int passengers, const QDateTime& fromDate, const QDateTime& toDate);

    bool getRent(QSqlDatabase db, const QModelIndex& i, int & id, int &fromloc, int &toloc, int &fromangle, int &toangle);
    bool selectRent(QSqlDatabase db, int id, int& fromloc, int& toloc, int& fromangle, int& toangle);



    bool deleteRent(QSqlDatabase db, int id);
    bool edit(QSqlDatabase db, int id, const QString& name, const QString& brand, const QString& tail_number, const QString& class1);

    bool checkCarUsed(QSqlDatabase db, const QDateTime& fromDate, const QDateTime& toDate, QStringList& ids);
    bool getLastDate(QSqlDatabase db, int carid, QDateTime& last);

protected:
    static const QString RentTableName;
    static const QString DateTimeFormat;

    QTableView* m_view;
};

