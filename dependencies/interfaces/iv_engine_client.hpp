#pragma once
#include <cstdint>
#include "../../source-sdk/math/vectors.hpp"
#include "../../source-sdk/math/view_matrix.hpp"
#include "../../source-sdk/classes/net_channel.hpp"
#include "../../dependencies/utilities/utilities.hpp"

struct player_info_t {
	int64_t __pad0;
	union {
		int64_t xuid;
		struct {
			int xuidlow;
			int xuidhigh;
		};
	};
	char name[128];
	int userid;
	char guid[33];
	unsigned int friendsid;
	char friendsname[128];
	bool fakeplayer;
	bool ishltv;
	unsigned int customfiles[4];
	unsigned char filesdownloaded;
};

class i_net_channel_info {
public:

	enum {
		GENERIC = 0,	// must be first and is default group
		LOCALPLAYER,	// bytes for local player entity update
		OTHERPLAYERS,	// bytes for other players update
		ENTITIES,		// all other entity bytes
		SOUNDS,			// game sounds
		EVENTS,			// event messages
		USERMESSAGES,	// user messages
		ENTMESSAGES,	// entity messages
		VOICE,			// voice data
		STRINGTABLE,	// a stringtable update
		MOVE,			// client move cmds
		STRINGCMD,		// string command
		SIGNON,			// various signondata
		TOTAL,			// must be last and is not a real group
	};

	virtual const char* get_name(void) const = 0;	// get channel name
	virtual const char* get_address(void) const = 0; // get channel IP address as string
	virtual float		get_time(void) const = 0;	// current net time
	virtual float		get_time_connected(void) const = 0;	// get connection time in seconds
	virtual int			get_buffer_size(void) const = 0;	// netchannel packet history size
	virtual int			get_data_rate(void) const = 0; // send data rate in byte/sec

	virtual bool		is_loop_back(void) const = 0;	// true if loopback channel
	virtual bool		is_timing_out(void) const = 0;	// true if timing out
	virtual bool		is_play_back(void) const = 0;	// true if demo playback

	virtual float		get_latency(int flow) const = 0;	 // current latency (RTT), more accurate but jittering
	virtual float		get_average_latency(int flow) const = 0; // average packet latency in seconds
	virtual float		get_average_loss(int flow) const = 0;	 // avg packet loss[0..1]
	virtual float		get_average_choke(int flow) const = 0;	 // avg packet choke[0..1]
	virtual float		get_average_data(int flow) const = 0;	 // data flow in bytes/sec
	virtual float		get_average_packets(int flow) const = 0; // avg packets/sec
	virtual int			get_total_data(int flow) const = 0;	 // total flow in/out in bytes
	virtual int			get_sequence_number(int flow) const = 0;	// last send seq number
	virtual bool		is_valid_packet(int flow, int frame_number) const = 0; // true if packet was not lost/dropped/chocked/flushed
	virtual float		get_packet_time(int flow, int frame_number) const = 0; // time when packet was send
	virtual int			get_packet_bytes(int flow, int frame_number, int group) const = 0; // group size of this packet
	virtual bool		get_stream_progress(int flow, int* received, int* total) const = 0;  // TCP progress if transmitting
	virtual float		get_since_last_time_recieved(void) const = 0;	// get time since last recieved packet in seconds
	virtual	float		get_command_interpolation_ammount(int flow, int frame_number) const = 0;
	virtual void		get_packet_response_latency(int flow, int frame_number, int* pnLatencyMsecs, int* pnChoke) const = 0;
	virtual void		get_remote_framerate(float* pflFrameTime, float* pflFrameTimeStdDeviation) const = 0;

	virtual float		get_timeout_seconds() const = 0;
};

class iv_engine_client {
public:
	i_net_channel_info* get_net_channel_info() {
		using original_fn = i_net_channel_info*(__thiscall*)(iv_engine_client*);
		return (*(original_fn * *)this)[78](this);
	}
	int get_local_player() {
		using original_fn = int(__thiscall*)(iv_engine_client*);
		return (*(original_fn * *)this)[12](this);
	}
	int get_player_for_user_id(int user_id) {
		using original_fn = int(__thiscall*)(iv_engine_client*, int);
		return (*(original_fn * *)this)[9](this, user_id);
	}
	void get_player_info(int index, player_info_t* info) {
		using original_fn = void(__thiscall*)(iv_engine_client*, int, player_info_t*);
		return (*(original_fn * *)this)[8](this, index, info);
	}
	void get_screen_size(int& width, int& height) {
		using original_fn = void(__thiscall*)(iv_engine_client*, int&, int&);
		return (*(original_fn * *)this)[5](this, width, height);
	}
	bool is_in_game() {
		using original_fn = bool(__thiscall*)(iv_engine_client*);
		return (*(original_fn * *)this)[26](this);
	}
	bool is_connected() {
		using original_fn = bool(__thiscall*)(iv_engine_client*);
		return (*(original_fn * *)this)[27](this);
	}
	void execute_cmd(const char* cmd) {
		using original_fn = void(__thiscall*)(iv_engine_client*, const char*);
		return (*(original_fn * *)this)[108](this, cmd); // this always seems to crash whilst debugging, just feel free to continue.
	}
	void set_view_angles(vec3_t& angles) {
		using original_fn = void(__thiscall*)(iv_engine_client*, vec3_t&);
		return (*(original_fn * *)this)[19](this, angles);
	}

	void get_view_angles(vec3_t& angles) {
		using original_fn = void(__thiscall*)(iv_engine_client*, vec3_t&);
		return (*(original_fn**)this)[18](this, angles);
	}

	view_matrix_t& world_to_screen_matrix() {
		view_matrix_t temp;
		using original_fn = view_matrix_t & (__thiscall*)(iv_engine_client*);
		return (*(original_fn * *)this)[37](this);
	}

	bool is_taking_screenshot() {
		using original_fn = bool(__thiscall*)(iv_engine_client*);
		return (*(original_fn * *)this)[92](this);
	}

	const char* get_level_name() {
		using original_fn = const char* (__thiscall*)(iv_engine_client*);
		return (*(original_fn * *)this)[53](this);
	}

	void* get_bsp_query() {
		using original_fn = void* (__thiscall*)(iv_engine_client*);
		return (*(original_fn * *)this)[43](this);
	}
};