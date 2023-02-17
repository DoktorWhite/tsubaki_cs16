#include <tsubaki_skills/tsubaki_common> 
#include <tsubaki_skills/tsubaki_laser> 

#include <tsubaki_skills/tsubaki_menu>

new DEBUG_MENUNAME_ALL_CMD[] = "DebuG_MenU";

#define MENUID_DEBUG_LASER 99900001
#define TOTAL_LASER_PATTERN    21

stock const COMMON_BULLET_MODEL[] = "models/tsubaki/tsubaki_bullet.mdl";

stock LASER_SPRITE_ID;
stock const LASER_SPRITE[] = "sprites/tsubaki/laserbeam.spr";

stock EXPLOSION_SPRITE_ID;
stock EXPLOSION_SPRITE[] = "sprites/zerogxplode.spr";

stock Float:gvOriginOffset[3];
stock normalTrace[MAX_PLAYERS];


public plugin_precache() {
    precache_model(COMMON_BULLET_MODEL);

    LASER_SPRITE_ID = precache_model(LASER_SPRITE);
    EXPLOSION_SPRITE_ID = precache_model(EXPLOSION_SPRITE);
}

public plugin_init() {
    register_menucmd(register_menuid(DEBUG_MENUNAME_ALL_CMD), MENU_KEYS_ALL, "DebugMenuHandler");
    
    register_concmd("set_offset", "ChangeHeightOffset");

    register_concmd("test", "testFx");
    register_concmd("debug_laser", "DebugCreateLaser");
}

public testFx(client, args) {

    new Float:origin[3], Float:dest[3], Float:angles[3];
    GetEntityOrigin(client, origin);
    GetClientEyeAngles(client, angles);
    angles[0] = ToRadian(-1.0*angles[0]);
    angles[1] = ToRadian(angles[1]);

    dest[0] = origin[0] + 1500.0 * Cosine(angles[0]) * Cosine(angles[1]);
    dest[1] = origin[1] + 1500.0 * Cosine(angles[0]) *   Sine(angles[1]);
    dest[2] = origin[2] + 1500.0 *   Sine(angles[0]);
    
    NormalLaser(LASER_SPRITE_ID, .skip_ent=client, .start=origin, .dest=dest, .width=50, .colors={0, 255, 0, 255}, .laser_last_time=7.0);
    NormalPenetrateLaser(LASER_SPRITE_ID, .skip_ent=client, .start=origin, .dest=dest, .width=50, .colors={255, 0, 0, 255}, .laser_last_time=7.0);
    return PLUGIN_HANDLED;
}


public ChangeHeightOffset(client, args) {
    new output[16];
    read_argv(1, output, charsmax(output));

    gvOriginOffset[2] = str_to_float(output);
    client_print(client, print_console, "Offset Has been set to %f", gvOriginOffset[2]);

    return PLUGIN_HANDLED;
}

public DebugMenuHandler(client, key) {
    switch(PLY_MENU_ID(client)) {
        case MENUID_DEBUG_LASER: {
            switch(key) {
                
                case MENU_KEYSTROKE_8: {
                    if(MenuHasPreviousPage(client)) {
                        PLY_MENU_PAGE(client)--;
                        DisplayDebugLaserMenu(client);
                    }
                }   
                case MENU_KEYSTROKE_9: {
                    if(MenuHasNextPage(client, TOTAL_LASER_PATTERN)) {
                        PLY_MENU_PAGE(client)++;
                        DisplayDebugLaserMenu(client);
                    }
                }
                default: {
                    new Float:origin[3], Float:dest[3], Float:angles[3];
                    GetEntityOrigin(client, origin);
                    GetClientEyeAngles(client, angles);
                    angles[0] = ToRadian(-1.0*angles[0]);
                    angles[1] = ToRadian(angles[1]);

                    dest[0] = origin[0] + 1500.0 * Cosine(angles[0]) * Cosine(angles[1]);
                    dest[1] = origin[1] + 1500.0 * Cosine(angles[0]) *   Sine(angles[1]);
                    dest[2] = origin[2] + 1500.0 *   Sine(angles[0]);

                    switch(GetPlayerMenuOptionId(client,key)) {
                        case 0:NormalLaser(LASER_SPRITE_ID, .skip_ent=client, .start=origin, .dest=dest, .width=50, .colors={255, 0, 0, 255}, .laser_last_time=0.1);
                        case 1:NormalPenetrateLaser(LASER_SPRITE_ID, .skip_ent=client, .start=origin, .dest=dest, .width=50, .colors={0, 255, 0, 255}, .laser_last_time=0.1);
                        case 2:PointingLaser(LASER_SPRITE_ID, .start=origin, .dest=dest, .owner=client, .width=50, .colors={255, 255, 0, 255}, .laser_last_time=0.1);
                        case 3:{
                            new Float:starts[8][3], Float:dests[8][3];
                            GetPlayerAimOrigin(client, origin);
                            origin[2] += gvOriginOffset[2];
                            for(new i=0; i<8; i++) {
                                CopyVector(starts[i], origin);
                                dests[i][0] = starts[i][0] + 1500.0 * Cosine(M_2PI*i/8);
                                dests[i][1] = starts[i][1] + 1500.0 * Sine(M_2PI*i/8);
                                dests[i][2] = starts[i][2];

                                MultipleNormalLaser(LASER_SPRITE_ID
                                                    ,.skip_ent=client
                                                    ,.laser_amount=8 
                                                    ,.starts=starts
                                                    ,.dests=dests
                                                    ,.colors={0, 255, 255, 255}, 
                                                    .width=30);
                            }
                        }
                    }

                    DisplayDebugLaserMenu(client);
                }
            }
        }
    }
}

    public DebugCreateLaser(client, args) {
        ResetPlayerMenu(client);
        PLY_MENU_ID(client) = MENUID_DEBUG_LASER;
        DisplayDebugLaserMenu(client);
        
        return PLUGIN_HANDLED;
    }

        DisplayDebugLaserMenu(client) {
            new menu_msg[512];
            new i, item;
            
            formatex(menu_msg, charsmax(menu_msg), DOUBLE_STRING_FMT, "レザー", MENU_TITLE_CONTENT_SPLIT);
            
            for(i=0; i<MAX_MENU_ITEM && (item=GetPlayerMenuOptionId(client,i))<TOTAL_LASER_PATTERN; i++) {
                switch(item) {
                    case 0:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "NormalLaser");
                    case 1:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "NormalPenetrateLaser");
                    case 2:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "PointingLaser");
                    case 3:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "MultipleNormalLaser");
                    //case :format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "");
                }
            }
            
            format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_FMT, menu_msg, MENU_TITLE_CONTENT_SPLIT);
            
            BuildMenuPreviousPage(client, menu_msg);
            BuildMenuNextPage(client, menu_msg, TOTAL_LASER_PATTERN);
            
            show_menu(client, MENU_KEYS_ALL, menu_msg, -1, DEBUG_MENUNAME_ALL_CMD);
        }
