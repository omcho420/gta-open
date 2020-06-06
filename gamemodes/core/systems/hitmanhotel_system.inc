#include <YSI_Coding\y_hooks>
#include <YSI_Coding\y_timers>

static 
    hexit,
    bool:PlayerIsWanted[MAX_PLAYERS],
    Timer:ClearWantedLevel[MAX_PLAYERS]
;

hook OnGameModeInit()
{
    Create3DTextLabel("Press "C_YELLOW"Y"C_WHITE" to view services", COLOR_WHITE, 2217.1999511719, -1146.5, 1026.8000488281, 40.0, 27, 0);
    hexit = CreateDynamicCP(-794.8065, 489.6315, 1376.1953, 1.5, 1, .streamdistance = CHECKPOINT_STREAMDISTANCE);
    return 1;
}

hook OnPlayerEnterDynamicCP(playerid, checkpointid)
{
    if(checkpointid == hexit) 
    {
        if(PlayerIsWanted[playerid] == true) {
            stop ClearWantedLevel[playerid];
            UI_HideTimeLeft(playerid);
        }
        SetPlayerPos(playerid, 2239.1824, -1192.7251, 1033.7969);
        SetPlayerFacingAngle(playerid, 269.8460);
        SetPlayerInterior(playerid, 15);
        SetPlayerVirtualWorld(playerid, 27);
    }
    return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(PRESSED(KEY_YES))
    {
        if(IsPlayerInRangeOfPoint(playerid, 5.0, 2217.1999511719, -1146.5, 1026.8000488281))
        {
            ShowHotelMenu(playerid);
        }
    }
    return 1;
}

hook OnPlayerUpdate(playerid)
{
    if(!IsPlayerInRangeOfPoint(playerid, 5.0, 2217.1999511719, -1146.5, 1026.8000488281))
    {
        TextMenu_HideSpecific(playerid, "TextMenuHotel");
    }
    return 1;
}

forward TextMenuHotel(playerid, listitem);
public TextMenuHotel(playerid, listitem)
{
    switch(listitem)
    {
        case 1:
        {
            if(Player_GetJob(playerid) != TYPE_HITMAN) {
                return SendErrorMsg(playerid, "You can't enter Hitman Hangout you're not a Hitman");
            }
            if(GetPlayerMoney(playerid) < 60000 && GetPlayerWantedLevel(playerid) != 0) {
                return SendErrorMsg(playerid, "You don't have enough money!");
            }
            if(GetPlayerWantedLevel(playerid) != 0) {
                SendServerMsg(playerid, "2 mins to erase your wanted level.");
                UI_TimeLeft(playerid, 120);
                PlayerIsWanted[playerid] = true;
                ClearWantedLevel[playerid] = defer ClearPlayerWanted(playerid);
                GivePlayerMoney(playerid, -60000);
            }
            SetPlayerPos(playerid, -794.806396, 497.738037, 1376.195312);
            SetPlayerInterior(playerid, 1);
            SendServerMsg(playerid, "Hitman's Hangout!");
            SetPlayerVirtualWorld(playerid, 1);
        }
        case 2:
        {
            if(GetPlayerMoney(playerid) < 1500) {
                return SendErrorMsg(playerid, "You don't have enough money!");
            }
            GivePlayerMoney(playerid, -1500);
            SetPlayerHealth(playerid, 100);
            SetPlayerArmour(playerid, 100);
        }
    }
    return 1;
}

stock ShowHotelMenu(playerid)
{
    TextMenu_Create(playerid, "TextMenuHotel", "Hitman Hotel");
    TextMenu_Add(playerid, "~w~Hitman's Party pass [~g~$60,000~w~]");
    TextMenu_Add(playerid, "~w~Hitman Heal [~g~$1,500~w~]");
    TextMenu_Show(playerid);
    return 1;
}

timer ClearPlayerWanted[120000](playerid) {
    UI_HideTimeLeft(playerid);
    PLAYER_SetPlayerWantedLevel(playerid, 0);
    PlayerIsWanted[playerid] = false;
    return 1;
}