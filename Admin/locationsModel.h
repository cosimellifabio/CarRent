#pragma once
#include <QSqlTableModel.h>
#include <QTableView.h>
#include <QComboBox.h>
class LocationsModel : public QSqlTableModel
{
public:
    LocationsModel(QObject* parent, QSqlDatabase db, QTableView* t) :QSqlTableModel(parent, db), m_view(t) {
    };
    void init();

    bool getList(QSqlDatabase db, QComboBox* cmb);

    bool selectLocation(QSqlDatabase db, int id, QString& name, int& before, int& after, int& hops, int& price);
    bool selectLocation(QSqlDatabase db, const QString& name, int& id, int& before, int& after, int& hops, int& price);

    bool getPriceHops(const QString& nameFrom, const QString& nameTo, int& price, int& hops);

protected:
    static const QString locTableName;

    QTableView* m_view;
};

