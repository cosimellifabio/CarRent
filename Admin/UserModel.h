#pragma once
#include <QSqlTableModel.h>
#include <QTableView.h>
class UserModel :  public QSqlTableModel
{
public:
    UserModel(QObject* parent ,  QSqlDatabase db, QTableView * t) :QSqlTableModel(parent, db), m_view(t){
    };
    void init();
    void show();
    bool create(QSqlDatabase db, const QString& name, const QString& surname, const QString& address, const QString& credit, const QString& driving);
    bool getUser(QSqlDatabase db, const QModelIndex& i, int & id, QString& name, QString& surname, QString& address, QString& credit, QString& driving);
    bool selectUser(QSqlDatabase db, int id, QString& name, QString& surname, QString& address, QString& credit, QString& driving);
    bool deleteUser(QSqlDatabase db, int id);
    bool edit(QSqlDatabase db, int id, const QString& name, const QString& surname, const QString& address, const QString& credit, const QString& driving);

    bool getLogin(QSqlDatabase db, const QString& name, const QString& surname, int& id, QString& address, QString& credit, QString& driving);
    bool getPreferredCar(QSqlDatabase db, int id, int& carid);
protected:
	static const QString userTableName;

    QTableView* m_view;
};

