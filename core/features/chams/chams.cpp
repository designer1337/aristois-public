#include "../features.hpp"

bool found = true;

void chams::initialize() {
	chams::materials::textured = interfaces::material_system->find_material("debug/debugambientcube", nullptr, true, nullptr);
	chams::materials::textured->increment_reference_count();

	chams::materials::flat = interfaces::material_system->find_material("debug/debugdrawflat", nullptr, true, nullptr);
	chams::materials::flat->increment_reference_count();

	utilities::console::log(("[ debug ] [->] materials intialized! \n"));
}

void chams::players::run(i_material_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world) {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player || !info.model)
		return;

	if (!visuals::should_render)
		return;

	auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(info.entity_index));

	const std::string_view model_name = info.model->name;

	if (!entity || !entity->is_alive() || entity->dormant() || entity == csgo::local_player || entity->team() == csgo::local_player->team() || model_name.empty())
		return;

	i_material* first_material_list[]{ materials::textured, materials::flat };
	i_material* first_material_list_hidden[]{ materials::textured, materials::flat };

	if (!first_material_list || !first_material_list_hidden)
		return;

	float clr_chams_health[]{ (255 - entity->health() * 2.55) / 255.f, (entity->health() * 2.55) / 255.f, 0.f, 1.f };

	if (model_name.find("player") != std::string_view::npos && model_name.find("shadow") == std::string_view::npos) {

		if (configuration::settings::aimbot_backtrack_enable && configuration::settings::visuals_backtrack_chams && entity->is_moving() && csgo::local_player->is_alive()) {
			auto record = &lag_compensation::records[info.entity_index];

			for (int t = 0; t < record->size(); t++) {
				if (record && !record->empty() && record->size() && lag_compensation::valid(record->at(t).simulation_time)) {
					interfaces::render_view->modulate_color(configuration::settings::visuals_backtrack_chams_color);
					interfaces::render_view->set_blend(configuration::settings::visuals_backtrack_chams_color[3]);
					chams::materials::flat->set_material_var_flag(material_var_ignorez, true);
					interfaces::model_render->override_material(chams::materials::flat);
					hooks::draw_model_execute::original(interfaces::model_render, ctx, state, info, record->at(t).matrix);
				}
			}

		}

		if (configuration::settings::visuals_chams_hidden_enable) {
			interfaces::render_view->modulate_color(configuration::settings::visuals_chams_health_based ? clr_chams_health : configuration::settings::visuals_chams_color_hidden);
			interfaces::render_view->set_blend(configuration::settings::visuals_chams_health_based ? clr_chams_health[3] : configuration::settings::visuals_chams_color_hidden[3]);
			first_material_list_hidden[configuration::settings::visuals_chams_type_hidden]->set_material_var_flag(material_var_ignorez, true);
			interfaces::model_render->override_material(first_material_list_hidden[configuration::settings::visuals_chams_type_hidden]);
			hooks::draw_model_execute::original(interfaces::model_render, ctx, state, info, bone_to_world);
		}


		if (configuration::settings::visuals_chams_enable) {
			interfaces::render_view->modulate_color(configuration::settings::visuals_chams_health_based ? clr_chams_health : configuration::settings::visuals_chams_color);
			interfaces::render_view->set_blend(configuration::settings::visuals_chams_health_based ? clr_chams_health[3] : configuration::settings::visuals_chams_color[3]);
			first_material_list[configuration::settings::visuals_chams_type]->set_material_var_flag(material_var_ignorez, false);
			interfaces::model_render->override_material(first_material_list[configuration::settings::visuals_chams_type]);
			hooks::draw_model_execute::original(interfaces::model_render, ctx, state, info, bone_to_world);
		}
		interfaces::model_render->override_material(nullptr);
	}

}

void chams::hands::run(i_material_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world) {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	i_material* first_material_list[]{ materials::textured, materials::flat };

	if (!first_material_list)
		return;

	if (!(std::strstr(info.model->name + 17, "arms")))
		return;

	if (configuration::settings::visuals_chams_hands_first_material) {
		interfaces::render_view->modulate_color(configuration::settings::visuals_chams_hands_color);
		interfaces::render_view->set_blend(configuration::settings::visuals_chams_hands_color[3]);
		first_material_list[configuration::settings::visuals_chams_hands_first_material_type]->set_material_var_flag(material_var_ignorez, false);
		interfaces::model_render->override_material(first_material_list[configuration::settings::visuals_chams_hands_first_material_type]);
		hooks::draw_model_execute::original(interfaces::model_render, ctx, state, info, bone_to_world);
	}

	interfaces::model_render->override_material(nullptr);
}