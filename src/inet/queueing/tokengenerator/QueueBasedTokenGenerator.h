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

#ifndef __INET_QUEUEBASEDTOKENGENERATOR_H
#define __INET_QUEUEBASEDTOKENGENERATOR_H

#include "inet/common/StringFormat.h"
#include "inet/queueing/base/PacketQueueingElementBase.h"
#include "inet/queueing/contract/IPacketQueue.h"
#include "inet/queueing/server/TokenBasedServer.h"

namespace inet {
namespace queueing {

class INET_API QueueBasedTokenGenerator : public PacketQueueingElementBase, public cListener, public StringFormat::IDirectiveResolver
{
  protected:
    int minNumPackets = -1;
    b minTotalLength = b(-1);
    const char *displayStringTextFormat = nullptr;
    IPacketQueue *queue = nullptr;
    TokenBasedServer *server = nullptr;
    cPar *numTokensParameter = nullptr;

    int numTokensGenerated = -1;

  protected:
    virtual void initialize(int stage) override;
    virtual void updateDisplayString();

    virtual void generateTokens();

  public:
    virtual bool supportsPushPacket(cGate *gate) override { return false; }
    virtual bool supportsPopPacket(cGate *gate) override { return false; }

    virtual const char *resolveDirective(char directive) override;

    virtual void receiveSignal(cComponent *source, simsignal_t signal, cObject *object, cObject *details) override;
};

} // namespace queueing
} // namespace inet

#endif // ifndef __INET_QUEUEBASEDTOKENGENERATOR_H

