#ifndef _ATTACK_PACKET_HANDLER_H
#define _ATTACK_PACKET_HANDLER_H

#include "..\BasePacketHandler.h"

class AttackRequestHandler : public BasePacketHandler
{
  public:
	AttackRequestHandler(NetworkManager* pManager);
    bool handlePacket(uint8_t* pPacketData);
};

#endif