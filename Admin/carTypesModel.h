#pragma once
#include <QSqlTableModel.h>
#include <QTableView.h>
#include <QComboBox.h>
class CarTypesModel : public QSqlTableModel
{
public:
    CarTypesModel(QObject* parent, QSqlDatabase db, QTableView* t) :QSqlTableModel(parent, db), m_view(t) {
    };
    void init();

    bool getList(QSqlDatabase db, QComboBox* cmb);

    bool selectType(QSqlDatabase db, const QString& name, int& price, int& speed, int& max_persons, int& service_km, int& service_price);


protected:
    static const QString locTableName;

    QTableView* m_view;
};

