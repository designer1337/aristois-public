smallest_pixel_7_11 = surface.create_font("Verdana", 12, 500, 0, 0x010 | 0x080)

local frame_rate = 1.0
function get_fps()
  frame_rate = 0.9 * frame_rate + 0.1 * globals.absolute_frametime();
	 return math.floor(1 / frame_rate);
end

local function get_tickrate()
  if not engine.is_connected() then
	return 0 end

  return math.floor(1.0 / globals.interval_per_tick())
end

local col_text = utilities.color(255, 255, 255, 255)
local col_accent = utilities.color(62, 66, 141, 255)

function watermark()
	local size = surface.screen_size()

	if (engine.is_in_game() and engine.is_connected()) then	
		render.filled_rect(size.x - 250, 10, 155, 30, col_accent)
		render.text(size.x - 145, 20, smallest_pixel_7_11, "rate:", false, col_text)
		render.text(size.x - 120, 20, smallest_pixel_7_11, tostring(get_tickrate()), false, col_text)
	else
		render.filled_rect(size.x - 250, 10, 180, 30, col_accent)
		render.text(size.x - 145, 20, smallest_pixel_7_11, "not connected", false, col_text)
	end

	render.text(size.x - 240, 20, smallest_pixel_7_11, "designer", false, col_text)
	render.text(size.x - 190, 20, smallest_pixel_7_11, "fps:", false, col_text)
	render.text(size.x - 170, 20, smallest_pixel_7_11, tostring(get_fps()), false, col_text)

end


client.set_event_callback("on_paint", function()
	watermark()
end)