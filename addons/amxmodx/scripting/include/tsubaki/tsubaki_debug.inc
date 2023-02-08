#if defined _tsubaki_debug
    #endinput
#endif
#define _tsubaki_debug

#include <tsubaki/tsubaki_method_lib>

#pragma semicolon 1

#if defined DEBUG

    public TestFx(client) {
    }

    public TestFxServer(client) {
    }

    new DEBUG_MENUNAME_ALL_CMD[] = "DebuG_MenU";

    #define MENUID_DEBUG_WEAPON 99900001
    #define MENUID_DEBUG_ENTITY 99900003
    #define MENUID_DEBUG_BULLET 99900005
    #define MENUID_DEBUG_CAMERA 99900007

    RegisterTBKDebugMenuAndCommand() {
        register_menucmd(register_menuid(DEBUG_MENUNAME_ALL_CMD), MENU_KEYS_ALL, "DebugMenuHandler");

        register_concmd("test", "TestFx");
        register_srvcmd("stest", "TestFxServer");
        
        register_concmd("debug_weapon", "DebugWeaponMenu");
        register_concmd("debug_entity", "DebugAimEntityDetails");
        register_concmd("debug_bullet", "DebugCreateBullet");
        register_concmd("debug_camera", "DebugCamera");
    }

    public DebugMenuHandler(client, key) {
        //client_print(client, print_chat, "Selected %d", key);

        if(!IsPlayerDeveloper(client) || key==MENU_KEYSTROKE_0) 
            return;

        switch(PLY_MENU_ID(client)) {
            case MENUID_DEBUG_WEAPON: {
                switch(key) {
                    case MENU_KEYSTROKE_8: {
                        if(MenuHasPreviousPage(client)) {
                            PLY_MENU_PAGE(client)--;
                            DisplayDebugWeaponMenu(client);
                        }
                    }   
                    case MENU_KEYSTROKE_9: {
                        if(MenuHasNextPage(client, TOTAL_WEAPON)) {
                            PLY_MENU_PAGE(client)++;
                            DisplayDebugWeaponMenu(client);
                        }
                    }
                    default:{
                        new id = GetPlayerMenuOptionId(client, key);
                        if(id <= 0 || id >= TOTAL_WEAPON || equal(WEAPON_LIST[id], EMPTY_ITEM_NAME))
                            return;
                        
                        new weapon_name[32];
                        formatex(weapon_name, charsmax(weapon_name), DOUBLE_STRING_FMT, WEAPON_HEADER, WEAPON_LIST[id]);
                        give_item(client, weapon_name);
                    }
                }
            }
            case MENUID_DEBUG_ENTITY: {
                switch(key) {
                    case MENU_KEYSTROKE_3: {            //Remove Entity
                        if(pev_valid(PLY_MENU_TARGET(client))) {
                            new clsname[32];
                            entity_get_string(PLY_MENU_TARGET(client), EV_SZ_classname, clsname, 32);
                            remove_entity(PLY_MENU_TARGET(client));
                            client_print(client, print_chat, "Entity %s(%d) Removed", clsname, PLY_MENU_TARGET(client));
                        }
                    }
                }
            }
            case MENUID_DEBUG_CAMERA: {
                if(0<=key<=CAMERA_TOPDOWN)
                    set_view(client, key);
            }
            case MENUID_DEBUG_BULLET: {
                switch(key) {
                    case MENU_KEYSTROKE_8: {
                        if(MenuHasPreviousPage(client)) {
                            PLY_MENU_PAGE(client)--;
                            DisplayDebugBulletMenu(client);
                        }
                    }   
                    case MENU_KEYSTROKE_9: {
                        if(MenuHasNextPage(client, TOTAL_WEAPON)) {
                            PLY_MENU_PAGE(client)++;
                            DisplayDebugBulletMenu(client);
                        }
                    }
                    default:{
                        new Float:origin[3];
                        GetEntityOrigin(client, origin);
                        switch(GetPlayerMenuOptionId(client,key)) {
                            case 0: {
                                new ent;

                                ent = CreateBullet(NORMAL_BULLET, COMMON_BULLET_MODEL);
                                fm_get_aim_origin(client, origin);
                                TeleportEntity(ent, origin);
                            }
                            case  1:AverageBullet(origin, .owner=client);
                            case  2:AverageChangeSpeedBullet(.origin=origin, .owner=client);
                            case  3:MoveAndSpinOnlyBullet(.origin=origin, .owner=client);
                            case  4:SpinAwayBullet(.origin=origin, .owner=client);
                            case  5:CreateSpinAwaySplitBullet(.origin=origin, .owner=client);
                            case  6:FireFullRandomBullet(.origin=origin, .owner=client);
                            case  7:AllRandomMachineBullet(.origin=origin, .owner=client);
                            case  8:RoundSpinBullet(.origin=origin, .owner=client);
                            case  9:ChangeDestBullet(.origin=origin, .dest_origin=origin, .owner=client, .angle=GetRandomAngleDegree());
                            case 10:ChangeAngleBullet(.origin=origin, .owner=client);
                            case 11:SpinBackBullet(.origin=origin, .owner=client);
                            case 12:ShapeDrawingBullet(.origin=origin, .owner=client);
                            case 13:RotateMachineBullet(.origin=origin, .owner=client);
                            case 14:SplitStraightBullet(.origin=origin, .owner=client);
                            case 15:{ 
                                fm_get_aim_origin(client, origin);
                                AppearAndDisapperBullet(.origin=origin, .owner=0);
                            }
                            case 16:FirePointingBullet(.origin=origin, .owner=client);
                            case 17:SplitBackwardBullet(.origin=origin, .owner=client);
                            case 18:LaunchSlowChangeDirBullet(.origin=origin, .owner=client);
                            case 19:{
                                fm_get_aim_origin(client, origin);
                                ExplosiveBullet(.origin=origin, .owner=0);
                            }
                            case 20:SwirlBullet(.origin=origin, .owner=client);
                            case 21:TracerBullet(.origin=origin, .owner=client);
                            case 22:SnakeProtect(.origin=origin, .owner=client);
                            //case :(.origin=origin, .owner=client);
                        }

                        DisplayDebugBulletMenu(client);
                    }
                }
            }
        }

        return;
    }

    public DebugWeaponMenu(client) {
        if(!IsPlayerDeveloper(client)) 
            return PLUGIN_CONTINUE;

        ResetPlayerMenu(client);
        PLY_MENU_ID(client) = MENUID_DEBUG_WEAPON;

        DisplayDebugWeaponMenu(client);

        return PLUGIN_HANDLED;
    }

        DisplayDebugWeaponMenu(client) {
            new menu_msg[512];
            new i, item;
            
            for(i=0; i<MAX_MENU_ITEM && (item=GetPlayerMenuOptionId(client,i))<TOTAL_WEAPON; i++) {
                format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, WEAPON_LIST[item]);
            }
            
            if(MenuHasPreviousPage(client)) { 
                format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, 8, MENU_PREVIOUS_PAGE);
            }

            if(MenuHasNextPage(client, TOTAL_WEAPON)) { 
                format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, 9, MENU_NEXT_PAGE);
            }

            show_menu(client, MENU_KEYS_ALL, menu_msg, -1, DEBUG_MENUNAME_ALL_CMD);
        }

    public DebugAimEntityDetails(client) {
    
        new body, ent;
        get_user_aiming(client, ent, body);

        if(!pev_valid(ent)) {
            client_print(client, print_chat, "No target found");
        } else {
            PLY_MENU_ID(client) = MENUID_DEBUG_ENTITY;
            PLY_MENU_TARGET(client) = ent;

            new menu_msg[512], clsname[32];

            entity_get_string(ent, EV_SZ_classname, clsname, 32);

            formatex(menu_msg, charsmax(menu_msg), DOUBLE_STRING_FMT, "対象エンティティ", MENU_TITLE_CONTENT_SPLIT);
            format(menu_msg, charsmax(menu_msg), "%sID:%d^nClassname:%s^n", menu_msg, ent, clsname);
            format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, 3, "削除");

            show_menu(client, MENU_KEYS_ALL, menu_msg, -1, DEBUG_MENUNAME_ALL_CMD);
        }
        
        return PLUGIN_HANDLED;
    }


    public DebugCamera(client) {
        PLY_MENU_ID(client) = MENUID_DEBUG_CAMERA;

        new menu_msg[512];

        formatex(menu_msg, charsmax(menu_msg), DOUBLE_STRING_FMT, "カメラ", MENU_TITLE_CONTENT_SPLIT);

        format(menu_msg, charsmax(menu_msg), "%s \
            1.1st^n \
            2.3rd^n \
            3.upleft^n \
            4.TopDown^n \
            "
            , menu_msg);
        
        show_menu(client, MENU_KEYS_ALL, menu_msg, -1, DEBUG_MENUNAME_ALL_CMD);

        return PLUGIN_HANDLED;
    }


    public DebugCreateBullet(client) {
        if(!IsPlayerDeveloper(client)) 
            return PLUGIN_CONTINUE;

        ResetPlayerMenu(client);
        PLY_MENU_ID(client) = MENUID_DEBUG_BULLET;
        DisplayDebugBulletMenu(client);

        return PLUGIN_HANDLED;
    }

        DisplayDebugBulletMenu(client) {
            new menu_msg[512];
            new i, item;
            
            formatex(menu_msg, charsmax(menu_msg), DOUBLE_STRING_FMT, "弾幕", MENU_TITLE_CONTENT_SPLIT);
            
            for(i=0; i<MAX_MENU_ITEM && (item=GetPlayerMenuOptionId(client,i))<TOTAL_WEAPON; i++) {
                switch(item) {
                    case 0:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "Create Bullet Model");
                    case 1:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "AverageBullet");
                    case 2:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "AverageChangeSpeedBullet");
                    case 3:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "MoveAndSpinOnlyBullet");
                    case 4:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "SpinAwayBullet");
                    case 5:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "CreateSpinAwaySplitBullet");
                    case 6:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "FireFullRandomBullet");
                    case 7:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "AllRandomMachineBullet");
                    case 8:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "RoundSpinBullet");
                    case 9:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "ChangeDestBullet");
                    case 10:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "ChangeAngleBullet");
                    case 11:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "SpinBackBullet");
                    case 12:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "ShapeDrawingBullet");
                    case 13:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "RotateMachineBullet");
                    case 14:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "SplitStraightBullet");
                    case 15:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "AppearAndDisapperBullet");
                    case 16:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "FirePointingBullet");
                    case 17:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "SplitBackwardBullet");
                    case 18:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "LaunchSlowChangeDirBullet");
                    case 19:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "ExplosiveBullet");
                    case 20:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "SwirlBullet");
                    case 21:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "TracerBullet");
                    case 22:format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "SnakeProtect");
                    //case :format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, i+1, "");
                }
            }
            
            format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_FMT, menu_msg, MENU_TITLE_CONTENT_SPLIT);
            
            if(MenuHasPreviousPage(client)) { 
                format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, 8, MENU_PREVIOUS_PAGE);
            }

            if(MenuHasNextPage(client, 25)) { 
                format(menu_msg, charsmax(menu_msg), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, 9, MENU_NEXT_PAGE);
            }

            show_menu(client, MENU_KEYS_ALL, menu_msg, -1, DEBUG_MENUNAME_ALL_CMD);
        }

#else

    RegisterTBKDebugMenuAndCommand() {

    }

#endif