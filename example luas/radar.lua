
function radar()
	if (engine.is_in_game() and engine.is_connected()) then
		for i=2,64 do
			local player = entity_list.get_entity(i)
			if(player  ~= nil and entity.is_alive(player) and not entity.dormant(player)) then
				entity.spotted(player, 1)
			end
		end
	end
end


client.set_event_callback("on_create_move", function()
	radar()
end)