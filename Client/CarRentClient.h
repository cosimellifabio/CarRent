// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef CALCULATORFORM_H
#define CALCULATORFORM_H
//! [0]
#include "ui_CarRentClient.h"
//! [0]

//! [1]


#include "UserModel.h"
#include "CarModel.h"
#include "LocationsModel.h"
#include <QSqlDatabase.h>

class CarRentClientForm : public QWidget
{
    Q_OBJECT

public:
    explicit CarRentClientForm(QWidget *parent = nullptr);

protected:
    bool createConnection(QString& connName,
		int              port,
		const QString& address,
		const QString& username,
		const QString& password,
		const QString& dbName,
		const QString& application_name);

private slots:
    void btnExitSlot();
	void refreshUser();
	void createUser();
	void deleteUser();
	void editUser();
	void loginUser();
	void setUser(const QItemSelection& selected, const QItemSelection& deselected);
	
	void refreshCar();
	void buyTrip();
	void loadTrip();
	//void editCar();
	void setCar(const QItemSelection& selected, const QItemSelection& deselected);
	void refreshAll();
private:
    Ui::CarRentClient ui;

    QSqlDatabase m_db;

	UserModel* m_userModel;
	CarModel* m_carModel;
	LocationsModel* m_locModel;
};
//! [1]

#endif
