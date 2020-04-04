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

#include "router/manager/router_proxy.h"

#include "base/logging.h"
#include "base/task_runner.h"
#include "router/manager/router.h"

namespace router {

RouterProxy::RouterProxy(std::shared_ptr<base::TaskRunner> io_task_runner,
                         std::unique_ptr<Router> router)
    : io_task_runner_(std::move(io_task_runner)),
      router_(std::move(router))
{
    DCHECK(io_task_runner_ && router_);
}

RouterProxy::~RouterProxy()
{
    disconnectFromRouter();
}

void RouterProxy::connectToRouter(const std::u16string& address, uint16_t port)
{
    if (!io_task_runner_->belongsToCurrentThread())
    {
        io_task_runner_->postTask(
            std::bind(&RouterProxy::connectToRouter, shared_from_this(), address, port));
        return;
    }

    router_->connectToRouter(address, port);
}

void RouterProxy::disconnectFromRouter()
{
    std::unique_lock lock(router_lock_);
    io_task_runner_->deleteSoon(std::move(router_));
}

void RouterProxy::refreshPeerList()
{
    if (!io_task_runner_->belongsToCurrentThread())
    {
        io_task_runner_->postTask(std::bind(&RouterProxy::refreshPeerList, shared_from_this()));
        return;
    }

    std::shared_lock lock(router_lock_);
    if (router_)
        router_->refreshPeerList();
}

void RouterProxy::disconnectPeer(uint64_t peer_id)
{
    if (!io_task_runner_->belongsToCurrentThread())
    {
        io_task_runner_->postTask(
            std::bind(&RouterProxy::disconnectPeer, shared_from_this(), peer_id));
        return;
    }

    std::shared_lock lock(router_lock_);
    if (router_)
        router_->disconnectPeer(peer_id);
}

void RouterProxy::refreshProxyList()
{
    if (!io_task_runner_->belongsToCurrentThread())
    {
        io_task_runner_->postTask(std::bind(&RouterProxy::refreshProxyList, shared_from_this()));
        return;
    }

    std::shared_lock lock(router_lock_);
    if (router_)
        router_->refreshProxyList();
}

void RouterProxy::refreshUserList()
{
    if (!io_task_runner_->belongsToCurrentThread())
    {
        io_task_runner_->postTask(std::bind(&RouterProxy::refreshUserList, shared_from_this()));
        return;
    }

    std::shared_lock lock(router_lock_);
    if (router_)
        router_->refreshUserList();
}

void RouterProxy::addUser(const proto::User& user)
{
    if (!io_task_runner_->belongsToCurrentThread())
    {
        io_task_runner_->postTask(std::bind(&RouterProxy::addUser, shared_from_this(), user));
        return;
    }

    std::shared_lock lock(router_lock_);
    if (router_)
        router_->addUser(user);
}

void RouterProxy::modifyUser(const proto::User& user)
{
    if (!io_task_runner_->belongsToCurrentThread())
    {
        io_task_runner_->postTask(std::bind(&RouterProxy::modifyUser, shared_from_this(), user));
        return;
    }

    std::shared_lock lock(router_lock_);
    if (router_)
        router_->modifyUser(user);
}

void RouterProxy::deleteUser(uint64_t entry_id)
{
    if (!io_task_runner_->belongsToCurrentThread())
    {
        io_task_runner_->postTask(std::bind(&RouterProxy::deleteUser, shared_from_this(), entry_id));
        return;
    }

    std::shared_lock lock(router_lock_);
    if (router_)
        router_->deleteUser(entry_id);
}

} // namespace router
