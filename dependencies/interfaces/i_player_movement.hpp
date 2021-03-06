#pragma once
#include "../../source-sdk/math/vectors.hpp"
#include "../../source-sdk/classes/user_cmd.hpp"

class player_move_helper {
public:

	bool bFirstRunOfFunctions : 1;
	bool bGameCodeMovedPlayer : 1;
	int nPlayerHandle; // edict index on server, client entity handle on client=
	int nImpulseCommand; // Impulse command issued.
	vec3_t vecViewAngles; // Command view angles (local space)
	vec3_t vecAbsViewAngles; // Command view angles (world space)
	int nButtons; // Attack buttons.
	int nOldButtons; // From host_client->oldbuttons;
	float flForwardMove;
	float flSideMove;
	float flUpMove;
	float flMaxSpeed;
	float flClientMaxSpeed;
	vec3_t vecVelocity; // edict::velocity // Current movement direction.
	vec3_t vecAngles; // edict::angles
	vec3_t vecOldAngles;
	float outStepHeight; // how much you climbed this move
	vec3_t outWishVel; // This is where you tried
	vec3_t outJumpVel; // This is your jump velocity
	vec3_t vecConstraintCenter;
	float flConstraintRadius;
	float flConstraintWidth;
	float flConstraintSpeedFactor;
	float flUnknown[5];
	vec3_t vecAbsOrigin;

	virtual	void _vpad() = 0;
	virtual void set_host(player_t* host) = 0;
};

class player_move_data {
public:
	bool    bFirstRunOfFunctions : 1;
	bool    bGameCodeMovedPlayer : 1;
	int     nPlayerHandle;        // edict index on server, client entity handle on client=
	int     nImpulseCommand;      // Impulse command issued.
	vec3_t    vecViewAngles;        // Command view angles (local space)
	vec3_t  vecAbsViewAngles;     // Command view angles (world space)
	int     nButtons;             // Attack buttons.
	int     nOldButtons;          // From host_client->oldbuttons;
	float   flForwardMove;
	float   flSideMove;
	float   flUpMove;
	float   flMaxSpeed;
	float   flClientMaxSpeed;
	vec3_t  vecVelocity;          // edict::velocity        // Current movement direction.
	vec3_t  vecAngles;            // edict::angles
	vec3_t  vecOldAngles;
	float   outStepHeight;        // how much you climbed this move
	vec3_t  outWishVel;           // This is where you tried
	vec3_t  outJumpVel;           // This is your jump velocity
	vec3_t  vecConstraintCenter;
	float   flConstraintRadius;
	float   flConstraintWidth;
	float   flConstraintSpeedFactor;
	float   flUnknown[5];
	vec3_t  vecAbsOrigin;
};

class virtual_game_movement {
public:
	virtual			~virtual_game_movement(void) { }

	virtual void	          process_movement(player_t* pPlayer, player_move_data* pMove) = 0;
	virtual void	          Reset(void) = 0;
	virtual void	          start_track_prediction_errors(player_t* pPlayer) = 0;
	virtual void	          finish_track_prediction_errors(player_t* pPlayer) = 0;
	virtual void	          DiffPrint(char const* fmt, ...) = 0;
	virtual vec3_t const& GetPlayerMins(bool ducked) const = 0;
	virtual vec3_t const& GetPlayerMaxs(bool ducked) const = 0;
	virtual vec3_t const& GetPlayerViewOffset(bool ducked) const = 0;
	virtual bool		        IsMovingPlayerStuck(void) const = 0;
	virtual player_t* GetMovingPlayer(void) const = 0;
	virtual void		        UnblockPusher(player_t* pPlayer, player_t* pPusher) = 0;
	virtual void            SetupMovementBounds(player_move_data* pMove) = 0;
};

class player_game_movement
	: public virtual_game_movement {
public:
	virtual ~player_game_movement(void) { }
};

class player_prediction {
public:
	void set_local_view_angles(vec3_t& angles) {
		using original_fn = void(__thiscall*)(void*, vec3_t&);
		return (*(original_fn**)this)[13](this, angles);
	}

	void setup_move(player_t* player, c_usercmd* cmd, player_move_helper* move_helper, void* move_data) {
		using original_fn = void(__thiscall*)(void*, player_t*, c_usercmd*, player_move_helper*, void*);
		return (*(original_fn**)this)[20](this, player, cmd, move_helper, move_data);
	}

	void finish_move(player_t* player, c_usercmd* cmd, void* move_data) {
		using original_fn = void(__thiscall*)(void*, player_t*, c_usercmd*, void*);
		return (*(original_fn**)this)[21](this, player, cmd, move_data);
	}
};