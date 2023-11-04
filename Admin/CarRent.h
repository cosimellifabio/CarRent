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
#include <QSqlDatabase.h>

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

private slots:
    void btnExitSlot();
	void refreshUser();
	void createUser();
	void deleteUser();
	void editUser();
	void setUser(const QItemSelection& selected, const QItemSelection& deselected);
	void refreshCar();
	void createCar();
	void deleteCar();
	void editCar();
	void setCar(const QItemSelection& selected, const QItemSelection& deselected);
	void refreshAll();
private:
    Ui::CarRent ui;

    QSqlDatabase m_db;

	UserModel* m_userModel;
	CarModel* m_carModel;
};
//! [1]

#endif
