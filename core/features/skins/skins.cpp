#include "../features.hpp"

void skins::knife::run(int stage) {
	if (stage != FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	auto weapon = csgo::local_player->active_weapon();

	if (!weapon)
		return;

	auto knife_index = interfaces::model_info->get_model_index(("models/weapons/v_knife_m9_bay.mdl"));

	auto get_weapons = csgo::local_player->get_weapons();
	for (int i = 0; get_weapons[i] != 0xFFFFFFFF; i++) {
		auto weapons = reinterpret_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity_handle(get_weapons[i]));

		if (!weapons)
			return;

		if (weapon->client_class()->class_id == class_ids::cknife) {
			const auto view_model = reinterpret_cast<base_view_model_t*>(interfaces::entity_list->get_client_entity_handle(csgo::local_player->view_model()));

			if (!view_model)
				return;

			const auto view_model_weapon = reinterpret_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity_handle(view_model->m_hweapon()));

			if (!view_model_weapon)
				return;

			view_model->model_index() = knife_index;

			const auto world_model = reinterpret_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity_handle(view_model_weapon->world_model_handle()));

			if (!world_model)
				return;

			world_model->model_index() = knife_index + 1;
		}

		if (weapons->client_class()->class_id == class_ids::cknife) {
			weapons->item_definition_index() = WEAPON_KNIFE_M9_BAYONET;
			weapons->fallback_paint_kit() = 415;
			weapons->model_index() = knife_index;
			weapons->entity_quality() = 3;
			weapons->fallback_wear() = 0.001f;
		}

		switch (weapons->item_definition_index()) {
		case WEAPON_AWP:
			weapons->fallback_paint_kit() = 344;
			break;
		case WEAPON_AK47:
			weapons->fallback_paint_kit() = 724;
			break;
		case WEAPON_SSG08:
			weapons->fallback_paint_kit() = 222;
			break;
		case WEAPON_M4A1:
			weapons->fallback_paint_kit() = 309;
			break;
		case WEAPON_M4A1_SILENCER:
			weapons->fallback_paint_kit() = 445;
			break;
		case WEAPON_USP_SILENCER:
			weapons->fallback_paint_kit() = 504;
			break;
		case WEAPON_GLOCK:
			weapons->fallback_paint_kit() = 353;
			break;
		case WEAPON_DEAGLE:
			weapons->fallback_paint_kit() = 711;
			break;
		case WEAPON_HKP2000:
			weapons->fallback_paint_kit() = 211;
			break;
		}

		weapons->original_owner_xuid_low() = 0;
		weapons->original_owner_xuid_high() = 0;
		weapons->fallback_seed() = 403;
		weapons->item_id_high() = -1;
	}
}

void skins::agents::run(int stage) {
	if (stage != FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		return;

	if (!configuration::settings::misc_agent_changer_enable)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive() || csgo::local_player->client_class()->class_id != class_ids::ccsplayer) //is_player
		return;

	static auto game_type = interfaces::console->get_convar("game_type");

	if (game_type->get_int() == 6) //dangerzone
		return;

	const char* models_to_change_ct[] = { 
		"models/player/custom_player/legacy/ctm_st6_varianti.mdl",
		"models/player/custom_player/legacy/ctm_st6_variantm.mdl",
		"models/player/custom_player/legacy/ctm_st6_variantg.mdl", 
		"models/player/custom_player/legacy/ctm_st6_variante.mdl", 
		"models/player/custom_player/legacy/ctm_st6_variantk.mdl", 
		"models/player/custom_player/legacy/ctm_fbi_varianth.mdl", 
		"models/player/custom_player/legacy/ctm_fbi_variantg.mdl", 
		"models/player/custom_player/legacy/ctm_fbi_variantf.mdl", 
		"models/player/custom_player/legacy/ctm_fbi_variantb.mdl", 
		"models/player/custom_player/legacy/ctm_sas_variantf.mdl" 
	};

	const char* models_to_change_t[] = { 
		"models/player/custom_player/legacy/tm_balkan_variantj.mdl", 
		"models/player/custom_player/legacy/tm_balkan_variantg.mdl", 
		"models/player/custom_player/legacy/tm_balkan_varianti.mdl", 
		"models/player/custom_player/legacy/tm_balkan_variantf.mdl",
		"models/player/custom_player/legacy/tm_balkan_varianth.mdl", 
		"models/player/custom_player/legacy/tm_phoenix_variantg.mdl", 
		"models/player/custom_player/legacy/tm_phoenix_variantf.mdl",
		"models/player/custom_player/legacy/tm_phoenix_varianth.mdl", 
		"models/player/custom_player/legacy/tm_leet_variantf.mdl", 
		"models/player/custom_player/legacy/tm_leet_varianti.mdl", 
		"models/player/custom_player/legacy/tm_leet_varianth.mdl", 
		"models/player/custom_player/legacy/tm_leet_variantg.mdl" 
	};

	auto model_index_ct = interfaces::model_info->get_model_index(models_to_change_ct[configuration::settings::misc_agent_ct]);

	auto model_index_t = interfaces::model_info->get_model_index(models_to_change_t[configuration::settings::misc_agent_t]);

	if (!model_index_ct || !model_index_t)
		return;

	if (csgo::local_player->team() == 2)
		csgo::local_player->set_model_index(model_index_t);

	else if (csgo::local_player->team() == 3)
		csgo::local_player->set_model_index(model_index_ct);

}