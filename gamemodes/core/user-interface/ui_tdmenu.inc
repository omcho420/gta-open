#include <YSI_Coding\y_hooks>

static
	PlayerText:TextMenuHeader[MAX_PLAYERS],
	PlayerText:TextMenuBody[MAX_PLAYERS],
	TextMenuCallbackHandler[MAX_PLAYERS][32],
	TextMenuItems[MAX_PLAYERS][512],
	TextMenuState[MAX_PLAYERS],
	TextMenuItemCount[MAX_PLAYERS];

hook OnPlayerConnect(playerid) {
	// reset data
	TextMenuState[playerid] = false;
	TextMenuItemCount[playerid] = 0;
	TextMenuCallbackHandler[playerid][0] = EOS;
	TextMenuItems[playerid][0] = EOS;

    TextMenuHeader[playerid] = CreatePlayerTextDraw(playerid, 143.0, 130.0, "Header");
	PlayerTextDrawLetterSize(playerid, TextMenuHeader[playerid], 0.5, 0.5 * 4);
	PlayerTextDrawAlignment(playerid, TextMenuHeader[playerid], 2);
	PlayerTextDrawColor(playerid, TextMenuHeader[playerid], COLOR_BLUE);
	PlayerTextDrawSetOutline(playerid, TextMenuHeader[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, TextMenuHeader[playerid], TD_OUTLINE_COLOUR);
	PlayerTextDrawFont(playerid, TextMenuHeader[playerid], 3);
	PlayerTextDrawSetProportional(playerid, TextMenuHeader[playerid], 1);
	

	TextMenuBody[playerid] = CreatePlayerTextDraw(playerid, 31.0, 130.0, "Content");
	PlayerTextDrawLetterSize(playerid, TextMenuBody[playerid], 0.2, 0.2 * 4);
	PlayerTextDrawTextSize(playerid, TextMenuBody[playerid], 267.0, 0.000000);
	PlayerTextDrawAlignment(playerid, TextMenuBody[playerid], 1);
	PlayerTextDrawColor(playerid, TextMenuBody[playerid], -1);
	PlayerTextDrawUseBox(playerid, TextMenuBody[playerid], 1);
	PlayerTextDrawBoxColor(playerid, TextMenuBody[playerid], 120);
	PlayerTextDrawSetOutline(playerid, TextMenuBody[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, TextMenuBody[playerid], TD_OUTLINE_COLOUR);
	PlayerTextDrawFont(playerid, TextMenuBody[playerid], 1);
	PlayerTextDrawSetProportional(playerid, TextMenuBody[playerid], 1);
	return 1;
}

hook OnPlayerText(playerid, text[]) {
    if(TextMenuState[playerid]) {
        if(TextMenuItemCount[playerid] != 0) {
            if(isnumeric(text)) {
				new value = strval(text);
			    if(value > 0 && value <= TextMenuItemCount[playerid]) {

					if(!isnull(TextMenuCallbackHandler[playerid])) {

						TextMenuState[playerid] = false;
						TextMenuItemCount[playerid] = 0;
						TextMenuItems[playerid][0] = EOS;
						PlayerTextDrawHide(playerid, TextMenuHeader[playerid]);
						PlayerTextDrawHide(playerid, TextMenuBody[playerid]);

						CallLocalFunction(TextMenuCallbackHandler[playerid], "ii", playerid, value);
						return ~0; // stop the hook from caryying on and don't show the input to players.
					} else {
						log("ui_tdmenu.inc - SEVERE! - empty string passed to CallLocalFunction.");
					}
			    }
			}
        }
    }
    return 0;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
	if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT || GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
 	    if(TextMenuState[playerid]) {
	  		if(newkeys == KEY_NO) {
				TextMenu_Hide(playerid);
			}
		}
	}
	return 1;
}

// returns
// - 0 if playerid is invalid.
// - total items.
stock TextMenu_Add(playerid, const item[]) {

	if(!IsPlayerConnected(playerid)) {
		return 0;
	}

	new tempString[128];
	// layout e.g (1. Deagle)
	TextMenuItemCount[playerid] ++;
	format(tempString, sizeof(tempString), "_~r~%i. %s~n~", TextMenuItemCount[playerid], item);
	strcat(TextMenuItems[playerid], tempString, sizeof(TextMenuItems[]));
	return TextMenuItemCount[playerid];
}

// returns
// - 0 if player is invalid.
// - 1 success
stock TextMenu_Create(playerid, const callback[], const title[]) {
	if(!IsPlayerConnected(playerid)) {
		return 0;
	}
	
	// Just to make sure everything is fresh everytime we create 
	// a new  Text Menu.
	TextMenu_Hide(playerid);

	strcpy(TextMenuCallbackHandler[playerid], callback, 32);

	new tempString[35];
	format(tempString, sizeof(tempString), "~b~%s", title);
    PlayerTextDrawSetString(playerid, TextMenuHeader[playerid], tempString);
	return 1;
}

// returns
// - 0 if player is invalid.
// - 1 success
stock TextMenu_Show(playerid)
{
	if(!IsPlayerConnected(playerid)) {
		return 0;
	}

	new
		string[1028];
    format(string, sizeof(string), "~n~~n~~n~%s~n~~n~~w~___________Press_~y~T_~w~Or_~y~F6~w~_and_type_option_number~n~_________________Press_~y~N_~w~to_close_this_box~n~~n~", TextMenuItems[playerid]);
	PlayerTextDrawSetString(playerid, TextMenuBody[playerid], string);
	
	PlayerTextDrawShow(playerid,TextMenuHeader[playerid]);
	PlayerTextDrawShow(playerid,TextMenuBody[playerid]);

	TextMenuState[playerid] = true;
	return 1;
}

// returns
// - 0 if player is invalid or trying to hide a menu when there isn't shown.
// - 1 success
stock TextMenu_Hide(playerid)
{
	if(!IsPlayerConnected(playerid)) {
		return 0;
	}

	if(!TextMenuState[playerid]) {
		return 0; 	
	}

	TextMenuCallbackHandler[playerid][0] = EOS;
	TextMenuState[playerid] = false;
	TextMenuItemCount[playerid] = 0;
	TextMenuItems[playerid][0] = EOS;
	PlayerTextDrawHide(playerid,TextMenuHeader[playerid]);
	PlayerTextDrawHide(playerid,TextMenuBody[playerid]);
	return 1;
}


// returns
// - 0 if player is invalid.
// - 1 success
stock TextMenu_HideSpecific(playerid, const callback[]) {

	if(!strcmp(TextMenuCallbackHandler[playerid], callback)) {
		if(TextMenu_Hide(playerid)) {
			return 1;
		}
	}
	return 0;
}