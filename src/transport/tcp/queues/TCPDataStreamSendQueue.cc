//
// Copyright (C) 2004 Andras Varga
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
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//


#include "TCPDataStreamSendQueue.h"

#include "TCPSegmentWithData.h"

Register_Class(TCPDataStreamSendQueue);

TCPDataStreamSendQueue::TCPDataStreamSendQueue() : TCPSendQueue()
{
    begin = end = 0;
}

TCPDataStreamSendQueue::~TCPDataStreamSendQueue()
{
}

void TCPDataStreamSendQueue::init(uint32 startSeq)
{
    begin = startSeq;
    end = startSeq;
    dataBuffer.clear();
}

std::string TCPDataStreamSendQueue::info() const
{
    std::stringstream out;
    out << "[" << begin << ".." << end << "), " << dataBuffer.getLength() << " bytes";
    return out.str();
}

void TCPDataStreamSendQueue::enqueueAppData(cPacket *msg)
{
    //tcpEV << "sendQ: " << info() << " enqueueAppData(bytes=" << msg->getByteLength() << ")\n";
    ByteArrayMessage *bamsg = check_and_cast<ByteArrayMessage *>(msg);
    int64 bytes = bamsg->getByteLength();
    ASSERT (bytes == bamsg->getByteArray().getDataArraySize());
    dataBuffer.push(bamsg->getByteArray());
    end += bytes;
    delete msg;
}

uint32 TCPDataStreamSendQueue::getBufferStartSeq()
{
    return begin;
}

uint32 TCPDataStreamSendQueue::getBufferEndSeq()
{
    return end;
}

TCPSegment *TCPDataStreamSendQueue::createSegmentWithBytes(uint32 fromSeq, ulong numBytes)
{
    //tcpEV << "sendQ: " << info() << " createSeg(seq=" << fromSeq << " len=" << numBytes << ")\n";
    ASSERT(seqLE(begin, fromSeq) && seqLE(fromSeq+numBytes, end));

    TCPSegmentWithBytes *tcpseg = new TCPSegmentWithBytes();
    tcpseg->setSequenceNo(fromSeq);
    tcpseg->setPayloadLength(numBytes);

    // add payload messages whose endSequenceNo is between fromSeq and fromSeq+numBytes
    unsigned int fromOffs = (uint32)(fromSeq - begin);
    char *buffer = new char[numBytes];
    unsigned int bytes = dataBuffer.getBytesToBuffer(buffer, numBytes, fromOffs);
    ASSERT(bytes == numBytes);
    tcpseg->getByteArray().assignBuffer(buffer, bytes);

    // give segment a name
    char msgname[80];
    sprintf(msgname, "tcpseg(l=%lu)", numBytes);
    tcpseg->setName(msgname);

    return tcpseg;
}

void TCPDataStreamSendQueue::discardUpTo(uint32 seqNum)
{
    //tcpEV << "sendQ: " << info() << " discardUpTo(seq=" << seqNum << ")\n";
    ASSERT(seqLE(begin,seqNum) && seqLE(seqNum,end));
    dataBuffer.drop(seqNum - begin);
    begin = seqNum;
}
