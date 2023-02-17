#include <tsubaki_skills/tsubaki_common> 
#include <tsubaki_skills/tsubaki_bullet> 

#include <tsubaki_skills/tsubaki_menu>

new DEBUG_MENUNAME_ALL_CMD[] = "DebuG_MenU";

#define MENUID_DEBUG_BULLET 99900001
#define TOTAL_BULLET_PATTERN    21

stock const COMMON_BULLET_MODEL[] = "models/tsubaki/tsubaki_bullet.mdl";

stock LASER_SPRITE_ID;
stock const LASER_SPRITE[] = "sprites/tsubaki/laserbeam.spr";

stock EXPLOSION_SPRITE_ID;
stock EXPLOSION_SPRITE[] = "sprites/zerogxplode.spr";

stock Float:gvOriginOffset[3];


public plugin_precache() {
    precache_model(COMMON_BULLET_MODEL);

    LASER_SPRITE_ID = precache_model(LASER_SPRITE);
    EXPLOSION_SPRITE_ID = precache_model(EXPLOSION_SPRITE);
}

public plugin_init() {
    register_menucmd(register_menuid(DEBUG_MENUNAME_ALL_CMD), MENU_KEYS_ALL, "DebugMenuHandler");
    
    register_concmd("set_offset", "ChangeHeightOffset");

    register_concmd("debug_bullet", "DebugCreateBullet");
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
        case MENUID_DEBUG_BULLET: {
            switch(key) {
                
                case MENU_KEYSTROKE_8: {
                    if(MenuHasPreviousPage(client)) {
                        PLY_MENU_PAGE(client)--;
                        DisplayDebugBulletMenu(client);
                    }
                }   
                case MENU_KEYSTROKE_9: {
                    if(MenuHasNextPage(client, TOTAL_BULLET_PATTERN)) {
                        PLY_MENU_PAGE(client)++;
                        DisplayDebugBulletMenu(client);
                    }
                }
                default: {
                    new Float:origin[3];
                    fm_get_aim_origin(client, origin);
                    for(new i=0; i<3; i++) origin[i] += gvOriginOffset[i];

                    switch(GetPlayerMenuOptionId(client,key)) {
                        case 0:AverageBullet(COMMON_BULLET_MODEL, origin, .owner=client);
                        case 1:AverageChangeSpeedBullet(COMMON_BULLET_MODEL, origin, .owner=client);
                        case 2:MoveAndSpinOnlyBullet(COMMON_BULLET_MODEL, origin, .owner=client);
                        case 3:SpinAwayBullet(COMMON_BULLET_MODEL, origin, .owner=client);
                        case 4:SpinAwaySplitBullet(COMMON_BULLET_MODEL, origin, .owner=client);
                        case 5:FullRandomBullet(COMMON_BULLET_MODEL, origin, .owner=client);
                        case 6:AllRandomMachineBullet(COMMON_BULLET_MODEL, origin, .owner=client);
                        case 7:RoundSpinBullet(COMMON_BULLET_MODEL, origin, .owner=client);
                        case 8:SpinBackBullet(COMMON_BULLET_MODEL, origin, .owner=client);
                        case 9:ShapeDrawingBullet(COMMON_BULLET_MODEL, origin, .owner=client);
                        case 10:RotateMachineBullet(COMMON_BULLET_MODEL, origin, .owner=client);
                        case 11:SplitStraightBullet(COMMON_BULLET_MODEL, origin, .owner=client);
                        case 12:AppearAndDisapperBullet(COMMON_BULLET_MODEL, origin, .owner=client);
                        case 13:{
                            new Float:angles[3];
                            GetPlayerViewAngle(client, angles);
                            angles[0] *= -1.0;
                            GetEntityOrigin(client, origin);
                            FireNonPenetrateBullet(COMMON_BULLET_MODEL, origin, angles, .owner=client);
                        }
                        case 14:SplitBackwardBullet(COMMON_BULLET_MODEL, origin, .owner=client);
                        case 15:LaunchSlowChangeDirBullet(COMMON_BULLET_MODEL, origin, .owner=client);
                        case 16:ExplosiveBullet(COMMON_BULLET_MODEL, EXPLOSION_SPRITE_ID, origin, .owner=0);
                        case 17:SwirlBullet(COMMON_BULLET_MODEL, origin, .owner=client);
                        case 18:TracerBullet(COMMON_BULLET_MODEL, origin, .target=client);
                        case 19:SnakeProtect(COMMON_BULLET_MODEL, LASER_SPRITE_ID, origin, .owner=client);
                        //case :(COMMON_BULLET_MODEL, origin, .owner=client);
                    }

                    DisplayDebugBulletMenu(client);
                }
            }
        }
    }
}

    public DebugCreateBullet(client, args) {
        ResetPlayerMenu(client);
        PLY_MENU_ID(client) = MENUID_DEBUG_BULLET;
        DisplayDebugBulletMenu(client);
        
        return PLUGIN_HANDLED;
    }

        DisplayDebugBulletMenu(client) {
            new menu_msg[512];
            new i, item;
            
            formatex(menu_msg, charsmax(menu_msg), DOUBLE_STRING_FMT, "弾幕", MENU_TITLE_CONTENT_SPLIT);
            
            for(i=0; i<MAX_MENU_ITEM && (item=GetPlayerMenuOptionId(client,i))<TOTAL_BULLET_PATTERN; i++) {
                switch(item) {
                    case 0:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "AverageBullet");
                    case 1:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "AverageChangeSpeedBullet");
                    case 2:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "MoveAndSpinOnlyBullet");
                    case 3:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "SpinAwayBullet");
                    case 4:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "SpinAwaySplitBullet");
                    case 5:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "FullRandomBullet");
                    case 6:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "AllRandomMachineBullet");
                    case 7:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "RoundSpinBullet");
                    case 8:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "SpinBackBullet");
                    case 9:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "ShapeDrawingBullet");
                    case 10:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "RotateMachineBullet");
                    case 11:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "SplitStraightBullet");
                    case 12:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "AppearAndDisapperBullet");
                    case 13:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "FireNonPenetrateBullet");
                    case 14:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "SplitBackwardBullet");
                    case 15:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "LaunchSlowChangeDirBullet");
                    case 16:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "ExplosiveBullet");
                    case 17:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "SwirlBullet");
                    case 18:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "TracerBullet");
                    case 19:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "SnakeProtect");
                    //case :format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "");
                }
            }
            
            format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_FMT, menu_msg, MENU_TITLE_CONTENT_SPLIT);
            
            BuildMenuPreviousPage(client, menu_msg);
            BuildMenuNextPage(client, menu_msg, TOTAL_BULLET_PATTERN);
            
            show_menu(client, MENU_KEYS_ALL, menu_msg, -1, DEBUG_MENUNAME_ALL_CMD);
        }
