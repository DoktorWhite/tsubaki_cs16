#if defined _tsubaki_method_lib
    #endinput
#endif
#define _tsubaki_method_lib

#include <amxmodx>
#include <amxmisc>
#include <cstrike>
#include <engine>
#include <fakemeta>
#include <fakemeta_util>
#include <fun>
#include <hamsandwich>
#include <msgstocks>

/*  CSGO Accessibility  */

    #define GetRandomFloat(%0,%1) random_float(%0,%1)

    #define IsValidEntity(%0) pev_valid(%0)
    #define GetEntityClassname(%0,%1) entity_get_string(%0, EV_SZ_classname, %1, sizeof(%1))

    #define IsPlayerAlive is_user_alive

    #define TeleportEntity(%0,%1) entity_set_vector(%0, EV_VEC_origin, %1)
    #define GetVectorLength(%0) vector_length(%0)
    #define GetEntityOrigin(%0,%1) entity_get_vector(%0,EV_VEC_origin,%1)
    #define GetClientAbsOrigin(%0,%1) entity_get_vector(%0,EV_VEC_origin,%1)
    #define GetEntityVelocity(%0,%1) get_user_velocity(%0,%1)
    #define SetEntityVelocity(%0,%1) entity_set_vector(%0, EV_VEC_velocity, %1)

    #define GetClientEyeAngles(%0,%1) entity_get_vector(%0, EV_VEC_v_angle, %1)

    #define Cosine(%0) floatcos(%0, radian)
    #define Sine(%0) floatsin(%0, radian)
    #define Tangent(%0) floattan(%0, radian)
    #define Clamp clamp
    #define RoundFloat floatround
    #define FloatAbs floatabs
    #define GetVectorDistance vector_distance

    #define SubtractVectors xs_vec_sub
    #define GetVectorAngles vector_to_angle

    #define GetRandomInt random_num

/* Useful Variables */
    stock EMPTY[] = "";
    stock EMPTY_ITEM_NAME[] = "---";
    stock UNKNOWN_ITEM_NAME[] = "???";
    stock USEABLE[] = "可使用";
    stock CURRENTLY_USING[] = "使用中";
    stock charIN_COOLDOWN[] = "冷卻中";
    stock LOCKED[] = "未解鎖";
    stock NO_REMAIN[] = "已到達使用上限";
    stock NO_AIM_TARGET[] = "沒有指到任何目標";
    stock CANCEL[] = "取消";

    stock TYPE_SPLIT_DELIMITER[] = ",";
    stock VALUE_SPLIT_DELIMITER[] = "_";

    stock const ALL_ONE_BS[32] = {
        1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383, 32767, 65535, 131071,
        262143, 524287, 1048575, 2097151, 4194303, 8388607, 16777215, 33554431, 67108863, 134217727,		
        268435455, 536870911, 1073741823, 2147483647, -1
    };

    new const BLOCKED_ENTITY_NAME[][] = 			//Entity that cannot be spawn
    {
        "func_bomb_target",
        "info_bomb_target",
        "hostage_entity",
        "monster_scientist",
        "func_hostage_rescue",
        "info_hostage_rescue",
        "info_vip_start",
        "func_vip_safetyzone",
        "func_escapezone",
        "func_vehicle",
        "func_pushable",
        "func_buyzone",
        "armoury_entity"
    };


    stock GetFlagNumber(const flag, const offset=0){
        if('a' <= flag <= 'z')
            return flag - 'a' + offset;
        else if('A' <= flag <= 'Z')
            return flag - 'A' + 26 + offset;

        return -1;
    }

    stock FlagsToBitsum(flags[], &output, offset=0)
    {
        output = 0;
        stock counter=0, length = strlen(flags), temp;

        for(counter=0; counter<length; counter++)
        {
            temp = GetFlagNumber(flags[counter] - 'a', offset);

            if(temp < 0 || temp >= 32)
                continue;

            output |= (1<<temp);
        }

        return counter;
    }

    
    stock InsertArrayElementNoRepeat(arr[], max_size, insert, empty_value=0)
    {
        static i;
        i=0;
        while(i<max_size && arr[i] != empty_value)
        {
            if(arr[i] == insert)
                return;

            i++;
        }
        
        if(i != max_size)
            arr[i] = insert;
    }

    stock RemoveArrayElement(arr[], max_size, remove, empty_value=0)
    {
        static pos;
        pos = 0;
        while(pos<max_size && arr[pos]!=empty_value)
        {
            if(arr[pos] == remove)
            {
                arr[pos] = empty_value;
                break;
            }
            pos++;
        }
        while(pos+1<max_size && arr[pos+1]!=empty_value)
        {
            arr[pos] = arr[pos+1];
            arr[pos+1] = empty_value;
            pos++;
        }
    }


/* Maths  */
    //#define M_PI 3.1415926535     //amxconst.inc
    #define M_1OVER2PI 1.570796326
    #define M_2PI 6.283185307
    #define M_STRAIGHT_ANGLE 180.0
    #define M_FULL_ANGLE 360.0
    #define ToRadian(%0) (%0 * M_PI / 180.0)
    #define ToDegree(%0) (%0 * 180.0 / M_PI)
    
    #define GetRandomAngleDegree() random_float(0.0,360.0)
    #define GetRandomAngleRadian() random_float(0.0,M_2PI)
    
    stock CopyVector(Float:dest[3], Float:source[3]) { dest[0] = source[0]; dest[1] = source[1]; dest[2] = source[2]; }
    #define ResetVector(%0) %0[0]=%0[1]=%0[2]=0.0

/*  Colors  */

    #define GetRGBA_A(%0) ((%0>>24) & 0xFF)
    #define GetRGB_R(%0) ((%0&0xFF0000)>>16)
    #define GetRGB_G(%0) ((%0&0xFF00)>>8)
    #define GetRGB_B(%0) (%0&0xFF)
    stock CreateRGBInt(R, G, B)	{return (R<<16) + (G<<8) + B;}
    stock CreateRGBAInt(R, G, B, A) {return (A<<24) + (R<<16) + (G<<8) + B;}
    stock CreateRGBAIntByArray(colors[4]) {return (colors[3]<<24) + (colors[0]<<16) + (colors[1]<<8) + colors[2];}

    #define SetEntityRenderColor(%0,%1,%2,%3,%4) set_rendering(%0, entity_get_int(%0,EV_INT_renderfx), %1, %2, %3, entity_get_int(%0,EV_INT_rendermode), %4)

/*  Server Variables  */

    stock LOG_PATH[256];
    stock SINGLE_STRING_NEWLINE_FMT[] = "%s^n";
    stock DOUBLE_STRING_FMT[] = "%s%s";
    stock DOUBLE_STRING_NEWLINE_FMT[] = "%s%s^n";
    stock DOUBLE_STRING_WITH_NUMBER_FMT[] = "%s%d.%s^n";

    #define ON true
    #define OFF false 

    stock g_CvarCurRoundMode;
    stock DUMMY_INT;

/*  Games  */

    // stock GODMODE_SOUND[] = "tsubaki/effects/barrier.wav";
    // #define EMIT_GODMODE_SOUND(%0,%1) emit_sound(%0, CHAN_BODY, GODMODE_SOUND, %1, ATTN_NONE, 0, PITCH_NORM)

    stock Float:GAMETIME;
    #define RefreshGlobalGametime() GAMETIME=get_gametime()

/*  Entity  */

    #define MAX_ENTITY 512

    stock iuser[MAX_ENTITY][5];
    stock Float:fuser[MAX_ENTITY][5];
    stock Float:vuser[MAX_ENTITY][5][3];
    #define SetEntityOwner(%0,%1)		entity_set_edict(%0, EV_ENT_owner, %1)
    #define GetEntityOwner(%0)			entity_get_edict(%0, EV_ENT_owner)
    stock Float:entity_damage[MAX_ENTITY];

    stock PLAYER[] = "player";

    stock const INFO_TARGET[] = "info_target";
    stock const BULLET_DMG_TYPE = DMG_CLUB;
    stock const COMMON_BULLET_MODEL[] = "models/tsubaki/tsubaki_bullet.mdl";
    stock const BULLET_SOUND[]	= "tsubaki/effects/bullet.wav";
    #define DEFAULT_BULLET_DMG 8.0
    #define DEFAULT_BULLET_THINKTIME 0.1
    #define DEFAULT_BULLET_SOLID SOLID_TRIGGER
    stock const Float:BULLET_SIZE = 20.0;
    stock const Float:BULLET_MIN_SIZE[3] = {-15.0, -15.0, -15.0};
    stock const Float:BULLET_MAX_SIZE[3] = { 15.0,  15.0,  15.0};
    

    stock const LAUNCHER_MODEL[] = "models/rpgrocket.mdl";

    stock LASER_BEAM_SPRITE_ID;
    stock const LASER_DMG_TYPE = DMG_CLUB;
    stock const COMMON_LASER_MODEL[] = "sprites/tsubaki/laserbeam.spr";
    stock const LASER_SOUND[] = "tsubaki/effects/S_laser_launcher_3s.mp3";

    stock EXPLOSION_SPRITE_ID;
    stock const COMMON_EXPLOSION_MODEL[] = "sprites/zerogxplode.spr";


    #define SetEntityDamage(%0,%1) entity_damage[%0]=%1
    #define GetEntityDamage(%0) entity_damage[%0]
    #define SetEntityGravity(%0,%1) set_pev(%0,pev_gravity,%1)

    #define CreateBullet CreateEntity

    CreateEntity(const clsname[], const model_route[], solid_type=SOLID_TRIGGER, movetype=MOVETYPE_NOCLIP, Float:gravity=0.0, owner=0, r=255, g=255, b=255, a=16, Float:damage=DEFAULT_BULLET_DMG, render=kRenderNormal, render_fx=kRenderFxGlowShell) {
        static ent;
        
        ent = create_entity(INFO_TARGET);
        if(ent) {
            entity_set_string(ent, EV_SZ_classname, clsname);
            engfunc(EngFunc_SetModel, ent, model_route);
            entity_set_int(ent, EV_INT_solid, solid_type);
            entity_set_int(ent, EV_INT_movetype, movetype);
            entity_set_size(ent, BULLET_MIN_SIZE, BULLET_MAX_SIZE);

            if(IsClientValid(owner))
                SetEntityOwner(ent, owner);
            
            SetEntityDamage(ent, damage);
            SetEntityGravity(ent, gravity);
            set_rendering(ent, render_fx, r, g, b, render, a);
        } else {
            return -1;
        }

        return ent;		
    }

    CreateLauncher(const clsname[], owner=0, Float:damage=DEFAULT_BULLET_DMG) {
        new ent;
        
        ent = create_entity(INFO_TARGET);
        if(ent) {
            entity_set_string(ent, EV_SZ_classname, clsname);
            engfunc(EngFunc_SetModel, ent, LAUNCHER_MODEL);
            entity_set_int(ent, EV_INT_solid, SOLID_NOT);
            entity_set_int(ent, EV_INT_movetype, MOVETYPE_NONE);

            if(IsClientValid(owner))
                SetEntityOwner(ent, owner);
            
            SetEntityDamage(ent, damage);
            set_pev(ent, pev_effects, pev(ent, pev_effects) | EF_NODRAW);
        } else {
            return -1;
        }

        return ent;	
    }

    //如果係玩家，請用於身分上
    #define TASKID_ENTITY_REMOVE_BASE	9000
    #define TASKID_ENTITY_REMOVE(%0)	(9000+%0)

    #define TASKID_ENTITY_TASK_BASE     11000
    #define TASKID_ENTITY_TASK(%0)      (11000+%0)

    #define RemoveBullet RemoveEntity
    #define SetRemoveEntityTask(%0,%1) set_task_ex(%1, "RemoveEntity", TASKID_ENTITY_REMOVE(%0), _, _, SetTask_Once)
    #define SetEntityNextThink(%0,%1) entity_set_float(%0, EV_FL_nextthink, %1)
    public RemoveEntity(taskid) {
        static ent;
        ent = taskid % TASKID_ENTITY_REMOVE_BASE;

        if( pev_valid(ent) ) {
            engfunc(EngFunc_RemoveEntity, ent);

            remove_task(TASKID_ENTITY_TASK(ent));
            remove_task(TASKID_ENTITY_REMOVE(ent));
        }
    }

/*  Player  */

    stock bool:IsClientValid(const client)
    {
        if(client>0 && client<=MaxClients && is_user_connected(client))
            return true;

        return false;
    }

    #define IsFakeClient(%0) is_user_bot(%0)

    #define GetClientHealth(%0) get_user_health(%0)

    stock bool: gbPlyDeveloper[MAX_PLAYERS];
    #define IsPlayerDeveloper(%0) gbPlyDeveloper[%0]


/*  Player Weapon Variables  */

    #define MAX_WEAPON_LEVEL 4
    #define TOTAL_WEAPON	29

    stock Float:WEAPON_EXP[MAX_WEAPON_LEVEL+1] = {300.0,800.0,3000.0,7500.0, 0.0};
    stock Float:DEFAULT_SUPPLY_TIME[] = {20.0, 40.0, 80.0, 150.0, 220.0, 250.0};
    stock WEAPON_AMOUNT_PER_LEVEL[MAX_WEAPON_LEVEL+1];
    stock AVAILIABLE_WEAPON_ID[MAX_WEAPON_LEVEL+1][7];
        //  = 
        // {
        //     {0, 3, 4, 5, 6},
        //     {1, 2},
        //     {8, 19, 15, 10, 17},
        //     {12, 11, 13, 9, 7, 23},
        //     {18, 22, 24, 27, 14, 16}
        // };
    

    stock giPlyChosenWeaponList[MAX_PLAYERS][MAX_WEAPON_LEVEL+1];
    
    #define DEFAULT_WEAPON_LEVEL_CHECK_TIME		4.0

    #define DEFAULT_WEAPON_INFO 7
        #define WEAPON_INFO_SLOT			0
        #define WEAPON_INFO_AMMO			1
        #define WEAPON_INFO_RESERVED		2
        #define WEAPON_INFO_ATTACK_AFFECT	3
        #define WEAPON_INFO_SEMI_FIRE		4
        #define WEAPON_INFO_LEVEL           5
        #define WEAPON_INFO_CSWID           6

    stock WEAPON_LIST_INFO[TOTAL_WEAPON][DEFAULT_WEAPON_INFO] = {
        {1, 13, 52, 0, 1, 0, 1}, 			// 0.p228
        {1, 30, 120, 0, 1, 1, 10}, 			// 1.elite
        {1, 20, 100, 0, 1, 1, 11}, 			// 2.fiveseven
        {1, 12, 100, 0, 1, 0, 16}, 			// 3.usp
        {1, 20, 120, 0, 1, 0, 17}, 			// 4.glock18
        {1, 7, 35, 0, 1, 0, 26}, 			// 5.deagle
        {0, 10, 90, 0, 1, 0, 3}, 			// 6.scout
        {0, 7, 32, 0, 0, 3, 5}, 			// 7.xm1014
        {0, 30, 100, 0, 0, 2, 7}, 			// 8.mac10
        {0, 30, 90, 0, 0, 3, 8}, 			// 9.aug
        {0, 25, 100, 0, 0, 2, 12}, 			// 10.ump45
        {0, 30, 90, 0, 0, 3, 13}, 			// 11.sg550
        {0, 35, 90, 0, 0, 3, 14}, 			// 12.galil
        {0, 25, 90, 0, 0, 3, 15}, 			// 13.famas
        {0, 10, 30, 0, 1, 4, 18}, 			// 14.awp
        {0, 30, 120, 0, 0, 2, 19}, 			// 15.mp5navy
        {0, 100, 200, 0, 0, 4, 20}, 			// 16.m249
        {0, 8, 32, 0, 0, 2, 21}, 			// 17.m3
        {0, 30, 90, 0, 0, 4, 22}, 			// 18.m4a1
        {0, 30, 120, 0, 0, 2, 23}, 			// 19.tmp
        {0, 20, 90, 0, 0, 4, 24}, 			// 20.g3sg1
        {0, 30, 90, 0, 0, 4, 27}, 			// 21.sg552
        {0, 30, 90, 0, 0, 4, 28}, 			// 22.ak47
        {0, 50, 100, 0, 0, 3, 30}, 			// 23.p90
        {2, 1, 0, 0, 0, -1, 29}, 			// 24.knife
        {3, 1, 0, 0, 0, -1, 4}, 			// 25.hegrenade
        {3, 1, 0, 0, 0, -1, 9}, 			// 26.smokegrenade
        {3, 1, 0, 0, 0, -1, 25}, 			// 27.flashbang
        {4, 1, 0, 0, 0, -1, 6}   			// 28.c4
    };

    stock WEAPON_HEADER[] = "weapon_";
    stock WEAPON_KNIFE[] = "weapon_knife";
    stock WEAPON_LIST[TOTAL_WEAPON][] = {
        "p228",
        "elite",
        "fiveseven",
        "usp",
        "glock18",
        "deagle",
        "scout",
        "xm1014",
        "mac10",
        "aug",
        "ump45",
        "sg550",
        "galil",
        "famas",
        "awp",
        "mp5navy",
        "m249",
        "m3",
        "m4a1",
        "tmp",
        "g3sg1",
        "sg552",
        "ak47",
        "p90",
        "knife",
        "hegrenade",
        "smokegrenade",
        "flashbang",
        "c4"
    };

    stock giPlyWeaponLevel[MAX_PLAYERS];
    stock Float:gfPlyWeaponExp[MAX_PLAYERS];
    stock gbsPlyReceivedWeapon[MAX_PLAYERS];
    
    stock giPlyKnifeKillCounter[MAX_PLAYERS];
    stock giPlyHeadshotCounter[MAX_PLAYERS];
    stock headshot[] = "headshot";
    #define CS_DMG_HEADSHOT (1<<30)

    #define TASKID_PLAYER_WEAPON_CHECK(%0)  (5000+%0)
    #define TASKID_PLAYER_WEAPON_SUPPLY(%0) (6000+%0)
    stock Float:gfPlyNextSupplyTime[MAX_PLAYERS];

    #define WEAPON_BAG_INFO 4
        #define WB_WEAPON_ID		0
        #define WB_AMMO				1
        #define WB_RESERVE_AMMO		2
        #define WB_BACKUP			3

    
    stock BuildAvailiableWeaponIds() {
        new level, weapon_id;
        for(level=0; level<MAX_WEAPON_LEVEL+1; level++) {
            WEAPON_AMOUNT_PER_LEVEL[level] = 0;

            for(weapon_id=0; weapon_id<TOTAL_WEAPON; weapon_id++) {
                if(WEAPON_LIST_INFO[weapon_id][WEAPON_INFO_LEVEL] == level && WEAPON_AMOUNT_PER_LEVEL[level] < 7) {
                    AVAILIABLE_WEAPON_ID[level][WEAPON_AMOUNT_PER_LEVEL[level]++] = weapon_id;
                }
            }
        }
    }


    #define ACTIVE_INFO		2
        #define ACTIVE_ID		0
        #define ACTIVE_REMAIN	1
    stock giPlyActiveSkill[MAX_PLAYERS][ACTIVE_INFO];
    #define TASKID_AUTO_ACTIVE_SKILL(%0)    (7000+%0)


  
/*  Menu Variables  */

    #define EMIT_MENU_SOUND(%0) EmitSoundToClient(%0, MENU_SOUND, %0, SNDCHAN_ITEM, SNDLEVEL_NONE, SND_NOFLAGS, 1.0, SNDPITCH_NORMAL, client)
    
    stock const MENU_KEYS_ALL = (MENU_KEY_1|MENU_KEY_2|MENU_KEY_3|MENU_KEY_4|MENU_KEY_5|MENU_KEY_6|MENU_KEY_7|MENU_KEY_8|MENU_KEY_9|MENU_KEY_0);
    #define MENU_KEYS_BLOCK (1<<12)
    #define MENU_KEYSTROKE_0	9
    #define MENU_KEYSTROKE_1	0
    #define MENU_KEYSTROKE_2	1
    #define MENU_KEYSTROKE_3	2
    #define MENU_KEYSTROKE_4	3
    #define MENU_KEYSTROKE_5	4
    #define MENU_KEYSTROKE_6	5
    #define MENU_KEYSTROKE_7	6
    #define MENU_KEYSTROKE_8	7
    #define MENU_KEYSTROKE_9	8
    stock const MENU_TITLE_CONTENT_SPLIT[] = "^n-------------------------^n";
    stock const MENU_PREVIOUS_PAGE[] = "上一頁";
    stock const MENU_NEXT_PAGE[] = "下一頁";

    stock giPlyCurMenuId[MAX_PLAYERS];
    stock giPlyCurMenuPage[MAX_PLAYERS];
    stock giPlyCurMenuTarget[MAX_PLAYERS];

    #define MAX_MENU_ITEM		7
    #define PLY_MENU_ID(%0)		giPlyCurMenuId[%0]
    #define PLY_MENU_PAGE(%0)	giPlyCurMenuPage[%0]
        #define GetPlayerMenuOptionId(%0,%1)    (giPlyCurMenuPage[%0]*MAX_MENU_ITEM+%1)
        #define MenuHasPreviousPage(%0)         (giPlyCurMenuPage[%0]>0)
        #define MenuHasNextPage(%0,%1)          ((giPlyCurMenuPage[%0]+1)*MAX_MENU_ITEM<%1)
    #define PLY_MENU_TARGET(%0) giPlyCurMenuTarget[%0]

    ResetPlayerMenu(client)
    {
        PLY_MENU_ID(client) = PLY_MENU_PAGE(client) = PLY_MENU_TARGET(client) = 0;
        menu_cancel(client);
    }


/*   Messages  */

    stock te_create_explosion_f(Float:position[3], sprite, scale = 50, framerate = 30, flags = TE_EXPLFLAG_NONE, receiver = 0, bool:reliable = false)
    {
        if(receiver && !is_user_connected(receiver))
            return 0;

        message_begin(get_msg_destination(receiver, reliable), SVC_TEMPENTITY, .player = receiver);
        write_byte(TE_EXPLOSION);
        engfunc(EngFunc_WriteCoord, position[0]);
        engfunc(EngFunc_WriteCoord, position[1]);
        engfunc(EngFunc_WriteCoord, position[2]);
        write_short(sprite);
        write_byte(scale);
        write_byte(framerate);
        write_byte(flags);
        message_end();

        return 1;
    }

/*  Precache  */

    stock PrecacheTsubakiStuff() {
        //precache_sound(GODMODE_SOUND);
        precache_model(LAUNCHER_MODEL);	
        precache_model(COMMON_BULLET_MODEL);
        LASER_BEAM_SPRITE_ID = precache_model( COMMON_LASER_MODEL );
        EXPLOSION_SPRITE_ID = precache_model( COMMON_EXPLOSION_MODEL );
        precache_sound(BULLET_SOUND);
    }
