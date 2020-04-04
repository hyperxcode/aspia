//
// Aspia Project
// Copyright (C) 2020 Dmitry Chapyshev <dmitry@aspia.ru>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.
//

#ifndef ROUTER__MANAGER__CONNECT_DIALOG_H
#define ROUTER__MANAGER__CONNECT_DIALOG_H

#include "router/manager/settings.h"
#include "ui_connect_dialog.h"

#include <QDialog>
#include <QPointer>

namespace qt_base {
class LocaleLoader;
} // namespace qt_base

namespace router {

class MainWindow;
class RouterProxy;

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    ConnectDialog();
    ~ConnectDialog();

    QString address() const;
    uint16_t port() const;
    QString userName() const;
    QString password() const;

protected:
    // QDialog implementation.
    void closeEvent(QCloseEvent* event) override;

private:
    void onCurrentLanguageChanged(int index);
    void onCurrentRouterChanged(int index);
    void onButtonBoxClicked(QAbstractButton* button);
    void reloadMru();

    Ui::ConnectDialog ui;
    Settings settings_;
    Settings::MruList mru_;

    QPointer<MainWindow> main_window_;

    DISALLOW_COPY_AND_ASSIGN(ConnectDialog);
};

} // namespace router

#endif // ROUTER__MANAGER__CONNECT_DIALOG_H
