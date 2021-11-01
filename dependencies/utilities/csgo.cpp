#include "csgo.hpp"

player_t* csgo::local_player = nullptr;
bool csgo::send_packet = true;
c_usercmd* csgo::cmd = nullptr;

float csgo::server_time(c_usercmd* cmd) {
    static int tick;
    static c_usercmd* last_cmd;

    if (cmd) {
        if (!last_cmd || last_cmd->hasbeenpredicted)
            tick = csgo::local_player->get_tick_base();
        else
            tick++;
        last_cmd = cmd;
    }
    return tick * interfaces::globals->interval_per_tick;
}