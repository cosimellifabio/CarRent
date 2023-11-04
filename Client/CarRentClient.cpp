// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "CarRentClient.h"
#include <QFile.h>
#include <QTextStream.h>
#include <QTimer.h>
#include <QDebug>

//--------------------------------------------------------------------------------
CarRentClientForm::CarRentClientForm(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.btnEXIT, SIGNAL(clicked()), this, SLOT(btnExitSlot()));
	connect(ui.btnRefresh, SIGNAL(clicked()), this, SLOT(refreshAll()));
	connect(ui.btnUserCreate, SIGNAL(clicked()), this, SLOT(createUser()));
	connect(ui.btnUserDelete, SIGNAL(clicked()), this, SLOT(deleteUser()));
	connect(ui.btnEditUser, SIGNAL(clicked()), this, SLOT(editUser()));
	connect(ui.btnCarCreate, SIGNAL(clicked()), this, SLOT(createCar()));
	connect(ui.btnCarDelete, SIGNAL(clicked()), this, SLOT(deleteCar()));
	connect(ui.btnCarUser, SIGNAL(clicked()), this, SLOT(editCar()));

	// db init
	QString connName;
	createConnection(connName, 5432,
		"localhost",
		"postgres",
		"pgpw",
		"carrent",
		"CarRentClient");
	m_userModel = new UserModel(this, m_db, ui.tblUsers);
	m_userModel->init();
	connect(ui.tblUsers->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(setUser(const QItemSelection&, const QItemSelection&)));

	refreshUser();
	m_carModel = new CarModel(this, m_db, ui.tblCars);
	m_carModel->init();
	connect(ui.tblCars->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(setCar(const QItemSelection&, const QItemSelection&)));

	refreshCar();
}
//--------------------------------------------------------------------------------

void CarRentClientForm::refreshAll() {
	refreshUser();
	refreshCar();
}
//--------------------------------------------------------------------------------


void CarRentClientForm::btnExitSlot()
{
	exit(1);
}
//--------------------------------------------------------------------------------

bool CarRentClientForm::createConnection(
	QString& connName,
	int   port,
	const QString& address,
	const QString& username,
	const QString& password,
	const QString& dbName,
	const QString& application_name)
{
	connName = QString("%1_%2_%3_%4")
		.arg(address)
		.arg(QString::number(port))
		.arg("CarRentClient")
		.arg(QString::number(std::rand()));

	m_db = QSqlDatabase::addDatabase("QPSQL", connName);

	QString application_name_th = application_name + connName;

	m_db.setHostName(address);
	m_db.setDatabaseName(dbName);
	m_db.setUserName(username);
	m_db.setPort(port);
	m_db.setPassword(password.toLatin1().data());
	m_db.setConnectOptions("application_name='" + application_name_th + "'");

	if (!m_db.open())
	{
		return false;
	}

	return true;
}
//--------------------------------------------------------------------------------

void CarRentClientForm::createUser() {

	m_userModel->create(m_db, ui.nedUserName->text(), ui.nedUserSurname->text(), ui.nedUserAddress->text(), ui.nedUserCredtCard->text(), ui.nedUserDrivingLic->text());
	refreshUser();
}
//--------------------------------------------------------------------------------

void CarRentClientForm::setUser(const QItemSelection& selected, const QItemSelection& deselected)
{
	if (!selected.size())
		return;
	auto  l = selected.at(0).topLeft();

	int id;
	QString name, surname, address, credit, driving;
	if (m_userModel->getUser(m_db, l, id, name, surname, address, credit, driving)) {
		ui.nedUserName->setText(name);
		ui.nedUserSurname->setText(surname);
		ui.nedUserAddress->setText(address);
		ui.nedUserCredtCard->setText(credit);
		ui.nedUserDrivingLic->setText(driving);
		ui.nedUserId->setText(QString::number(id));
	}
}
//--------------------------------------------------------------------------------

void CarRentClientForm::deleteUser() {


	m_userModel->deleteUser(m_db, ui.nedUserId->text().toInt());
	refreshUser();
}
//--------------------------------------------------------------------------------

void CarRentClientForm::editUser() {


	m_userModel->edit(m_db, ui.nedUserId->text().toInt(), ui.nedUserName->text(), ui.nedUserSurname->text(), ui.nedUserAddress->text(), ui.nedUserCredtCard->text(), ui.nedUserDrivingLic->text());
	refreshUser();
}
//--------------------------------------------------------------------------------

void CarRentClientForm::refreshUser() {
	m_userModel->sort(0, Qt::SortOrder::AscendingOrder);
	m_userModel->select();
	m_userModel->show();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

void CarRentClientForm::createCar() {

	m_carModel->create(m_db, ui.nedCarName->text(), ui.nedCarBrand->text(), ui.nedCarTail->text(), ui.nedCarClass->text());
	refreshCar();
}
//--------------------------------------------------------------------------------

void CarRentClientForm::setCar(const QItemSelection& selected, const QItemSelection& deselected)
{
	if (!selected.size())
		return;
	auto  l = selected.at(0).topLeft();

	int id;
	QString name, surname, address, credit;
	if (m_carModel->getCar(m_db, l, id, name, surname, address, credit)) {
		ui.nedCarName->setText(name);
		ui.nedCarBrand->setText(surname);
		ui.nedCarTail->setText(address);
		ui.nedCarClass->setText(credit);
		ui.nedCarId->setText(QString::number(id));
	}

}
//--------------------------------------------------------------------------------

void CarRentClientForm::deleteCar() {


	m_carModel->deleteCar(m_db, ui.nedCarId->text().toInt());
	refreshCar();
}
//--------------------------------------------------------------------------------

void CarRentClientForm::editCar() {


	m_carModel->edit(m_db, ui.nedCarId->text().toInt(), ui.nedCarName->text(), ui.nedCarBrand->text(), ui.nedCarTail->text(), ui.nedCarClass->text());
	refreshCar();
}
//--------------------------------------------------------------------------------

void CarRentClientForm::refreshCar() {
	m_carModel->sort(0, Qt::SortOrder::AscendingOrder);
	m_carModel->select();
	m_carModel->show();
}
//--------------------------------------------------------------------------------
