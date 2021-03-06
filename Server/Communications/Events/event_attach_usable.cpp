#include "event_attach_usable.h"
#include "event.h"
#include "protocol_server.h"
#include "player.h"
#include "worm.h"

EventAttachUsable::EventAttachUsable(std::unique_ptr<Usable> usable) 
 : usable(std::move(usable)) {}

void EventAttachUsable::process(Player& player, ProtocolServer& protocol) {
	protocol.sendUsableId(usable->get_id(), usable->get_ammo());
	player.attach_usable(std::move(usable));
}
