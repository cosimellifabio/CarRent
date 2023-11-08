// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "CarRentClient.h"
#include <QFile.h>
#include <QTextStream.h>
#include <QTimer.h>
#include <QDebug>
#include <QMessageBox>
#include <QSqlRecord>
#include <QComboBox>
#include <QSpinBox>

#include <QSqlQuery.h>
#include <QSqlRecord.h>

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
	connect(ui.btnLogin, SIGNAL(clicked()), this, SLOT(loginUser()));

	connect(ui.btnTripBuy, SIGNAL(clicked()), this, SLOT(buyTrip()));
	//connect(ui.btnTripLoad, SIGNAL(clicked()), this, SLOT(loadTrip()));
	//connect(ui.btnTrip, SIGNAL(clicked()), this, SLOT(editCar()));

	ui.cmbPassengers->addItem("1");
	ui.cmbPassengers->addItem("2");
	ui.cmbPassengers->addItem("3");
	ui.cmbPassengers->addItem("4");
	ui.cmbPassengers->addItem("5");
	ui.cmbPassengers->addItem("6");
	ui.cmbPassengers->addItem("7");

	ui.dateFrom->setMinimumDateTime(QDateTime::currentDateTime());
	ui.dateTo->setMinimumDateTime(QDateTime::currentDateTime());

	// db init
	QString connName;
	createConnection(connName, 5432,
		"localhost",
		"postgres",
		"pgpw",
		"carrent",
		"CarRentClient");

	m_userModel = new UserModel(this, m_db, NULL);
	m_userModel->init();
	refreshUser();

	m_carTypesModel = new CarTypesModel(this, m_db, NULL);
	m_carTypesModel->init();

	m_carModel = new CarModel(this, m_db, ui.tblCars);
	m_carModel->init();
	connect(ui.tblCars->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(setCar(const QItemSelection&, const QItemSelection&)));
	refreshCar();

	m_locModel = new LocationsModel(this, m_db, NULL);
	m_locModel->init();
	m_locModel->getList(m_db, ui.cmbFrom);
	m_locModel->getList(m_db, ui.cmbTo);

	m_rentModel = new RentModel(this, m_db, ui.tableRents);
	m_rentModel->init();
	//connect(ui.tableRents->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(setRent(const QItemSelection&, const QItemSelection&)));

	connect(ui.dateFrom, &QDateTimeEdit::dateTimeChanged, this, &CarRentClientForm::dateTimeRentChanged);
	//connect(ui.dateTo, &QDateTimeEdit::dateTimeChanged, this, &CarRentClientForm::dateTimeRentChanged);
	connect(ui.cmbFrom, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(cmbChanged(const QString&)));
	connect(ui.cmbTo, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(cmbChanged(const QString&)));
	connect(ui.sbxFrom, SIGNAL(valueChanged(const QString&)), this, SLOT(cmbChanged(const QString&)));
	connect(ui.sbxTo, SIGNAL(valueChanged(const QString&)), this, SLOT(cmbChanged(const QString&)));
}
//--------------------------------------------------------------------------------

void CarRentClientForm::refreshAll() {
	refreshUser();
	refreshCar();
	refreshRent();
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

void CarRentClientForm::loginUser() {

	int id;
	QString address, credit, driving;
	if (m_userModel->getLogin(m_db, ui.nedUserName->text(), ui.nedUserSurname->text(), id, address, credit, driving)) {
		ui.nedUserAddress->setText(address);
		ui.nedUserCredtCard->setText(credit);
		ui.nedUserDrivingLic->setText(driving);
		ui.nedUserId->setText(QString::number(id));

		refreshRent();

		int carid;
		if (m_userModel->getPreferredCar(m_db, id, carid) && carid) {
			for (size_t i = 0; i < m_carModel->rowCount(); i++)
			{
				int idc = m_carModel->record(i).value("id").toInt();

				if (idc == carid) {
					ui.tblCars->selectRow(i);
					//m_carModel->selectRow(i);
				}

			}
		}
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setText("User not Found, set name and surname");
		//msgBox.setInformativeText("Do you want to save your changes?");
		//msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		//msgBox.setDefaultButton(QMessageBox::Save);
		int ret = msgBox.exec();
	}
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

/*void CarRentClientForm::createCar() {

	//m_carModel->create(m_db, ui.nedCarName->text(), ui.nedCarBrand->text(), ui.nedCarTail->text(), ui.nedCarClass->text());
	refreshCar();
}*/
//--------------------------------------------------------------------------------

void CarRentClientForm::setCar(const QItemSelection& selected, const QItemSelection& deselected)
{
	if (!selected.size())
		return;
	auto  l = selected.at(0).topLeft();

	int id, next_service_km;
	QString name, brand, tail_number, class1;
	if (m_carModel->getCar(m_db, l, m_carSelected, name, brand, tail_number, class1, next_service_km)) {
		ui.nedCarName->setText(name);
		//		ui.nedCarId->setText(QString::number(id));

		int price = 1;
		int km = 5, seconds = 0;
		calculateTrip(km, price, seconds); // to calculate time to
	}

}
//--------------------------------------------------------------------------------

void CarRentClientForm::loadTrip() {

	//m_carModel->deleteCar(m_db, ui.nedCarId->text().toInt());
	refreshCar();
}
//--------------------------------------------------------------------------------

void CarRentClientForm::buyTrip() {

	QStringList ids;
	if (m_rentModel->checkCarUsed(m_db, ui.dateFrom->dateTime(), ui.dateTo->dateTime(), ids) && ids.size() && (ids.indexOf(QString::number(m_carSelected)) >= 0)) {


		QSqlQuery query2(m_db);

		QString q2 = QString("WITH max_dates AS ( SELECT car, MAX(date_to) AS max_date FROM rents GROUP BY car) SELECT MIN(max_date) FROM max_dates; ");
		query2.prepare(q2);

		if (query2.exec()) {
			if (query2.next()) {

				QSqlRecord reci2 = query2.record();


				QDateTime last = QDateTime::fromString(reci2.value(0).toString(), RentModel::DateTimeFormat);
				if (last > QDateTime::currentDateTime()) {
					QMessageBox msgBox;
					msgBox.setText("No Car is avaliable! Next avaliable date: " + reci2.value(0).toString());
					//msgBox.setInformativeText("Do you want to save your changes?");
					//msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
					//msgBox.setDefaultButton(QMessageBox::Save);
					int ret = msgBox.exec();
					return;

				}
			}
		}




		QMessageBox msgBox;
		msgBox.setText("Car Not avaliable!");
		//msgBox.setInformativeText("Do you want to save your changes?");
		//msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		//msgBox.setDefaultButton(QMessageBox::Save);
		int ret = msgBox.exec();
		return;
	}

	QString name, brand, tail_number, class1;
	int next_service_km;
	if (!m_carModel->selectCar(m_db, m_carSelected, name, brand, tail_number, class1, next_service_km)) {
		QMessageBox msgBox;
		msgBox.setText("Select a car!");
		//msgBox.setInformativeText("Do you want to save your changes?");
		//msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		//msgBox.setDefaultButton(QMessageBox::Save);
		int ret = msgBox.exec();
		return;
	}
	int pricekm, speed, max_persons, service_km, service_price;
	m_carTypesModel->selectType(m_db, class1, pricekm, speed, max_persons, service_km, service_price);
	if (ui.cmbPassengers->currentText().toInt() > max_persons) {
		QMessageBox msgBox;
		msgBox.setText("Too much passengers, Select another car!");
		//msgBox.setInformativeText("Do you want to save your changes?");
		//msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		//msgBox.setDefaultButton(QMessageBox::Save);
		int ret = msgBox.exec();
		return;
	}

	int idFrom, beforeFrom, afterFrom, hopsFrom, priceFrom;
	m_locModel->selectLocation(m_db, ui.cmbFrom->currentText(), idFrom, beforeFrom, afterFrom, hopsFrom, priceFrom);
	int idTo, beforeTo, afterTo, hopsTo, priceTo;
	m_locModel->selectLocation(m_db, ui.cmbTo->currentText(), idTo, beforeTo, afterTo, hopsTo, priceTo);

	int price = 1;
	int km = 5, seconds = 0;
	calculateTrip(km, price,  seconds);
	
	QMessageBox msgBox;
	msgBox.setText("The price is " + QString::number(price) + " do you want to proceed with the payment? \n If you say YES the payment will be done");
	//msgBox.setInformativeText("Do you want to save your changes?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Discard);
	msgBox.setDefaultButton(QMessageBox::Discard);
	int ret = msgBox.exec();
	if (ret == QMessageBox::Yes) {

		m_rentModel->create(m_db, ui.nedTripName->text(), ui.nedUserId->text().toInt(), m_carSelected,
			idFrom, idTo, ui.sbxFrom->value(), ui.sbxTo->value(), price, km, ui.cmbPassengers->currentText().toInt(), ui.dateFrom->dateTime(), ui.dateTo->dateTime());
		
		manageService(km);
		refreshAll();
		QMessageBox msgBox;
		msgBox.setText("Transaction Done!");
		//msgBox.setInformativeText("Do you want to save your changes?");
		//msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		//msgBox.setDefaultButton(QMessageBox::Save);
		int ret = msgBox.exec();
	}

}
//--------------------------------------------------------------------------------

void CarRentClientForm::calculateTrip(int &km, int &price, int &seconds) {

	km = price = seconds = 0;
	
	QString name, brand, tail_number, class1;
	int next_service_km;
	if (!m_carModel->selectCar(m_db, m_carSelected, name, brand, tail_number, class1, next_service_km)) {
		return;
	}
	int pricekm, speed, max_persons, service_km, service_price;
	m_carTypesModel->selectType(m_db, class1, pricekm, speed, max_persons, service_km, service_price);

	int idFrom, beforeFrom, afterFrom, hopsFrom, priceFrom;
	m_locModel->selectLocation(m_db, ui.cmbFrom->currentText(), idFrom, beforeFrom, afterFrom, hopsFrom, priceFrom);
	int idTo, beforeTo, afterTo, hopsTo, priceTo;
	m_locModel->selectLocation(m_db, ui.cmbTo->currentText(), idTo, beforeTo, afterTo, hopsTo, priceTo);

	km = m_rentModel->getRentArcLength(idFrom, idTo, ui.sbxFrom->value(), ui.sbxTo->value()) + m_rentModel->getRentLineLength(idFrom, idTo, ui.sbxFrom->value(), ui.sbxTo->value());//   (int)(arc(ui.sbxFrom->value(), ui.sbxTo->value(), min_id * 5) + n_jump * 5 + 5);
	price = (int)((double)km / (double)pricekm + 0.5);
	seconds = (int)((double)(km * 3600.) / (double)speed + 0.5);

	ui.dateTo->setDateTime(ui.dateFrom->dateTime().addSecs(seconds));
}
//--------------------------------------------------------------------------------

void CarRentClientForm::refreshCar() {
	m_carModel->sort(0, Qt::SortOrder::AscendingOrder);
	m_carModel->select();
	m_carModel->show();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

void CarRentClientForm::setRent(const QItemSelection& selected, const QItemSelection& deselected)
{
	if (!selected.size())
		return;
	auto  l = selected.at(0).topLeft();

	int id;
}
//--------------------------------------------------------------------------------

void CarRentClientForm::refreshRent() {

	if (ui.nedUserId->text().toInt()) {

		m_rentModel->sort( 12, Qt::SortOrder::AscendingOrder);
		m_rentModel->setFilter("userid=" + ui.nedUserId->text());
		m_rentModel->select();
		m_rentModel->show();
	}
}
//--------------------------------------------------------------------------------
void CarRentClientForm::refreshTimeTo() {

	int price = 1;
	int km = 5, seconds = 0;
	calculateTrip(km, price, seconds); // to calculate time to
}
//--------------------------------------------------------------------------------
void CarRentClientForm::dateTimeRentChanged(const QDateTime& datetime) {

	m_carModel->sort(0, Qt::SortOrder::AscendingOrder);
	QStringList ids;
	if (m_rentModel->checkCarUsed(m_db, ui.dateFrom->dateTime(), ui.dateTo->dateTime(), ids) && ids.size()) {
		m_carModel->setFilter("id NOT IN (" + ids.join(",") + ")");
	}
	else {
		m_carModel->setFilter("");
	}
	m_carModel->select();
	m_carModel->show();

	refreshTimeTo();
}
//--------------------------------------------------------------------------------
// QComboBox::currentIndexChanged(const QString &text)
void CarRentClientForm::cmbChanged(const QString& text) {
	refreshTimeTo();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

void CarRentClientForm::manageService(int km) {

	if (km) {
		QString name, brand, tail_number, class1;
		int next_service_km;
		if (!m_carModel->selectCar(m_db, m_carSelected, name, brand, tail_number, class1, next_service_km)) {
			return;
		}
		int pricekm, speed, max_persons, service_km, service_price;
		m_carTypesModel->selectType(m_db, class1, pricekm, speed, max_persons, service_km, service_price);

		if (next_service_km < km) {
			next_service_km = service_km;
			// add a service and the payment
			QDateTime last;
			m_rentModel->getLastDate(m_db, m_carSelected, last);
			m_rentModel->create(m_db, "SERVICE FOR CAR " + name, 1, m_carSelected,
				1, 1, 0, 0, -service_price, 0, 1, last, last.addDays(1));
		}
		else
			next_service_km -= km;
		m_carModel->updateService(m_db, m_carSelected, next_service_km);
	}
}
