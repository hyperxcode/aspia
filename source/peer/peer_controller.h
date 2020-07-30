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

#ifndef PEER__PEER_CONTROLLER_H
#define PEER__PEER_CONTROLLER_H

#include "base/macros_magic.h"
#include "base/net/network_channel.h"
#include "peer/peer_id.h"

#include <memory>
#include <string>

namespace peer {

class PeerController : public base::NetworkChannel::Listener
{
public:
    PeerController();
    ~PeerController();

    struct RouterInfo
    {
        std::u16string address;
        uint16_t port;
        base::ByteArray public_key;
        std::u16string username;
        std::u16string password;
    };

    class Delegate
    {
    public:
        virtual ~Delegate() = default;

        virtual void onRouterConnected() = 0;
        virtual void onRouterDisconnected() = 0;
        virtual void onPeerConnected(std::unique_ptr<base::NetworkChannel> channel) = 0;
    };

    void start(const RouterInfo& router_info, Delegate* delegate);
    void connectTo(peer::PeerId peer_id);

protected:
    // NetworkChannel::Listener implementation.
    void onConnected() override;
    void onDisconnected(base::NetworkChannel::ErrorCode error_code) override;
    void onMessageReceived(const base::ByteArray& buffer) override;
    void onMessageWritten(size_t pending) override;

private:
    void connectToRouter();

    RouterInfo router_info_;
    Delegate* delegate_ = nullptr;

    size_t current_router_ = 0;
    std::unique_ptr<base::NetworkChannel> channel_;

    DISALLOW_COPY_AND_ASSIGN(PeerController);
};

} // namespace peer

#endif // PEER__PEER_CONTROLLER_H
