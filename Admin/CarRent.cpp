// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "CarRent.h"
#include <QFile.h>
#include <QTextStream.h>
#include <QTimer.h>
#include <QDebug>
#include <QFileDialog>

#include <QSqlQuery.h>
#include <QSqlRecord.h>
//--------------------------------------------------------------------------------
CarRentForm::CarRentForm(QWidget* parent)
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
	connect(ui.btnReport, SIGNAL(clicked()), this, SLOT(report()));

	// db init
	QString connName;
	createConnection(connName, 5432,
		"localhost",
		"postgres",
		"pgpw",
		"carrent",
		"CarRent");
	m_userModel = new UserModel(this, m_db, ui.tblUsers);
	m_userModel->init();
	connect(ui.tblUsers->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(setUser(const QItemSelection&, const QItemSelection&)));

	refreshUser();
	m_carModel = new CarModel(this, m_db, ui.tblCars);
	m_carModel->init();
	connect(ui.tblCars->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(setCar(const QItemSelection&, const QItemSelection&)));

	m_carTypesModel = new CarTypesModel(this, m_db, NULL);
	m_carTypesModel->init();

	m_locModel = new LocationsModel(this, m_db, NULL);
	m_locModel->init();

	m_rentModel = new RentModel(this, m_db, ui.tableRents);
	m_rentModel->init();
	connect(ui.tableRents->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(setRent(const QItemSelection&, const QItemSelection&)));
	refreshAll();

	m_scene = new QGraphicsScene(this);
	ui.graphicsView->setScene(m_scene);

	QPen outlinePen(Qt::white);
	outlinePen.setWidth(2);

	int w = ui.graphicsView ->width() - 50;
	int step = w / 3;
	int center = w / 2;
	// addEllipse(x,y,w,h,pen,brush)
	m_scene->addEllipse(0, 0, w, w, outlinePen, QBrush(Qt::red));
	m_scene->addEllipse(step/2, step/2, w - step, w - step, outlinePen, QBrush(Qt::green));
	m_scene->addEllipse(step, step, w - 2*step, w - 2*step, outlinePen, QBrush(Qt::red));
}
//--------------------------------------------------------------------------------

void CarRentForm::refreshAll() {
	refreshUser();
	refreshCar();
	refreshRent();
}
//--------------------------------------------------------------------------------

void CarRentForm::refreshRent() {

	m_rentModel->sort(12, Qt::SortOrder::AscendingOrder);
	m_rentModel->select();
	m_rentModel->show();
}
//--------------------------------------------------------------------------------


void CarRentForm::btnExitSlot()
{
	exit(1);
}
//--------------------------------------------------------------------------------

bool CarRentForm::createConnection(
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
		.arg("CarRent")
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

void CarRentForm::createUser() {

	m_userModel->create(m_db, ui.nedUserName->text(), ui.nedUserSurname->text(), ui.nedUserAddress->text(), ui.nedUserCredtCard->text(), ui.nedUserDrivingLic->text());
	refreshUser();
}
//--------------------------------------------------------------------------------

void CarRentForm::setUser(const QItemSelection& selected, const QItemSelection& deselected)
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

void CarRentForm::deleteUser() {


	m_userModel->deleteUser(m_db, ui.nedUserId->text().toInt());
	refreshUser();
}
//--------------------------------------------------------------------------------

void CarRentForm::editUser() {


	m_userModel->edit(m_db, ui.nedUserId->text().toInt(), ui.nedUserName->text(), ui.nedUserSurname->text(), ui.nedUserAddress->text(), ui.nedUserCredtCard->text(), ui.nedUserDrivingLic->text());
	refreshUser();
}
//--------------------------------------------------------------------------------

void CarRentForm::refreshUser() {
	m_userModel->sort(0, Qt::SortOrder::AscendingOrder);
	m_userModel->select();
	m_userModel->show();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

void CarRentForm::createCar() {

	m_carModel->create(m_db, ui.nedCarName->text(), ui.nedCarBrand->text(), ui.nedCarTail->text(), ui.nedCarClass->text());
	refreshCar();
}
//--------------------------------------------------------------------------------

void CarRentForm::setCar(const QItemSelection& selected, const QItemSelection& deselected)
{
	if (!selected.size())
		return;
	auto  l = selected.at(0).topLeft();

	int id, ns;
	QString name, surname, address, credit;
	if (m_carModel->getCar(m_db, l, id, name, surname, address, credit, ns)) {
		ui.nedCarName->setText(name);
		ui.nedCarBrand->setText(surname);
		ui.nedCarTail->setText(address);
		ui.nedCarClass->setText(credit);
		ui.nedCarId->setText(QString::number(id));
	}

}
//--------------------------------------------------------------------------------

void CarRentForm::setRent(const QItemSelection& selected, const QItemSelection& deselected)
{
	if (!selected.size())
		return;
	auto  l = selected.at(0).topLeft();

	int id, fromloc, toloc, fromangle, toangle;
	if (m_rentModel->getRent(m_db, l, id, fromloc, toloc, fromangle, toangle)) {

		m_scene->clear();
		QPen outlinePen(Qt::white);
		outlinePen.setWidth(2);

		int w = ui.graphicsView->width() - 50;
		int step = w / 3;
		int center = w / 2;
		// addEllipse(x,y,w,h,pen,brush)
		m_scene->addEllipse(0, 0, w, w, outlinePen, QBrush(Qt::red));
		m_scene->addEllipse(step / 2, step / 2, w - step, w - step, outlinePen, QBrush(Qt::green));
		m_scene->addEllipse(step, step, w - 2 * step, w - 2 * step, outlinePen, QBrush(Qt::red));

		QPen outlinePen2(Qt::black);
		outlinePen2.setWidth(4);

		double rad = fromangle * M_PI / 180.;
		m_scene->addEllipse(center + step * (fromloc)* cos(rad) / 2, center + step * (fromloc) * sin(rad) / 2, 2, 2, outlinePen2, QBrush(Qt::black));

		rad = toangle * M_PI / 180.;
		m_scene->addEllipse(center + step * (toloc) * cos(rad) / 2, center + step * (toloc)* sin(rad) / 2, 2, 2, outlinePen2, QBrush(Qt::black));

		int arccircle = fromloc;
		if (toloc < fromloc) {

			arccircle = toloc;
			rad = fromangle * M_PI / 180.;
		}
		else  {
			rad =toangle * M_PI / 180.;
		}
		m_scene->addLine(center + step * (fromloc)*cos(rad) / 2, center + step * (fromloc)*sin(rad) / 2, center + step * (toloc)*cos(rad) / 2, center + step * (toloc)*sin(rad) / 2, outlinePen2);

		if (toangle < fromangle) {
			int s = fromangle;
			fromangle = toangle;
			toangle = s;
		}

		if (toangle - fromangle < 180)
		{
			for (double i = fromangle; i < toangle; ++i) {
				rad = i * M_PI / 180.;
				m_scene->addEllipse(center + step * (arccircle)*cos(rad) / 2, center + step * (arccircle)*sin(rad) / 2, 2, 2, outlinePen2, QBrush(Qt::black));
			}
		}
		else
		{
			for (double i = 0; i < fromangle; ++i) {
				rad = i * M_PI / 180.;
				m_scene->addEllipse(center + step * (arccircle)*cos(rad) / 2, center + step * (arccircle)*sin(rad) / 2, 2, 2, outlinePen2, QBrush(Qt::black));
			}
			for (double i = toangle; i < 360; ++i) {
				rad = i * M_PI / 180.;
				m_scene->addEllipse(center + step * (arccircle)*cos(rad) / 2, center + step * (arccircle)*sin(rad) / 2, 2, 2, outlinePen2, QBrush(Qt::black));
			}
		}

	}

}
//--------------------------------------------------------------------------------

void CarRentForm::deleteCar() {


	m_carModel->deleteCar(m_db, ui.nedCarId->text().toInt());
	refreshCar();
}
//--------------------------------------------------------------------------------

void CarRentForm::editCar() {


	m_carModel->edit(m_db, ui.nedCarId->text().toInt(), ui.nedCarName->text(), ui.nedCarBrand->text(), ui.nedCarTail->text(), ui.nedCarClass->text());
	refreshCar();
}
//--------------------------------------------------------------------------------

void CarRentForm::refreshCar() {
	m_carModel->sort(0, Qt::SortOrder::AscendingOrder);
	m_carModel->select();
	m_carModel->show();
}
//--------------------------------------------------------------------------------
void CarRentForm::report() {
	QString filename = QFileDialog::getSaveFileName(this, "Save report file", "", ".txt");
	QFile f(filename);
	if (f.open(QIODevice::WriteOnly))
	{
		QTextStream outStream(&f);
		QSqlQuery query(m_db);

		QString q = QString("SELECT public.cars.id,public.cars.name,public.cars.brand, public.cars.tail_number,public.cars.class,next_service_km, ");
		q += QString(" (SELECT count(*) FROM public.rents WHERE (userid = 1) AND (public.rents.car =  public.cars.id) AND (public.rents.date_from < now())) as ser_done, ");
		q += QString(" (SELECT public.rents.date_from FROM public.rents WHERE (userid = 1) AND (public.rents.car =  public.cars.id) AND (public.rents.date_from >= now())) as ser_to_to, ");
		q += QString(" (SELECT sum(km) FROM public.rents WHERE (public.rents.car =  public.cars.id) AND (public.rents.date_from < now())) as km_done, ");
		q += QString(" (SELECT sum(km) FROM public.rents WHERE (public.rents.car =  public.cars.id) AND (public.rents.date_from >= now())) as km_to_do, ");
		q += QString(" (SELECT sum(price) FROM public.rents WHERE (public.rents.car =  public.cars.id) AND (public.rents.date_from < now())) as p_done, ");
		q += QString(" (SELECT sum(price) FROM public.rents WHERE (public.rents.car =  public.cars.id) AND (public.rents.date_from >= now())) as p_to_do ");
		q += QString(" from %1  ORDER BY id ").arg(m_carModel->CarTableName);
		query.prepare(q);

		if (query.exec()) {

			while (query.next()) {
				QSqlRecord reci = query.record();

				int id = reci.value(0).toInt();
				outStream << "\n\nCAR ID: " << reci.value(0).toString() << endl;
				outStream << "CAR Name: " << reci.value(1).toString() << endl;
				outStream << "\tCAR Brand: " << reci.value(2).toString() << endl;
				outStream << "\tCAR Tail: " << reci.value(3).toString() << endl;

				QString class1 = reci.value(4).toString();
				int pricekm, speed, max_persons, service_km, service_price;
				m_carTypesModel->selectType(m_db, class1, pricekm, speed, max_persons, service_km, service_price);

				outStream << "\tCAR Class: " << class1 << endl;
				outStream << "\tCAR Next Service At KM : " << reci.value(5).toString() << endl;
				outStream << "\t\tCAR Next Service In Minimum Hours : " << (int)((double)reci.value(5).toInt() / (double)speed + 0.5) << endl;
				outStream << "\t\tCAR Service Done : " << reci.value(6).toString() << endl;
				outStream << "\t\tCAR Next Service : " << reci.value(7).toString() << endl;
				outStream << "\tCAR KM done: " << reci.value(8).toString() << endl;
				outStream << "\t\tCAR KM to do: " << reci.value(9).toString() << endl;
				outStream << "\tCAR Money done: " << reci.value(10).toString() << endl;
				outStream << "\t\tCAR Money to do: " << reci.value(11).toString() << endl;

				QSqlQuery query2(m_db);

				QString q2 = QString("SELECT name, fromloc, from_angle, toloc, to_angle, passengers, (EXTRACT(EPOCH FROM ( NOW() - date_from)) * 100.)/ EXTRACT(EPOCH FROM ( date_to - date_from)) as dif FROM public.rents  ");
				q2 += QString(" WHERE (public.rents.car =  %1) AND (public.rents.date_from < now()) AND (public.rents.date_to > now()) ").arg(id);
				query2.prepare(q2);

				if (query2.exec()) {
					if (query2.next()) {
						QSqlRecord reci2 = query2.record();
						outStream << "\tCAR IS IN RENT NOW " << endl;
						outStream << "\t\tTRIP Name: " << reci2.value(0).toString() << endl;
						outStream << "\t\tTRIP Loc From: " << reci2.value(1).toString() << endl;
						outStream << "\t\tTRIP Loc From Angle: " << reci2.value(2).toString() << endl;
						outStream << "\t\tTRIP Loc To: " << reci2.value(3).toString() << endl;
						outStream << "\t\tTRIP Loc To Angle: " << reci2.value(4).toString() << endl;
						outStream << "\t\tTRIP Passengers: " << reci2.value(5).toString() << endl;
						outStream << "\t\tTRIP Loc IS At: " << reci2.value(6).toInt() << "% of entire trip" << endl;
					}
					else {
						outStream << "\tCAR IS FREE NOW " << endl;

					}
				}

				outStream << "\tCAR NEXT PROGRAMMED TRIP: " << endl;
				QSqlQuery query3(m_db);

				q2 = QString("SELECT name, fromloc, from_angle, toloc, to_angle, passengers, date_from, date_to FROM public.rents  ");
				q2 += QString(" WHERE (public.rents.car =  %1) AND (public.rents.date_from > now()) ORDER BY date_from").arg(id);
				query3.prepare(q2);

				if (query3.exec()) {
					while (query3.next()) {
						QSqlRecord reci3 = query3.record();
						outStream << "\n\t\tTRIP Name: " << reci3.value(0).toString() << endl;
						outStream << "\t\tTRIP Date From: " << reci3.value(6).toString() << endl;
						outStream << "\t\tTRIP Date To: " << reci3.value(7).toString() << endl;
						outStream << "\t\tTRIP Loc From: " << reci3.value(1).toString() << endl;
						outStream << "\t\tTRIP Loc From Angle: " << reci3.value(2).toString() << endl;
						outStream << "\t\tTRIP Loc To: " << reci3.value(3).toString() << endl;
						outStream << "\t\tTRIP Loc To Angle: " << reci3.value(4).toString() << endl;
						outStream << "\t\tTRIP Passengers: " << reci3.value(5).toString() << endl;
					}
				}
			}
		}

		f.close();
	}
}
//--------------------------------------------------------------------------------
