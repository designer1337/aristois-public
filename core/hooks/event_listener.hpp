#pragma once
#include "../../dependencies/utilities/csgo.hpp"

class event_manager : public i_game_event_listener2 {
public:
	void fire_game_event(i_game_event* event);
	bool initialize();
	bool release();
};

extern event_manager event_listener;