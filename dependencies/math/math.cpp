#include "../utilities/csgo.hpp"

float math::normalize_yaw(float yaw) {
	if (yaw > 180)
		yaw -= (round(yaw / 360) * 360.f);
	else if (yaw < -180)
		yaw += (round(yaw / 360) * -360.f);

	return yaw;
}

const void math::normalize_in_place(vec3_t& angles) {
	for (auto i = 0; i < 3; i++) {
		while (angles[i] < -180.0f)
			angles[i] += 360.0f;

		while (angles[i] > 180.0f)
			angles[i] -= 360.0f;
	}
}

float math::random_float(float min, float max) {
	static auto rnd = reinterpret_cast<float (*) (float, float)> (GetProcAddress(GetModuleHandleW(L"vstdlib.dll"), "RandomFloat"));

	if (rnd)
		return rnd(min, max);
	else
		return 0.f;
}

vec3_t math::cross_product(const vec3_t & a, const vec3_t & b) {
	return vec3_t(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

void math::vector_angles(const vec3_t& forward, vec3_t& up, vec3_t& angles) {
	vec3_t left = cross_product(up, forward);
	left.normalized();

	float forwardDist = forward.Length2D();

	if (forwardDist > 0.001f) {
		angles.x = atan2f(-forward.z, forwardDist) * 180 / M_PI_F;
		angles.y = atan2f(forward.y, forward.x) * 180 / M_PI_F;

		float upZ = (left.y * forward.x) - (left.x * forward.y);
		angles.z = atan2f(left.z, upZ) * 180 / M_PI_F;
	}
	else {
		angles.x = atan2f(-forward.z, forwardDist) * 180 / M_PI_F;
		angles.y = atan2f(-left.x, left.y) * 180 / M_PI_F;
		angles.z = 0;
	}
}

void math::correct_movement(vec3_t old_angles, c_usercmd* cmd, float old_forwardmove, float old_sidemove) {
	float delta_view;
	float f1;
	float f2;

	if (old_angles.y < 0.f)
		f1 = 360.0f + old_angles.y;
	else
		f1 = old_angles.y;

	if (cmd->viewangles.y < 0.0f)
		f2 = 360.0f + cmd->viewangles.y;
	else
		f2 = cmd->viewangles.y;

	if (f2 < f1)
		delta_view = abs(f2 - f1);
	else
		delta_view = 360.0f - abs(f1 - f2);

	delta_view = 360.0f - delta_view;

	cmd->forwardmove = cos(deg_to_rad(delta_view)) * old_forwardmove + cos(deg_to_rad(delta_view + 90.f)) * old_sidemove;
	cmd->sidemove = sin(deg_to_rad(delta_view)) * old_forwardmove + sin(deg_to_rad(delta_view + 90.f)) * old_sidemove;
}

void math::angle_vectors(const vec3_t& angles, vec3_t* forward) {
	float	sp, sy, cp, cy;

	sy = sin(deg_to_rad(angles[1]));
	cy = cos(deg_to_rad(angles[1]));

	sp = sin(deg_to_rad(angles[0]));
	cp = cos(deg_to_rad(angles[0]));

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

vec3_t math::calculate_angle(const vec3_t& source, const vec3_t& destination, const vec3_t& viewAngles) {
	vec3_t delta = source - destination;
	vec3_t angles;

	angles.x = rad_to_deg(atanf(delta.z / std::hypotf(delta.x, delta.y))) - viewAngles.x;
	angles.y = rad_to_deg(atanf(delta.y / delta.x)) - viewAngles.y;
	angles.z = 0.0f;

	if (delta.x >= 0.0)
		angles.y += 180.0f;

	return angles;
}

void math::sin_cos(float r, float* s, float* c) {
	*s = sin(r);
	*c = cos(r);
}

vec3_t math::angle_vector(vec3_t angle) {
	auto sy = sin(angle.y / 180.f * static_cast<float>(M_PI));
	auto cy = cos(angle.y / 180.f * static_cast<float>(M_PI));

	auto sp = sin(angle.x / 180.f * static_cast<float>(M_PI));
	auto cp = cos(angle.x / 180.f * static_cast<float>(M_PI));

	return vec3_t(cp * cy, cp * sy, -sp);
}

void math::transform_vector(vec3_t & a, matrix_t & b, vec3_t & out) {
	out.x = a.dot(b.mat_val[0]) + b.mat_val[0][3];
	out.y = a.dot(b.mat_val[1]) + b.mat_val[1][3];
	out.z = a.dot(b.mat_val[2]) + b.mat_val[2][3];
}

void math::angle_vectors_alternative(vec3_t & angles, vec3_t * forward, vec3_t * right, vec3_t * up) {
	float sp, sy, sr, cp, cy, cr;

	sin_cos(deg_to_rad(angles.x), &sp, &cp);
	sin_cos(deg_to_rad(angles.y), &sy, &cy);
	sin_cos(deg_to_rad(angles.z), &sr, &cr);

	if (forward) {
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right) {
		right->x = -1 * sr * sp * cy + -1 * cr * -sy;
		right->y = -1 * sr * sp * sy + -1 * cr * cy;
		right->z = -1 * sr * cp;
	}

	if (up) {
		up->x = cr * sp * cy + -sr * -sy;
		up->y = cr * sp * sy + -sr * cy;
		up->z = cr * cp;
	}
}

vec3_t math::vector_add(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x + b.x,
		a.y + b.y,
		a.z + b.z);
}

vec3_t math::vector_subtract(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x - b.x,
		a.y - b.y,
		a.z - b.z);
}

vec3_t math::vector_multiply(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x * b.x,
		a.y * b.y,
		a.z * b.z);
}

vec3_t math::vector_divide(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x / b.x,
		a.y / b.y,
		a.z / b.z);
}

bool math::screen_transform(const vec3_t & point, vec3_t & screen) {
	auto matrix = interfaces::engine->world_to_screen_matrix();

	float w = matrix[3][0] * point.x + matrix[3][1] * point.y + matrix[3][2] * point.z + matrix[3][3];
	screen.x = matrix[0][0] * point.x + matrix[0][1] * point.y + matrix[0][2] * point.z + matrix[0][3];
	screen.y = matrix[1][0] * point.x + matrix[1][1] * point.y + matrix[1][2] * point.z + matrix[1][3];
	screen.z = 0.0f;

	int inverse_width = static_cast<int>((w < 0.001f) ? -1.0f / w :
		1.0f / w);

	screen.x *= inverse_width;
	screen.y *= inverse_width;
	return (w < 0.001f);
}

bool math::world_to_screen(const vec3_t & origin, vec3_t & screen) {
	auto matrix = interfaces::engine->world_to_screen_matrix();

	auto find_point = [](vec3_t & point, int screen_w, int screen_h, int degrees) -> void {
		float x2 = screen_w * 0.5f;
		float y2 = screen_h * 0.5f;

		float d = sqrt(pow((point.x - x2), 2) + (pow((point.y - y2), 2))); //Distance
		float r = degrees / d; //Segment ratio

		point.x = r * point.x + (1 - r) * x2; //find point that divides the segment
		point.y = r * point.y + (1 - r) * y2; //into the ratio (1-r):r
	};

	float w = matrix[3][0] * origin.x + matrix[3][1] * origin.y + matrix[3][2] * origin.z + matrix[3][3];

	int screen_width, screen_height;
	interfaces::engine->get_screen_size(screen_width, screen_height);

	float inverse_width = -1.0f / w;
	bool behind = true;

	if (w > 0.01) {
		inverse_width = 1.0f / w;
		behind = false;
	}

	screen.x = (float)((screen_width / 2) + (0.5 * ((matrix[0][0] * origin.x
		+ matrix[0][1] * origin.y
		+ matrix[0][2] * origin.z
		+ matrix[0][3]) * inverse_width) * screen_width + 0.5));

	screen.y = (float)((screen_height / 2) - (0.5 * ((matrix[1][0] * origin.x
		+ matrix[1][1] * origin.y
		+ matrix[1][2] * origin.z
		+ matrix[1][3]) * inverse_width) * screen_height + 0.5));

	if (screen.x > screen_width || screen.x < 0 || screen.y > screen_height || screen.y < 0 || behind) {
		find_point(screen, screen_width, screen_height, screen_height / 2);
		return false;
	}

	return !(behind);
}
