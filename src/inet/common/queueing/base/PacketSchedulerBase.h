//
// Copyright (C) OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see http://www.gnu.org/licenses/.
//

#ifndef __INET_PACKETSCHEDULERBASE_H
#define __INET_PACKETSCHEDULERBASE_H

#include "inet/common/queueing/base/PacketProcessorBase.h"
#include "inet/common/queueing/contract/IPacketCollector.h"
#include "inet/common/queueing/contract/IPacketProvider.h"

namespace inet {
namespace queueing {

class INET_API PacketSchedulerBase : public PacketProcessorBase, public IPacketCollector, public IPacketProvider
{
  protected:
    std::vector<cGate *> inputGates;
    std::vector<IPacketProvider *> providers;

    cGate *outputGate = nullptr;
    IPacketCollector *collector = nullptr;

  protected:
    virtual void initialize(int stage) override;

    virtual int schedulePacket() = 0;

  public:
    virtual IPacketProvider *getProvider(cGate *gate) override { return providers[gate->getIndex()]; }

    virtual bool supportsPushPacket(cGate *gate) override { return false; }
    virtual bool supportsPopPacket(cGate *gate) override { return true; }

    virtual bool canPopSomePacket(cGate *gate) override;
    virtual Packet *canPopPacket(cGate *gate) override { throw cRuntimeError("Invalid operation"); }
    virtual Packet *popPacket(cGate *gate) override;

    virtual void handleCanPopPacket(cGate *gate) override;
};

} // namespace queueing
} // namespace inet

#endif // ifndef __INET_PACKETSCHEDULERBASE_H

