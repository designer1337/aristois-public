#include "..//features.hpp"

void lag_compensation::update(int stage) {
	if (!configuration::settings::aimbot_backtrack_enable || !csgo::local_player || !csgo::local_player->is_alive()) {
		for (auto& record : records)
			record.clear();

		return;
	}

	if (stage == FRAME_RENDER_START) {
		for (int i = 1; i <= interfaces::globals->max_clients; i++) {
			auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
			if (!entity || entity == csgo::local_player || entity->dormant() || !entity->is_alive() || entity->team() == csgo::local_player->team()) {
				records[i].clear();
				continue;
			}

			if (!records[i].empty() && (records[i].front().simulation_time == entity->simulation_time()))
				continue;

			auto nci = interfaces::engine->get_net_channel_info();

			if (!nci)
				continue;

			stored_records record{ };
			record.hitbox = entity->get_hitbox_position(hitbox_head);
			record.origin = entity->abs_origin();
			record.simulation_time = entity->simulation_time();

			entity->setup_bones(record.matrix, 256, 0x7FF00, interfaces::globals->cur_time);

			records[i].push_front(record);
			
			while (configuration::settings::aimbot_backtrack_ping_spike ? (records[i].size() > 3 && records[i].size() > static_cast<size_t>(time_to_ticks(0.2f) + nci->get_latency(0))) : (records[i].size() > 3 && records[i].size() > static_cast<size_t>(time_to_ticks(0.2f))))
				records[i].pop_back();

			if (auto invalid = std::find_if(std::cbegin(records[i]), std::cend(records[i]), [](const stored_records& rec) { return !valid(rec.simulation_time); }); invalid != std::cend(records[i]))
				records[i].erase(invalid, std::cend(records[i]));
		}
	}
}

void lag_compensation::run(c_usercmd* cmd) {
	if (!configuration::settings::aimbot_backtrack_enable)
		return;

	if (!(cmd->buttons & in_attack))
		return;

	if (!csgo::local_player)
		return;

	auto local_eye_pos = csgo::local_player->get_eye_pos();

	auto best_fov = 255.f;
	player_t* best_target{ };
	int best_target_index{ };
	vec3_t best_target_origin{ };
	int best_record{ };

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || entity == csgo::local_player || entity->dormant() || !entity->is_alive()
			|| entity->team() == csgo::local_player->team())
			continue;

		auto origin = entity->abs_origin();
		auto angle = math::calculate_angle(local_eye_pos, origin, cmd->viewangles);
		auto fov = std::hypotf(angle.x, angle.y);
		if (fov < best_fov) {
			best_fov = fov;
			best_target = entity;
			best_target_index = i;
			best_target_origin = origin;
		}
	}

	if (best_target) {
		if (records[best_target_index].size() <= 3)
			return;

		best_fov = 255.f;

		for (size_t i = 0; i < records[best_target_index].size(); i++) {
			auto& record = records[best_target_index][i];
			if (!valid(record.simulation_time))
				continue;

			auto angle = math::calculate_angle(local_eye_pos, record.origin, cmd->viewangles);
			auto fov = std::hypotf(angle.x, angle.y);
			if (fov < best_fov) {
				best_fov = fov;
				best_record = i;
			}
		}
	}

	if (best_record) {
		auto record = records[best_target_index][best_record];
		best_target->set_abs_origin(record.origin);
		cmd->tick_count = time_to_ticks(record.simulation_time + lerp_time());
	}
}

void lag_compensation::update_incoming_sequences(i_net_channel* net_channel) {
	if (net_channel == nullptr)
		return;

	if (last_incoming_sequence == 0)
		last_incoming_sequence = net_channel->in_sequence_nr;

	if (net_channel->in_sequence_nr > last_incoming_sequence) {
		last_incoming_sequence = net_channel->in_sequence_nr;
		sequences_vector.emplace_front(sequence_object_t(net_channel->in_reliable_state, net_channel->out_reliable_state, net_channel->in_sequence_nr, interfaces::globals->realtime));
	}

	if (sequences_vector.size() > 2048U)
		sequences_vector.pop_back();
}

void lag_compensation::clear_incoming_sequences() {
	if (!sequences_vector.empty()) {
		last_incoming_sequence = 0;
		sequences_vector.clear();
	}
}

void lag_compensation::add_latency_to_net_channel(i_net_channel* net_channel, float latency) {
	for (auto& sequence : sequences_vector) {
		if (interfaces::globals->realtime - sequence.flCurrentTime >= latency) {
			net_channel->in_reliable_state = sequence.iInReliableState;
			net_channel->in_sequence_nr = sequence.iSequenceNr;
			break;
		}
	}
}