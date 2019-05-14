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

#include "inet/common/ModuleAccess.h"
#include "inet/common/queueing/QueueBasedTokenGenerator.h"

namespace inet {
namespace queueing {

Define_Module(QueueBasedTokenGenerator);

void QueueBasedTokenGenerator::initialize(int stage)
{
    if (stage == INITSTAGE_LOCAL) {
        displayStringTextFormat = par("displayStringTextFormat");
        queue = getModuleFromPar<IPacketQueue>(par("queueModule"), this);
        check_and_cast<cSimpleModule *>(queue)->subscribe(packetPoppedSignal, this);
        server = getModuleFromPar<TokenBasedServer>(par("serverModule"), this);
        numTokens = par("numTokens");
        numTokensGenerated = 0;
        WATCH(numTokensGenerated);
    }
}

void QueueBasedTokenGenerator::updateDisplayString()
{
    auto text = StringFormat::formatString(displayStringTextFormat, this);
    getDisplayString().setTagArg("t", 0, text);
}

const char *QueueBasedTokenGenerator::resolveDirective(char directive)
{
    static std::string result;
    switch (directive) {
        case 't':
            result = std::to_string(numTokensGenerated);
            break;
        default:
            throw cRuntimeError("Unknown directive: %c", directive);
    }
    return result.c_str();
}


void QueueBasedTokenGenerator::receiveSignal(cComponent *source, simsignal_t signal, cObject *object, cObject *details)
{
    Enter_Method_Silent();
    if (signal == packetPoppedSignal) {
        server->addTokens(numTokens);
        numTokensGenerated += numTokens;
        updateDisplayString();
    }
    else
        throw cRuntimeError("Unknown signal");
}

} // namespace queueing
} // namespace inet

