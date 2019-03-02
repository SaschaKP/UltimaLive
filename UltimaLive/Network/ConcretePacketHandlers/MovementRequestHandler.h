#ifndef _MOVEMENT_PACKET_HANDLER_H
#define _MOVEMENT_PACKET_HANDLER_H

#include <time.h>
#include "..\BasePacketHandler.h"

class MovementRequestHandler : public BasePacketHandler
{
  public:
    MovementRequestHandler(NetworkManager* pManager);
    bool handlePacket(uint8_t* pPacketData);
};

#endif