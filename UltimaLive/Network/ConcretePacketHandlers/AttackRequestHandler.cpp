#include "AttackRequestHandler.h"
#include "..\NetworkManager.h"
#include "MovementRequestHandler.h"

AttackRequestHandler::AttackRequestHandler(NetworkManager* pManager)
  : BasePacketHandler(pManager)
{
  //do nothing
}

bool AttackRequestHandler::handlePacket(uint8_t* pPacketData)
{
	return true;
}