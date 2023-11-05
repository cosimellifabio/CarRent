#pragma once
#include <QSqlTableModel.h>
#include <QTableView.h>
class CarModel :  public QSqlTableModel
{
public:
    CarModel(QObject* parent ,  QSqlDatabase db, QTableView * t) :QSqlTableModel(parent, db), m_view(t){
    };
    void init();
    void show();
    bool create(QSqlDatabase db, const QString& name, const QString& brand, const QString& tail_number, const QString& class1);
    bool getCar(QSqlDatabase db, const QModelIndex& i, int & id, QString& name, QString& brand, QString& tail_number, QString& class1, int& next_service_km);
    bool selectCar(QSqlDatabase db, int id, QString& name, QString& brand, QString& tail_number, QString& class1, int& service);
    bool deleteCar(QSqlDatabase db, int id);
    bool edit(QSqlDatabase db, int id, const QString& name, const QString& brand, const QString& tail_number, const QString& class1);

    bool updateService(QSqlDatabase db, int id, int km_to_do);
	static const QString CarTableName;
protected:

    QTableView* m_view;
};

