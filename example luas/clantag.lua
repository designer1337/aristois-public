function clantag()
	value = "test"
	utilities.apply_clan_tag(value)
end


client.set_event_callback("on_create_move", function()
	clantag()
end)