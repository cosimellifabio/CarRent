// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef CALCULATORFORM_H
#define CALCULATORFORM_H
//! [0]
#include "ui_CarRent.h"
//! [0]

//! [1]


#include "UserModel.h"
#include "CarModel.h"
#include "LocationsModel.h"
#include "carTypesModel.h"
#include "RentModel.h"
#include <QSqlDatabase.h>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>

class CarRentForm : public QWidget
{
    Q_OBJECT

public:
    explicit CarRentForm(QWidget *parent = nullptr);

protected:
    bool createConnection(QString& connName,
		int              port,
		const QString& address,
		const QString& username,
		const QString& password,
		const QString& dbName,
		const QString& application_name);
	void refreshRent();
	void refreshUser();
	void refreshCar();

private slots:
    void btnExitSlot();
	void createUser();
	void deleteUser();
	void editUser();
	void setUser(const QItemSelection& selected, const QItemSelection& deselected);
	void createCar();
	void deleteCar();
	void editCar();
	void setCar(const QItemSelection& selected, const QItemSelection& deselected);
	void refreshAll();
	void setRent(const QItemSelection& selected, const QItemSelection& deselected);

	void report();
private:
    Ui::CarRent ui;

    QSqlDatabase m_db;

	UserModel* m_userModel;
	CarModel* m_carModel;
	LocationsModel* m_locModel;
	CarTypesModel* m_carTypesModel;
	RentModel* m_rentModel;

	int m_carSelected = 0;

	QGraphicsScene* m_scene;
};
//! [1]

#endif
