#pragma once
#include "../../../dependencies/utilities/csgo.hpp"

namespace prediction {
	void start(c_usercmd* cmd);
	void end();
	int get_predicted_flags();

	inline player_move_data data;
	inline float old_cur_time;
	inline float old_frame_time;
	inline int* prediction_random_seed;
	inline int predicted_flags;
}; // namespace prediction