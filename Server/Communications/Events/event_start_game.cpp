#include "event_start_game.h"
#include "event.h"
#include "protocol_server.h"
#include "player.h"

EventStartGame::EventStartGame(const std::string background) 
 : background(background) {}

void EventStartGame::process(Player& player, ProtocolServer& protocol) {
	protocol.sendStartGame(this->background);
}
