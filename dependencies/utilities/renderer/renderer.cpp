#include "renderer.hpp"

void render::initialize() {
	render::fonts::watermark = interfaces::surface->font_create();
	render::fonts::name = interfaces::surface->font_create();
	render::fonts::esp = interfaces::surface->font_create();
	render::fonts::visitor9 = interfaces::surface->font_create();

	interfaces::surface->set_font_glyph(render::fonts::watermark, ("Lucida Console"), 10, 0, 0, 0, font_flags::fontflag_dropshadow);
	interfaces::surface->set_font_glyph(render::fonts::name, ("Small Fonts"), 8, 400, 0, 0, font_flags::fontflag_dropshadow | font_flags::fontflag_antialias);
	interfaces::surface->set_font_glyph(render::fonts::visitor9, ("Verdana"), 12, 500, 0, 0, font_flags::fontflag_antialias | font_flags::fontflag_dropshadow);
	interfaces::surface->set_font_glyph(render::fonts::esp, ("Tahoma"), 12, 700, 0, 0, font_flags::fontflag_dropshadow);

	utilities::console::log(("[ debug ] [->] surface fonts intialized! \n"));
}

void render::cornered_rect(int x, int y, int w, int h, color colour) {
	interfaces::surface->set_drawing_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_line(x, y, x, y + (h / 5));
	interfaces::surface->draw_line(x, y, x + (w / 5), y);
	interfaces::surface->draw_line(x + w, y, x + w - (w / 5), y);
	interfaces::surface->draw_line(x + w, y, x + w, y + (h / 5));
	interfaces::surface->draw_line(x, y + h, x + (w / 5), y + h);
	interfaces::surface->draw_line(x, y + h, x, y + h - (h / 5));
	interfaces::surface->draw_line(x + w, y + h, x + w - (w / 5), y + h);
	interfaces::surface->draw_line(x + w, y + h, x + w, y + h - (h / 5));
}

void render::line(int x1, int y1, int x2, int y2, color colour) {
	interfaces::surface->set_drawing_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_line(x1, y1, x2, y2);
}

void render::text(int x, int y, unsigned long font, std::string string, bool text_centered, color colour) {
	const auto converted_text = std::wstring(string.begin(), string.end());

	int width, height;
	interfaces::surface->get_text_size(font, converted_text.c_str(), width, height);

	interfaces::surface->set_text_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_text_font(font);
	interfaces::surface->draw_text_pos(text_centered ? (x - (width / 2)): x, y);
	interfaces::surface->draw_render_text(converted_text.c_str(), wcslen(converted_text.c_str()));
}

void render::rect(int x, int y, int w, int h, color color) {
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_outlined_rect(x, y, w, h);
}

void render::filled_rect(int x, int y, int w, int h, color colour) {
	interfaces::surface->set_drawing_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_filled_rectangle(x, y, w, h);
}

void render::textured_polygon(int n, vertex_t * vertice, color col) {
	static int texture_id = interfaces::surface->create_new_texture_id(true);
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	interfaces::surface->set_texture_rgba(texture_id, buf, 1, 1);
	interfaces::surface->set_drawing_color(col.r, col.g, col.b, col.a);
	interfaces::surface->set_texture(texture_id);
	interfaces::surface->draw_polygon(n, vertice);
}

void render::circle(int x, int y, int r, int s, color col) {
	float step = M_PI * 2.0 / s;
	for (float a = 0; a < (M_PI * 2.0); a += step) {
		float x1 = r * cos(a) + x;
		float y1 = r * sin(a) + y;
		float x2 = r * cos(a + step) + x;
		float y2 = r * sin(a + step) + y;
		interfaces::surface->set_drawing_color(col.r, col.g, col.b, col.a);
		interfaces::surface->draw_line(x1, y1, x2, y2);
	}
}

vec2_t render::get_text_size(unsigned long font, std::string text) {
	std::wstring a(text.begin(), text.end());
	const wchar_t* wstr = a.c_str();
	static int w, h;

	interfaces::surface->get_text_size(font, wstr, w, h);
	return { static_cast<float>(w), static_cast<float>(h) };
}

