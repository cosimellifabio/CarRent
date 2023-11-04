// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QApplication>

#include "windows.h"
#include "CarRentClient.h"

//#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

#define MAX_LOADSTRING 100

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE h0, LPTSTR lpCmdLine, int nCmdShow)
{
    char* argv[] = { "" };
    int argc = 1;
    QApplication app(argc, argv);
    CarRentClientForm calculator;

    calculator.show();
    return app.exec();
}

