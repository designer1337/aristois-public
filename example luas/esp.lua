world = vec3_t.new(); world.x=0; world.y=0; world.z=0;
tahoma = surface.create_font("Visitor TT2 BRK", 9, 500, 0, 128)


--	interfaces::surface->set_font_glyph(render::fonts::name, ("Visitor TT2 BRK"), 8, 400, 0, 0, font_flags::fontflag_outline | font_flags::fontflag_antialias);

function esp()
	if (engine.is_in_game() and engine.is_connected()) then
		for i=2,engine.get_max_clients() do
			player = entity_list.get_entity(i)
			if(player ~= nil and entity.is_alive(player) and not entity.dormant(player)) then
				origin = entity.get_abs_origin(player)

				info = engine.get_player_info(entity.index(player))


				if mathematics.world_to_screen(origin, world) then
						surface.set_text_color(255,0,0,255)
						surface.set_text_font(tahoma)
						surface.set_text_pos(world.x,world.y)
						surface.draw_text(info.name)

						surface.set_text_color(255,0,0,255)
						surface.set_text_font(tahoma)
						surface.set_text_pos(world.x,world.y + 10)
						surface.draw_text(tostring(entity.get_health(player)) .. " HP")
				end
			end
		end
	end
end


client.set_event_callback("on_paint", function()
	esp()
end)