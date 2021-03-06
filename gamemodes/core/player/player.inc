#include <player_spawn>
#include <player_spawn-protection>
#include <player_class>
#include <player_job>
#include <player_location>
#include <player_items>

#include <attachment>
#include <job>

#include <YSI_Coding\y_hooks>
#include <YSI_Data\y_bit>

new 
    BitArray:PlayerSpawnState<MAX_PLAYERS>;

hook OnPlayerSpawn(playerid) {
    Bit_Set(PlayerSpawnState, playerid, true);
    return 1;
}

hook OnPlayerDeathEx(playerid, killerid, reason, Float: damage, bodypart) {
    Bit_Set(PlayerSpawnState, playerid, false);
    return 1;
}

stock Player_IsSpawned(playerid) {
    return Bit_Get(PlayerSpawnState, playerid);
}

// updates player colour depending on wanted level.
// innocent = white
// < 4 = yellow
// 4 .. 12 = orange
// > 12 = red.
stock Player_UpdateColour(playerid) {
    new
        playerWantedLevel = GetPlayerWantedLevel(playerid);
    if (playerWantedLevel == 0) {
        SetPlayerColor(playerid, COLOR_WHITE);
    }
    else if (0 < playerWantedLevel < 4) {
        SetPlayerColor(playerid, COLOR_YELLOW);
    }
    else if (4 <= playerWantedLevel <= 12) {
        SetPlayerColor(playerid, COLOR_ORANGE);
    }
    else if (12 < playerWantedLevel) {
        SetPlayerColor(playerid, COLOR_RED);
    }
    return 1;
}

static ptask _PlayerPerSecondTimer[1000](playerid) {
    if(Player_IsSpawned(playerid)) {
        CallLocalFunction("OnPlayerSecondUpdate", "i", playerid);
    }
}