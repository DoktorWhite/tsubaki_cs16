#if defined _tsubaki_lib
    #endinput
#endif
#define _tsubaki_lib


#include <tsubaki/tsubaki_method_lib>

#pragma semicolon 1


#define DEBUG
//#define MAX_PLAYERS 32    //amxconst.inc

/*  SQL Variables  */
    stock DB_HOST[] = "127.0.0.1";
    stock DB_USER[] = "root";
    stock DB_PASS[] = "";

    #if defined DEBUG
        stock DATABASE_NAME[] = "tsubaki_database_debug";
    #else
        stock DATABASE_NAME[] = "tsubaki_database";
    #endif

    //stock Handle:EMPTY_HANDLE;
    
    stock Handle:g_hDatabase;

    #define QUERY_LENGTH512

/*  Map Variables  */

    #define MAX_MAP_LOAD	9
    #define MAX_ITEM_ORIGIN_LOAD	128
    
    stock MAP_LOADED;
    stock LOADED_MAP_NAME[MAX_MAP_LOAD][64];
    stock LOADED_MAP_GAMEMODE[MAX_MAP_LOAD];
    stock giPlyMapChoice[MAX_PLAYERS];

    stock CURRENT_MAP_ID;
    stock CURRENT_MAP_DIFFICULTY;
    stock CURRENT_MAP_ITEM_ORIGIN_AMOUNT;
    stock Float:CURRENT_MAP_ITEM_ORIGIN[MAX_ITEM_ORIGIN_LOAD][3];

    #define MAP_DISPLAY_RANK	5
    #define MAP_SCORE_PER_LEVEL	7500.0
    stock Float:gfPlyGameScore[MAX_PLAYERS];

/* Game Variables  */

    #if defined DEBUG
        #define GAME_START_PLAYER	10
        #define GAME_PREPARE_TIME	20.0
        #define TASKTIME_CALCULATE_PROGRESS	5.0
        #define GAME_PROGRESS_FACTOR 0.0
        #define HVZ_PLAYER_RESPAWN_TIME (12.0 * gfGameProgress / 100.0)
        #define HVZ_ZOMBIE_AMOUNT(%0) 	1	
        #define BOSS_PASSIVE_DEFAULT_RELOAD_SPD 5.0
        #define ITEM_APPEAR_INTERVAL	10.0
        #define PLAYER_NEEDED_FOR_LEADER	8
        #define SERVER_CHAT_MSG_INTERVAL	10.0
        #define AUTO_BP_INTERVAL			3.0
    #else
        #define GAME_START_PLAYER	3
        #define GAME_PREPARE_TIME	110.0
        #define TASKTIME_CALCULATE_PROGRESS	3.0
        #define GAME_PROGRESS_FACTOR (CURRENT_MAP_DIFFICULTY * 0.5 * clamp(ZOMBIE_COUNTER, 1, 6) / clamp(ALIVE_HUMAN_COUNTER, 1, 99))
        #define HVZ_PLAYER_RESPAWN_TIME (12.0 * gfGameProgress / 100.0)
        #define HVZ_ZOMBIE_AMOUNT(%0)	(1 + %0/7) 	
        #define BOSS_PASSIVE_DEFAULT_RELOAD_SPD 1.25
        #define ITEM_APPEAR_INTERVAL	20.0
        #define PLAYER_NEEDED_FOR_LEADER	8
        #define SERVER_CHAT_MSG_INTERVAL	50.0
        #define AUTO_BP_INTERVAL			15.0
    #endif


    #define TOTAL_GAME_MODE 1+5			//0 is used for empty
        stock NAME_OF_GAMEMODE[TOTAL_GAME_MODE][] =
        {
            "EMPTY",
            "人類VS喪屍",
            "大逃殺BR",
            "VS BOSS",
            "Citymall",
            "HUMAN MUST DIE"
        };
        #define GAMEMODE_HVZ			1
        #define GAMEMODE_BR				2
        #define GAMEMODE_VB				3
        #define GAMEMODE_CITYMALL		4
            #if defined DEBUG
                #define CITYMALL_DEFAULT_TIME 9999.0
            #else
                #define CITYMALL_DEFAULT_TIME	240.0
            #endif
            #define CITYMALL_EXTEND_TIME	15.0
        #define GAMEMODE_HUMAN_MUST_DIE	5

    stock giRoundCounter;
    //int giNextRoundMode;
    stock giCurRoundMode = GAMEMODE_HVZ;
        #define AllowIdentity(identity)	(GAMEMODE_POSSIBLE_IDENTITY[giCurRoundMode] & (1<<identity))
    stock gbsGameStatus = (1<<0);
        #define GAME_STATUS_PREPARE				(1<<0)
        #define GAME_STATUS_SET					(1<<1)
        #define GAME_STATUS_START				(1<<2)
        #define GAME_STATUS_END					(1<<3)
        #define GAME_STATUS_BR_END				(1<<4)
        #define GAME_STATUS_NO_KILL_LOG			(1<<7)
        #define GAME_STATUS_LAST_HUMAN			(1<<10)
        #define GAME_STATUS_NO_RESPAWN			(1<<12)
        #define GAME_STATUS_TEAM_ATK			(1<<14)
        #define GAME_STATUS_KICK_BOT			(1<<15)
        #define GAME_STATUS_EMERGENCY			(1<<18)
        #define GAME_STATUS_PROGRESS_COMPLETE	(1<<19)
        #define GAME_STATUS_BOSS				(1<<20)
        #define GAME_STATUS_BOSS_PASSIVE		(1<<22)
        #define GAME_STATUS_BOSS_FREEZE			(1<<23)
        #define GAME_STATUS_BOSS_GODMODE		(1<<24)
        #define GAME_STATUS_BOSS_ABILITY_END	(1<<26)
        #define GAME_STATUS_BOSS_FINAL			(1<<27)
        #define GAME_STATUS_FINAL_FINISHED		(1<<28)

        #define GAME_PREPARE				(gbsGameStatus & GAME_STATUS_PREPARE)
        #define GAME_START					((gbsGameStatus & GAME_STATUS_SET) || (gbsGameStatus & GAME_STATUS_START))
        #define GAME_END					(gbsGameStatus & GAME_STATUS_END)
        #define GAME_BR_END					(gbsGameStatus & GAME_STATUS_BR_END)
        #define GAME_NO_KILL_LOG			(gbsGameStatus & GAME_STATUS_NO_KILL_LOG)
        #define GAME_LAST_HUMAN				(gbsGameStatus & GAME_STATUS_LAST_HUMAN)
        #define GAME_NO_RESPAWN				(gbsGameStatus & GAME_STATUS_NO_RESPAWN)
        #define GAME_TEAM_ATK_ON			(gbsGameStatus & GAME_STATUS_TEAM_ATK)
        #define GAME_KICK_BOT				(gbsGameStatus & GAME_STATUS_KICK_BOT)
        #define GAME_EMERGENCY				(gbsGameStatus & GAME_STATUS_EMERGENCY)
        #define GAME_PROGRESS_COMPLETE		(gbsGameStatus & GAME_STATUS_PROGRESS_COMPLETE)
        #define GAME_BOSS					(gbsGameStatus & GAME_STATUS_BOSS)
        #define GAME_BOSS_PASSIVE			(gbsGameStatus & GAME_STATUS_BOSS_PASSIVE)
        #define GAME_BOSS_FREEZE			(gbsGameStatus & GAME_STATUS_BOSS_FREEZE)
        #define GAME_BOSS_GODMODE			(gbsGameStatus & GAME_STATUS_BOSS_GODMODE)
        #define GAME_BOSS_ABILITY_END		(gbsGameStatus & GAME_STATUS_BOSS_ABILITY_END)	
        #define GAME_BOSS_FINAL				(gbsGameStatus & GAME_STATUS_BOSS_FINAL)
        #define GAME_FINAL_FINISHED			(gbsGameStatus & GAME_STATUS_FINAL_FINISHED)	//Boss Ability Calculated?

    #define ENDING_HUMAN_WIN		1
    #define ENDING_HUMAN_LOSE		2
    #define ENDING_DRAW				3
    #define ENDING_BR_GAMEOVER		4
    #define ENDING_BR_TIMEUP		5

    stock Float:gfGameStartTime;
    stock Float:gfGameProgress;

    #define TASKID_GAME_STATUS  101

    #define TBK_DMG_REFLECT		(1<<25)

    /* FOR BR */
    #define BR_DEFAULT_CAMP_METER			2500.0
    #define BR_CAMP_DISTNACE				350.0
    #define BR_CAMP_METER_LOST_PER_CHECK	75.0
    #define BR_CAMP_METER_INCREASE_PER_100	10.0

    #define BR_MOVING_DISTANCE(%0)	 (fuser[%0][3])
    #define BR_CAMP_METER(%0)		 (fuser[%0][4])
    #define BR_PLAYER_NEW_ORIGIN(%0) (vuser[%0][3])
    #define BR_PLAYER_OLD_ORIGIN(%0) (vuser[%0][4])
    #define BR_TEAMMATE_1(%0)		 (iuser[%0][0])
    #define BR_TEAMMATE_2(%0)		 (iuser[%0][1])
    #define BR_TARGET_1(%0)			 (iuser[%0][2])
    #define BR_TARGET_2(%0)			 (iuser[%0][3])
    
    stock CAMP_METER_DISPLAY_FMT[] = "CAMP METER:%.0f  ";

    /*  FOR HVZ  */
    #define HVZ_LEADER_AREA_COUNTER(%0)		(iuser[%0][0])
    #define HVZ_USE_KEY_SKILL_COOLDOWN(%0)	(fuser[%0][0])
    #define LEADER_AREA_RANGE			300.0

    /*  Rewards  */
    #define REWARD_PER_ZOMBIE_KILL		250
    #define REWARD_AFTER_BOSS_KILL		5000
    #define REWARD_PER_RESPAWN			1000
 
/*  Music  */

    #define TOTAL_BGM_SITUATION		1+9
        #define BGM_STOP 				0
        #define BGM_SIT_READY			1		//a
        #define BGM_SIT_NORMAL			2		//b
        #define BGM_SIT_EMERGENCY       3		//c
        #define BGM_SIT_PROGRESS_END	4		//d
        #define BGM_SIT_BOSS			5		//e
        #define BGM_SIT_BOSS_FINAL		6		//f
        #define BGM_SIT_LOSING			7		//g
        #define BGM_SIT_WINNING			8		//h
        #define BGM_SIT_LAST_HUMAN		9		//i


    stock NAME_OF_BGM[TOTAL_BGM_SITUATION][64];
    stock SOURCE_OF_BGM[TOTAL_BGM_SITUATION][64];
    stock BGM_PATH[TOTAL_BGM_SITUATION][256];
    stock Float:BGM_LENGTH[TOTAL_BGM_SITUATION];
    stock Float:BGM_VOLUME[TOTAL_BGM_SITUATION];
    stock CUR_BGM_SITUATION;

    #define TASKID_BGM  105

/*  Game Event Variables  */

    stock HamHook:g_hTraceAttack;
    stock HamHook:g_hTakeDamage;
    stock HamHook:g_hKilled;
    stock HamHook:g_hRespawn;
    stock HamHook:g_hWeaponTouch;
    stock HamHook:g_hArmouryTouch;
    stock HamHook:g_hResetMaxSpeed;
    stock HamHook:g_hBloodColor;
    stock g_hCmd;
    stock g_pGameRules;
    stock g_msgid_hideweapon;
    stock g_hAllToFullPack;

/*  Event Variables  */

    #define TASKID_PLAYER_RESPAWN(%0)   (3000+%0)
    stock Float:gfPlayerRespawnTime[MAX_PLAYERS];
    
    stock CRITICAL_DMG_MSG[] = "爆撃";
    
/*  Ability Variables  */
    #define TOTAL_ABILITY 12+1
        #define ABILITY_HP_LIMIT 	1	//a		
        #define ABILITY_ATK			2	//b
        #define ABILITY_DEF			3	//c
        #define ABILITY_GRAV		4	//d
            #define PLAYER_BASE_GRAVITY 800.0
        #define ABILITY_SPD			5	//e
            #define PLAYER_BASE_SPEED	320.0
        #define ABILITY_ITEM_EFFECT	6	//f	//薬物効果
        #define ABILITY_BUFF_RATE	7	//g	//バフ率
        #define ABILITY_DEBUFF_RATE	8	//h	//デバフ防御率
        #define ABILITY_WEAPON_EXP	9	//i
        #define ABILITY_DROP_RATE	10	//j	//ドロップ率
        #define ABILITY_SUPPLY_TIME	11  //k 補給時間
        #define ABILITY_CRITICAL	12  //l 爆擊機率

    #define CRITICAL_DMG_AMPIFIER	2.0


    stock NAME_OF_ABILITY[TOTAL_ABILITY][] =
    {
        "", 
        "生命値上限", 			//a
        "攻擊", 				//b
        "防御", 				//c
        "重力軽減", 			//d
        "速度", 				//e
        "薬物効果", 			//f
        "BUFF機率", 			//g
        "DEBUFF防御機率", 		//h
        "武器経験値増幅率", 	//i		
        "物品掉落率",			//j
        "補給時間",				//k
        "爆擊機率"				//l
    };

    stock Float:HUMAN_ABILITY_DEFAULT_PAUSE[TOTAL_ABILITY];
    stock Float:HUMAN_ABILITY_DEFAULT_START[TOTAL_ABILITY];
    stock Float:HUMAN_ABILITY_LOW[TOTAL_ABILITY];
    stock Float:HUMAN_ABILITY_TOP[TOTAL_ABILITY];

    stock Float:gfPlyCurAbility[MAX_PLAYERS][TOTAL_ABILITY];
    stock Float:gfPlyFreeAbility[MAX_PLAYERS][TOTAL_ABILITY];
    stock Float:gfPlyEquipAbility[MAX_PLAYERS][TOTAL_ABILITY];
    stock Float:gfPlyGameProgressAbility[MAX_PLAYERS][TOTAL_ABILITY];
    stock Float:gfPlyAbnormalAbility[MAX_PLAYERS][TOTAL_ABILITY];

    stock bool:gbPlyGodMode[MAX_PLAYERS];
    stock bool:gbPlyFreeze[MAX_PLAYERS];

    #define TASKID_CALCULATE_FINAL_ABILITY  (4000+%0)
    
    #define DEFAULT_SP 5

    stock giPlyLevel[MAX_PLAYERS];
    stock giPlyAvailableSP[MAX_PLAYERS];
    stock giPlySP[MAX_PLAYERS][TOTAL_ABILITY];

    stock MAX_SP[TOTAL_ABILITY] = {
        0,
        60,
        0,
        35,
        35,
        35,
        0,
        20,
        20,
        35,
        35,
        35,
        35
    };
    stock Float:SP_ABILITY_FACTOR[TOTAL_ABILITY] = {
        0.0,
        0.7,			//HP
        0.01,			//ATK
        0.005,			//DEF
        -3.0,			//GRAV
        1.5,			//SPD
        0.005,			//ITEM EFFECT
        0.005,			//BUFF
        0.005,			//DEBUFF
        0.02,			//WEAPON EXP
        0.0025,			//DROP
        0.0075,
        0.0025
    };

    stock Float:BOT_ABILITY_AMPIFIER[TOTAL_ABILITY] = {
        0.0,
        0.5,
        -0.2,
        0.0,
        0.95,
        0.2,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0
    };

    #define BOSS_ATK_INCREASE_PER_OLD_PLAYER	0.25
    #define ATK_INCREASE_PER_PASSIVE			0.0125

/*  Identity Variables  */
    #define TOTAL_IDENTITY  (1+4)			//0 is used for empty(Total Player)
        #define IDENTITY_HUMAN		1		//flag:a
        #define IDENTITY_LEADER		2		//flag:b
        #define IDENTITY_ZOMBIE		3		//flag:c
        #define IDENTITY_BOSS		4		//flag:d

        #define ID_ALL			(ID_HUMAN|ID_LEADER|ID_ZOMBIE|ID_BOSS)
        #define ID_HUMAN		(1<<IDENTITY_HUMAN)
        #define ID_LEADER		(1<<IDENTITY_LEADER)
        #define ID_ZOMBIE		(1<<IDENTITY_ZOMBIE)
        #define ID_BOSS			(1<<IDENTITY_BOSS)

    stock NAME_OF_IDENTITY[TOTAL_IDENTITY][] =
    { 
        "EMPTY", "人類", "隊長", "喪屍", "BOSS"
    };

    stock GAMEMODE_POSSIBLE_IDENTITY[TOTAL_GAME_MODE] = 
    {
        0,
        (1<<IDENTITY_HUMAN)|(1<<IDENTITY_LEADER)|(1<<IDENTITY_ZOMBIE)|(1<<IDENTITY_BOSS),		//HVZ
        (1<<IDENTITY_HUMAN),																	//BR
        (1<<IDENTITY_HUMAN)|(1<<IDENTITY_ZOMBIE)|(1<<IDENTITY_BOSS),								//VB
        (1<<IDENTITY_HUMAN),																	//Citymall
        (1<<IDENTITY_HUMAN)|(1<<IDENTITY_ZOMBIE)												//HUMAN MUST DIE
    };

    #define ALLOW_IDENTITY(%0) (GAMEMODE_POSSIBLE_IDENTITY[giCurRoundMode] & (1<<%0))

    stock giPlyIdentity[MAX_PLAYERS];
    #define PLY_IDENTITY(%0) 	giPlyIdentity[%0]
    #define IsPlayerHuman(%0) 	(giPlyIdentity[%0] == IDENTITY_HUMAN || giPlyIdentity[%0] == IDENTITY_LEADER)
    #define IsPlayerZombie(%0)	(giPlyIdentity[%0] == IDENTITY_ZOMBIE)
    #define IsPlayerBoss(%0)	(giPlyIdentity[%0] == IDENTITY_BOSS)

    stock LEADER_ID;

    #define ChangePlayerIdentity(client, identity) (giPlyIdentity[client] = identity)

    stock ALIVE_IDENTITY_COUNTER[TOTAL_IDENTITY];
    stock IDENTITY_COUNTER[TOTAL_IDENTITY];

    #define ALIVE_HUMAN_COUNTER			(ALIVE_IDENTITY_COUNTER[IDENTITY_HUMAN] + ALIVE_IDENTITY_COUNTER[IDENTITY_LEADER])
    #define ALIVE_ZOMBIE_COUNTER		(ALIVE_IDENTITY_COUNTER[IDENTITY_ZOMBIE])
    #define ZOMBIE_COUNTER				(IDENTITY_COUNTER[IDENTITY_ZOMBIE])

    #define GetPlayers_IncludeHumans			(1<<IDENTITY_HUMAN)
    #define GetPlayers_IncludeLeader			(1<<IDENTITY_LEADER)
    #define GetPlayers_IncludeZombies			(1<<IDENTITY_ZOMBIE)
    #define GetPlayers_IncludeBoss				(1<<IDENTITY_BOSS)
    #define GetPlayers_IncludeAllIdentity		(1<<TOTAL_IDENTITY)
    #define GetPlayers_IncludeBots				(1<<24)
    #define GetPlayers_IncludeAlive				(1<<25)
    #define GetPlayers_IncludeDead				(1<<26)
    #define GetPlayers_IncludeSpectator			(1<<28)

/*  Player Variables  */

    //Player Motion
    stock gbsPlyOldButtons[MAX_PLAYERS];
    
    stock gbsPlyGameStatus[MAX_PLAYERS];	
        #define PLY_STATUS_SERIOUS_INJURE	(1<<0)

        #define PGS_SERIOUS_INJURED(%0)		(gbsPlyGameStatus[%0] & PLY_STATUS_SERIOUS_INJURE)

    //Will be Saved
    stock giPlyID[MAX_PLAYERS];
    stock gsPlayerName[MAX_PLAYERS][64];
    stock gsPlayerTitle[MAX_PLAYERS][32];

    stock m_CollisionGroup[] = "m_CollisionGroup";
    stock m_hObserverTarget[] = "m_hObserverTarget";

    stock GetPlayerAimingTarget(client)
    {
        if(!IsPlayerAlive(client))
            return GetEntPropEnt(client, Prop_Send, m_hObserverTarget);
        else
            return GetClientAimTarget(client);
    }

    #define PLAYTIME_PER_LEVEL	1800
    #define NEWBIE_LEVEL 70
    #define IsPlayerNewbie(%0) (giPlyLevel[%0]<=NEWBIE_LEVEL)
    stock giPlyTotalPlaytime[MAX_PLAYERS];
    stock giPlyLoginTimeStamp[MAX_PLAYERS];
    stock gbsPlayerSettingBits[MAX_PLAYERS];
        #define SETTING_DISABLE_NEWBIE_TUTORIAL	(1<<0)
        #define SETTING_DISABLE_AUTO_DISPLAY_BP	(1<<1)
        #define SETTING_DISABLE_HUD				(1<<31)

        #define PS_DISABLE_NEWBIE_TUTORIAL(%0)		(gbsPlayerSettingBits[%0] & SETTING_DISABLE_NEWBIE_TUTORIAL)
        #define PS_DIABLE_AUTO_BP(%0)				(gbsPlayerSettingBits[%0] & SETTING_DISABLE_AUTO_DISPLAY_BP)
        #define PS_DISABLE_HUD(%0)					(gbsPlayerSettingBits[%0] & SETTING_DISABLE_HUD)
    stock giPlyLastLoginRound[MAX_PLAYERS];
    stock gbsPlyRoundResetBits[MAX_PLAYERS];
        #define ROUND_RESET_NEWBIE_SUPPORT	(1<<0)
        #define ROUND_RESET_WAREHOUSE_RESET	(1<<1)
        #define ROUND_RESET_EXTEND_CITYMALL	(1<<2)
        #define ROUND_RESET_CITYMALL_STORE	(1<<3)

        #define RR_NEWBIE_SUPPORT(%0)	(gbsPlyRoundResetBits[%0] & ROUND_RESET_NEWBIE_SUPPORT)
        #define RR_WAREHOUSE_RESET(%0)	(gbsPlyRoundResetBits[%0] & ROUND_RESET_WAREHOUSE_RESET)	//玩家重新整理過一次倉庫
        #define RR_EXTEND_CITYMALL(%0)	(gbsPlyRoundResetBits[%0] & ROUND_RESET_EXTEND_CITYMALL)
        #define RR_CITYMALL_STORE(%0)	(gbsPlyRoundResetBits[%0] & ROUND_RESET_CITYMALL_STORE)


    stock giPlyVoiceVolume[MAX_PLAYERS];
    stock giPlyBGMVolume[MAX_PLAYERS];
    
    
    stock Float:gfPlyDueDamage[MAX_PLAYERS][MAX_PLAYERS];
    stock Float:gfPlyLastDamageTime[MAX_PLAYERS];
    stock giPlyKilledPlayerAmount[MAX_PLAYERS][TOTAL_IDENTITY];
    stock giPlyBrokenDoor[MAX_PLAYERS];
    stock giPlyLastHitBy[MAX_PLAYERS];

    #define KILL_FOR_RESPAWN	3
    #define ZOMBIE_KILL_COUNTER(%0) (giPlyKilledPlayerAmount[%0][IDENTITY_HUMAN]+giPlyBrokenDoor[%0])
    stock WillZombieRespawn(client, bool:force_respawn=false)
    {
        if(!force_respawn)
        {
            return ((ZOMBIE_KILL_COUNTER(client))%KILL_FOR_RESPAWN==0);
        }
        
        return true;
    }

    stock PROP_DOOR_ROTATING[] = "prop_door_rotating";
    stock FUNC_DOOR[] = "func_door";

    stock Float:gfPlyUnstuckTime[MAX_PLAYERS];

/*  Boss Variables  */

    #define MAXIMUM_BOSS_AMOUNT 4
    stock BOSS_ID[MAXIMUM_BOSS_AMOUNT];		//Boss　プレイヤーID
    stock Float:BOSS_FINAL_ADJUSTOR[MAXIMUM_BOSS_AMOUNT];
    stock bool: BOSS_ABILITY_CALCULATED[MAXIMUM_BOSS_AMOUNT];	//Boss能力計算した？
    stock BOSS_FINAL_ID;
    stock BOSS_FINAL_COUNTER;
    stock BOSS_FINAL_MSG_FMT[128];
    #define TASKID_PASSIVE_COUNTER  301
    #define TASKID_PASSIVE_CONTINUE 401

    //Space Compression
    #define TASKID_SPACE_COMPRESSION        501
    #define TASKID_SPACE_COMPRESSION_HUD    502

    //Boss Active Skill'
    #define TOTAL_BOSS_ACTIVE_SKILL		4
    stock Float:BOSS_ACTIVE_SKILL_COOLDOWN[TOTAL_BOSS_ACTIVE_SKILL];
    #define BOSS_ACTIVE_COOLDOWN(%0)		(BOSS_ACTIVE_SKILL_DEFAULT_DATA[%0][0]/100.0)
    #define CanBossUseActiveInFinal(%0)		(BOSS_ACTIVE_SKILL_DEFAULT_DATA[%0][1]!=0)

    #define BOSS_ACTIVE_DATA		4
        #define BADATA_COOLDOWN				0
        #define BADATA_CDR_FOR_NEWBIE		1
        #define BADATA_CDR_PER_OLD_PLAYER	2
        #define BADATA_COOLDOWN_MINIMUM		3

    stock BOSS_ACTIVE_SKILL_DEFAULT_DATA[TOTAL_BOSS_ACTIVE_SKILL][TOTAL_BOSS_ACTIVE_SKILL] = 
    {
        {5500, 1000, 300, 2000},
        {3500,  750, 250, 2000},
        {7000, 1000,   0, 6000},
        {4500,  500, 250, 2500},
    };
    /*
    int BOSS_ACTIVE_SKILL_DEFAULT_DATA[TOTAL_BOSS_ACTIVE_SKILL][2] = 
    {
        {5000, 1},
        {3500, 0},
        {7000, 0},
        {4000, 1}
    };
    */

    stock BOSS_ACTIVE_SKILL_NAME[TOTAL_BOSS_ACTIVE_SKILL][] = {
        "雷射",
        "反射傷害",
        "傳送周圍人類",
        "隨機彈幕"
    };
    #define BOSS_ACTIVE_POINTING_LASER	0
    #define BOSS_ACTIVE_REFLECT_DAMAGE 	1
    #define BOSS_ACTIVE_TELEPORT_HUMAN	2
    #define BOSS_ACTIVE_RANDOM_STG		3

    stock BOSS_PASSIVE_CNT_FMT[] = "BOSS 技能 RELOAD 中 (%.0f/100) %s";
    stock Float:BOSS_PASSIVE_RELOAD_SPD = BOSS_PASSIVE_DEFAULT_RELOAD_SPD;
    #define BOSS_PASSIVE_COUNTER_RATE 0.1

    #define TOTAL_FINAL		4
        #define FINAL_ALPHA		0
        #define FINAL_DELTA		1
            #define DELTA_TELEPORT_CD	7.0-ALIVE_HUMAN_COUNTER*0.2
        #define FINAL_GAMMA		2
        #define FINAL_LAMBDA	3
            #define LAMBDA_DMG_AREA_CD	10.0-ALIVE_HUMAN_COUNTER*0.2

/*  Item  Variables  */

    #define TOTAL_ITEM 64
    #define TOP_ITEM_RARE 8

    #define ITEM_DATA 11
        #define ITEM_DATA_TYPE					0
            #define IT_REMOVABLE			(1<<1) 		//a
            #define IT_ETERNAL				(1<<2) 		//b
            #define IT_CONTINUOUS_EFFECT	(1<<3) 		//c
            #define IT_EQUIP				(1<<4) 		//d
            #define IT_INSTANTLY			(1<<5)		//e		一次性
            #define IT_PERIOD				(1<<6)		//f
            #define IT_GAMBLE				(1<<12)		//l
            #define IT_GREAT_GAMBLE			(1<<14)		//n
            #define IT_HEAL					(1<<16)		//p
            #define IT_ONLY_HEALING_ITEM	(1<<17)		//q
            #define IT_GAME_PAUSE			(1<<20)		//t
            #define IT_TRIGGER_WHILE_DEAD	(1<<22)		//v
            #define IT_MATERIAL				(1<<26)		//z
        #define ITEM_DATA_IDENTITY				1
        #define ITEM_DATA_DURABILITY			2
        #define ITEM_DATA_RARE					3
        #define ITEM_DATA_LAST_TIME				4
        #define ITEM_DATA_CONTINUOUS_RATE		5
        #define ITEM_DATA_AVAILABLE_GAMEMODE	6
        #define ITEM_DATA_MAX_LEVEL				7
        #define ITEM_DATA_INSTANT_HEAL			8
        #define ITEM_DATA_CONTINUOUS_HEAL		9
        #define ITEM_DATA_SELL_PRICE			10

    #define ITEM_HUD_WITHOUT_TIME (IT_EQUIP|IT_ETERNAL)
    #define ITEM_DISPLAY_TIME(item) (~ITEM_HUD_WITHOUT_TIME & ITEM_DEFAULT_DATA[item][ITEM_TYPE])

    stock LOADED_ITEM_AMOUNT;
    stock NAME_OF_ITEM[TOTAL_ITEM][32];
    stock DESCRIPTION_OF_ITEM[TOTAL_ITEM][64];
    stock ITEM_DEFAULT_DATA[TOTAL_ITEM][ITEM_DATA];
    stock ITEM_ABILITY_LIST[TOTAL_ITEM][TOTAL_ABILITY];
    stock Float:ITEM_ABILITY_VALUE[TOTAL_ITEM][TOTAL_ABILITY];
    stock ITEM_DISPLAY_SORT[TOTAL_ITEM];

    stock GAMBLE_ITEM_AMOUNT;
    stock GAMBLE_ITEM[TOTAL_ITEM/2];
    stock GREAT_GAMBLE_ITEM_AMOUNT;
    stock GREAT_GAMBLE_ITEM[TOTAL_ITEM/2];

    #define ITEM_ENTITY_THINKTIME	0.5
    #define ITEM_ENTITY_SEARCH_RANGE 30.0
    #define MAX_APPEAR_ITEM_RARE	8
    stock RANDOM_ITEM_AMOUNT[TOTAL_IDENTITY];
    stock RANDOM_ITEM_LIST[TOTAL_IDENTITY][TOTAL_ITEM*MAX_APPEAR_ITEM_RARE];

    stock RANDOM_ITEM_AMOUNT_AFTER_KILL[TOTAL_IDENTITY];
    stock RANDOM_ITEM_LIST_AFTER_KILL[TOTAL_IDENTITY][TOTAL_ITEM*MAX_APPEAR_ITEM_RARE];

    stock RANDOM_ITEM_AMOUNT_IN_STORE;
    stock RANDOM_ITEM_IN_STORE[TOTAL_ITEM];

    stock giPlyTriggeredItem[MAX_PLAYERS][TOTAL_ITEM];
    stock giPlyTriggeredItemLevel[MAX_PLAYERS][TOTAL_ITEM];
    stock Float:gfPlyItemTime[MAX_PLAYERS][TOTAL_ITEM];
    stock bool: gbCanPlayerGetItem[MAX_PLAYERS];

    #define TASKID_ITEM_REMOVE_BASE 1000000
    #define TASKID_ITEM_EFFECT_BASE 2000000
    stock TASKID_ITEM_REMOVE(client, item_id) { return TASKID_ITEM_REMOVE_BASE + client*10000 + item_id; }
    stock TASKID_ITEM_EFFECT(client, item_id) { return TASKID_ITEM_EFFECT_BASE + client*10000 + item_id; }

    stock GetItemDisplayId(item_id)
    {
        for(new i=0; i<TOTAL_ITEM && ITEM_DISPLAY_SORT[i]!=0; i++)
            if(ITEM_DISPLAY_SORT[i] == item_id)
                return i;

        return -1;
    }

    //Identity flags
    // a~o	attacker identity
    // p~z	victim identity

/*  Abnormal Variables  */
    #define TOTAL_ABNORMAL	64

    #define ABNORMAL_DATA 5
        #define ABNORMAL_DATA_TYPE						0
            #define AT_REMOVABLE			(1<<1)		//a
            #define AT_ETERNAL				(1<<2)		//b
            #define AT_CONTINUOUS_EFFECT	(1<<3)		//c
            #define AT_EQUIP				(1<<4)		//d
            #define AT_GAMBLE_BUFF			(1<<12)		//l
            #define AT_GAMBLE_DEBUFF		(1<<13)		//m
            #define AT_INSURANCE_TARGET		(1<<14)		//n		醫療保險對象
            #define AT_GAME_PAUSE			(1<<20)		//t
            #define AT_TRIGGER_WHILE_DEAD	(1<<22)		//v
            #define AT_DONT_DISPLAY			(1<<23)		//w
            #define AT_BUFF					(1<<24)		//x
            #define AT_DEBUFF				(1<<25)		//y
        #define ABNORMAL_DATA_IDENTITY					1
        #define ABNORMAL_DATA_MAX_LEVEL					2
        #define ABNORMAL_DATA_AVAILABLE_GAMEMODE		3
        #define ABNORMAL_DATA_CONTINUOUS_EFFECT_RATE	4

    #define ABNORMAL_HUD_WITHOUT_TIME (AT_EQUIP|AT_ETERNAL)
    #define ABNORMAL_DONT_DISPLAY(abnormal) (AT_DONT_DISPLAY & ABNORMAL[abnormal][ABNORMAL_TYPE])
    #define ABNORMAL_DISPLAY_TIME(abnormal)	(~ABNORMAL_HUD_WITHOUT_TIME & ABNORMAL[abnormal][ABNORMAL_TYPE])

    stock  LOADED_ABNORMAL;
    stock NAME_OF_ABNORMAL[TOTAL_ABNORMAL][32];
    stock  ABNORMAL_DEFAULT_DATA[TOTAL_ABNORMAL][ABNORMAL_DATA];
    stock  ABNORMAL_ABILITY_LIST[TOTAL_ABNORMAL][TOTAL_ABILITY];
    stock Float:ABNORMAL_ABILITY_VALUE[TOTAL_ABNORMAL][TOTAL_ABILITY];

    stock  GAMBLE_BUFF_AMOUNT;
    stock  GAMBLE_BUFF[TOTAL_ABNORMAL/3];
    stock  GAMBLE_DEBUFF_AMOUNT;
    stock  GAMBLE_DEBUFF[TOTAL_ABNORMAL/3];

    stock  giPlyTriggerAbnormal[MAX_PLAYERS][TOTAL_ABNORMAL];
    stock  giPlyTriggerAbnormalLevel[MAX_PLAYERS][TOTAL_ABNORMAL];
    stock giPlyAbnormalTriggeredPlayer[MAX_PLAYERS][TOTAL_ABNORMAL];
    stock Float:gfPlyAbnormalTime[MAX_PLAYERS][TOTAL_ABNORMAL];

    #define TASKID_ABNORMAL_EFFECT_BASE 3000000
    #define TASKID_ABNORMAL_REMOVE_BASE 4000000
    stock TASKID_ABNORMAL_EFFECT(client, abnormal) { return TASKID_ABNORMAL_EFFECT_BASE+client*10000+abnormal; }
    stock TASKID_ABNORMAL_REMOVE(client, abnormal) { return TASKID_ABNORMAL_REMOVE_BASE+client*10000+abnormal; }

/*  Equip Variables  */
    #define TOTAL_EQUIP		64
    #define EQUIP_DISPLAY_RARE	5
        #define DISPLAY_EQUIP(%0)	(EQUIP_DEFAULT_DATA[%0][EQUIP_DATA_RARE] <= EQUIP_DISPLAY_RARE)

    #define EQUIP_DATA		8
        #define EQUIP_DATA_TYPE					0
            #define TOTAL_EQUIP_TYPE				4			
            #define TOTAL_EQUIP_TYPE_ALL_IDENTITY	TOTAL_EQUIP_TYPE+1		//1 is for leader
            #define GetEquipSlot(%0)		(EQUIP_DEFAULT_DATA[%0][EQUIP_DATA_TYPE]-1)
        #define EQUIP_DATA_RARE					1
        #define EQUIP_DATA_USE_LIMIT			2
        #define EQUIP_DATA_COOLDOWN				3
        #define EQUIP_DATA_ROUND_COOLDOWN		4
        #define EQUIP_DATA_ENABLE				5
        #define EQUIP_DATA_AVAILABLE_GAMEMODE	6
        #define EQUIP_AUTO_TRIGGER_MSG			7
    
    stock LOADED_EQUIP;
    stock NAME_OF_EQUIP[TOTAL_EQUIP][32];
    stock EQUIP_DESCRIPTION[TOTAL_EQUIP][128];
    stock EQUIP_UNLOCK_CONDITION[TOTAL_EQUIP][128];
    stock EQUIP_DISPLAY_LIST[TOTAL_EQUIP_TYPE_ALL_IDENTITY][TOTAL_EQUIP/2];

    stock EQUIP_DEFAULT_DATA[TOTAL_EQUIP][EQUIP_DATA];
    stock EQUIP_ABILITY_LIST[TOTAL_EQUIP][TOTAL_ABILITY];
    stock Float:EQUIP_ABILITY_VALUE[TOTAL_EQUIP][TOTAL_ABILITY];

    stock giPlyEquipLastUsedRound[MAX_PLAYERS][TOTAL_EQUIP];
    stock gbsPlyEquipUnlocked[MAX_PLAYERS][1+(TOTAL_EQUIP/32)];
        stock bool:IsPlayerEquipUnlocked(client, equip_id) { return (gbsPlyEquipUnlocked[client][equip_id/32])&(1<<(equip_id%32)) != 0; }
        UnlockPlayerEquip(client, equip_id, bool:dontBroadcast=false) 
        { 
            if(!IsClientValid(client) || IsPlayerEquipUnlocked(client, equip_id)) return;

            gbsPlyEquipUnlocked[client][equip_id/32] |= (1<<(equip_id%32));
            if(!dontBroadcast)
                client_print(client, print_chat, " \x04已解鎖裝備%s", NAME_OF_EQUIP[equip_id]);

            if(!IsPlayerEquipUnlocked(client, 15))
            {
                new counter = 0, i=0;
                for(;i<LOADED_EQUIP; i++)
                    if(IsPlayerEquipUnlocked(client, i))
                        counter++;

                if(counter >= 15)
                    UnlockPlayerEquip(client, 15, dontBroadcast);
            }
        }
    stock giPlyCurrentEquip[MAX_PLAYERS][TOTAL_EQUIP_TYPE_ALL_IDENTITY];
    stock giPlyCurrentEquipRemain[MAX_PLAYERS][TOTAL_EQUIP_TYPE_ALL_IDENTITY];
    stock Float:gfPlyCurrentEquipCooldown[MAX_PLAYERS][TOTAL_EQUIP_TYPE_ALL_IDENTITY];
    stock giEquipParam[MAX_PLAYERS][TOTAL_EQUIP_TYPE_ALL_IDENTITY];

    #define TASKID_EQUIP_EFFECT_BASE 5000000
    stock TASKID_EQUIP_EFFECT(client,equip) { return TASKID_EQUIP_EFFECT_BASE+client*10000+equip; }

    //Slot Menu
    stock EQUIP_ITEM_SLOT_FORMAT[] = "装備%c:%s";
    stock EQUIP_ITEM_SLOT_DETAIL_FORMAT[] = "装備%c:%s(%s)";
    stock EQUIP_ITEM_SLOT_CDT_FMT[] = "装備%c:%s(冷卻中 [%.0f秒])";
    
    //Select Menu
    stock EQUIP_ITEM_DISPLAY_FORMAT[] = "%s(%s)";
    stock EQUIP_ITEM_DISPLAY_CDR_FMT[] = "%s(冷卻中 [%d局])";
    stock EQUIP_ITEM_PRICE_FMT[] = "%s(價格$%d)";
    stock EQUIP_ITEM_NOT_SELLING_FMT[] = "%s(非賣品)";
    
    //裝置局數Cooldown
    stock GetEquipCooldownRound(const client, const equip) { return clamp(giPlyEquipLastUsedRound[client][equip]==0?0:giPlyEquipLastUsedRound[client][equip]+EQUIP_DEFAULT_DATA[equip][EQUIP_DATA_ROUND_COOLDOWN]+1-giRoundCounter, 0, 99999); }
    //裝備是否使用Cooldown中
    stock IsEquipSlotInCooldown(const client, const slot) { return gfPlyCurrentEquipCooldown[client][slot] > get_gametime() ; }


    #define BULLET_INSURANCE_AMOUNT	250
    #define BULLET_INSURANCE_DMG_AMPIFIER	1.3
    #define DEBUFF_INSURANCE_AMOUNT	500

    #define A5_HEAL_AMOUNT	1

/*  Backpack and Warehouse Variables  */

    #define DEFAULT_BP_SPACE 5
    #define DEFAULT_WH_SPACE 20

    #define MAX_BP_SPACE 10
    #define MAX_WH_SPACE 100

    #define TOTAL_PLAYER_ITEM_INFO	2
        #define PLAYER_ITEM_ID			0
        #define PLAYER_ITEM_DURABILITY	1

    stock giPlyBPSpace[MAX_PLAYERS];
    stock giPlyWHSpace[MAX_PLAYERS];

    stock giPlyBPItem[MAX_PLAYERS][MAX_BP_SPACE][TOTAL_PLAYER_ITEM_INFO];
    stock giPlyWHItem[MAX_PLAYERS][MAX_WH_SPACE][TOTAL_PLAYER_ITEM_INFO];

    #define BP_CASH_LIMIT	2000000000
    stock giPlyBPCash[MAX_PLAYERS];
    stock giPlyWHCash[MAX_PLAYERS];

    #define SetPlayerMoney(%0,%1) cs_set_user_money(%0,%1,1)
    #define GetPlayerMoney(%0) cs_get_user_money(%0)

    stock ClearWHSpace(client, pos, info=0) {
        for(; info<TOTAL_PLAYER_ITEM_INFO; info++) 
            giPlyWHItem[client][pos][info]=0;
    }

    stock GivePlayerCash(client, amount) {
        if( (giPlyBPCash[client] += amount) > BP_CASH_LIMIT )
        {
            client_print(client, print_chat, " \x04已存入$%d入銀行", giPlyBPCash[client]);
            
            giPlyWHCash[client] += giPlyBPCash[client];
            giPlyBPCash[client] = 0;
        }
        
        SetPlayerMoney(client, giPlyBPCash[client]);
    } 

    stock bool:gbGamemodeLoadBPWH[TOTAL_GAME_MODE] = {
        false,
        true,
        false,
        true,
        true,
        false
    };
    

/*  Zombie Variables  */
    #define SELECTABLE_ZOMBIE 7
    #define TOTAL_ZOMBIE 		SELECTABLE_ZOMBIE+4
    #define ZOMBIE_DUMMY 		SELECTABLE_ZOMBIE+1
    #define ZOMBIE_BOSS 		SELECTABLE_ZOMBIE+2
    #define ZOMBIE_COPY_BOSS	SELECTABLE_ZOMBIE+3

    stock NAME_OF_ZOMBIE[TOTAL_ZOMBIE][32];
    stock NAME_OF_ZOMBIE_SKILL[TOTAL_ZOMBIE][32];
    stock ZOMBIE_MODEL[TOTAL_ZOMBIE][256];
    stock Float:ZOMBIE_ABILITY[TOTAL_ZOMBIE][TOTAL_ABILITY];
    stock Float:ZOMBIE_ABILITY_PER_PLAYER[TOTAL_ZOMBIE][TOTAL_ABILITY];
    stock Float:ZOMBIE_ABILITY_PER_PROGRESS[TOTAL_ZOMBIE][TOTAL_ABILITY];

    stock const ZOMBIE_SPAWN_SOUND[] = "tsubaki/effects/zombie_sound1.mp3";

    stock giPlyCurZombieType[MAX_PLAYERS];
    stock giPlyChosenZombieType[MAX_PLAYERS];

/*  Menu Variables  */

    #define MENUID_BACKPACK_USE		8
    #define MENUID_BACKPACK_THROW	9

    #define MENUID_BP_TO_WH			12
    #define MENUID_WH_TO_BP			14

    #define MENUID_BP_TRADE			18
    #define MENUID_WH_SELL			19

    #define MENUID_EQUIP		16

    #define MENUID_MISSION		31
    #define MENUID_TUTORIAL		33

    #define MENUID_MODEL_DISPLAY		61
    #define MENUID_ARM_DISPLAY			62
    #define MENUID_SOUND_DISPLAY		63


    #define MENUID_CRAFT_MENU_IN_CM		102
    #define MENUID_CRAFT_MENU			103

    //DONT use 200~299 if not citymall shop
    #define MENUID_SHOP_BASE		200

/*  Command Variables  */

    stock const PLAYER_COMMAND_SP[] = "/sp";
    stock const PLAYER_COMMAND_BP_USE[] = "/bp";
    stock const PLAYER_COMMAND_EQUIP[] = "/equip";
    stock const PLAYER_COMMAND_BP_THROW[] = "/throw";
    stock const PLAYER_COMMAND_WEAPON_BACKPACK[] = "/wp";
    stock const PLAYER_CMD_WEAPON_BACKPACK_F[] = "/weapon";
    stock const PLAYER_COMMAND_VOTEMAP[] = "/votemap";
    stock const PLAYER_COMMAND_ZOMBIE_MENU[] = "/zombie";
    stock const PLAYER_COMMAND_FLASHLIGHT[] = "/flashlight";
    stock const PLAYER_COMMAND_NIGHTVISION[] = "/nightvision";
    stock const PLAYER_COMMAND_SAVEDATA[] = "/savedata";
    stock const PLAYER_COMMAND_SETTING[] = "/setting";
    stock const PLAYER_COMMAND_UNSTUCK[] = "/stuck";
    stock const PLAYER_COMMAND_CAMERA[] = "/camera";
    stock const PLAYER_COMMAND_TUTORIAL[] = "/help";
    stock const PLAYER_COMMAND_TRADE[] = "/trade";
    stock const PLAYER_COMMAND_TELEPORT[] = "/tp";

    stock Float:gfPlyLastTeleportTime[MAX_PLAYERS];

/*  HUD Message Variables  */
    //HUD maximum 4 channel
    #define HUD_CHN_GAME_START_TIMER	1
    #define HUD_CHN_PLAYER_TEMP_MSG		1
    #define HUD_CHN_RESPAWN				1
    #define HUD_CHN_BOSS_SKILL 			1
    #define HUD_CHN_BR_ENDING			1

    #define HUD_CHN_SERVER_UPDATE		2
    #define HUD_CHN_GAME_PROGRESS		2
    #define HUD_CHN_SPACE_COMPRESSION	2
    
    #define HUD_CHN_PLAYER_INFO			3

    #define HUD_CHN_BROADCAST			4
        #define BROADCAST_HUD_MAX_AMOUNT	6
        #define BROADCAST_HUD_MAX_LENGTH	128

    //Message
    stock HUMAN_COMMAND_HUD[] = " /bp 背包 | /equip 装備 | /wp 武器 | /help 教學 | /trade 交易 \n/sp 技能點 | /throw丟棄物品 | /stuck解除卡住 | /setting各種設定\n F 快速使用A裝備";
    stock ZOMBIE_COMMAND_HUD[] = " /bp 背包 | /zombie 喪屍種類 | /wp 輔助技能 | /help 教學\n/throw丟棄物品 | /setting各種設定";

    stock GAME_STATUS_HUD[256];

    #define TASKTIME_SERVER_HUD 4.0
    #define TASKTIME_PLAYER_HUD 2.0
    #define TASKID_PLAYER_HUD_BASE	8000
    #define TASKID_PLAYER_HUD(%0)	(8000+%0)
    #define TASKID_SERVER_GAME_PAUSE_HUD		8501

    stock GAME_END_MSG[128];

/*  Custom Files, Model, Sound Variables  */

    #define CATEGORY_MODEL			1
    #define CATEGORY_ARM			2
    #define CATEGORY_SOUND			3

    #define TOTAL_PLAYER_MODEL		64
    stock LOADED_MODEL;
    stock NAME_OF_PLAYER_MODEL[TOTAL_PLAYER_MODEL][32];
    stock PLAYER_MODEL_PATH[TOTAL_PLAYER_MODEL][256];
    stock PLAYER_MODEL_PRICE[TOTAL_PLAYER_MODEL];
    stock MODEL_ENABLE[TOTAL_PLAYER_MODEL];
    stock giPlyModelId[MAX_PLAYERS];

    #define TOTAL_ARM_MODEL		64
    stock NAME_OF_ARM_MODEL[TOTAL_ARM_MODEL][32];
    stock ARM_MODEL_PATH[TOTAL_ARM_MODEL][256];
    stock ARM_MODEL_PRICE[TOTAL_ARM_MODEL];
    stock ARM_MODEL_ENABLE[TOTAL_ARM_MODEL];
    stock giPlyArmModelId[MAX_PLAYERS];

    #define TOTAL_VOICE	16
    #define MAX_VOICE_FOR_EACH_SIT	2
    #define TOTAL_VOICE_SITUATION	5
        #define VOICE_SIT_PREPARE		0	
        #define VOICE_SIT_HURT			1
        #define VOICE_SIT_REAL_HURT		2
        #define VOICE_SIT_CHEER			3
        #define VOICE_SIT_SUPPLY		4

    stock LOADED_VOICE;
    stock NAME_OF_VOICE[TOTAL_VOICE][32];

    stock Float:VOICE_PATH_ID[TOTAL_VOICE][TOTAL_VOICE_SITUATION][MAX_VOICE_FOR_EACH_SIT];
    stock VOICE_PATH[TOTAL_VOICE*MAX_VOICE_FOR_EACH_SIT*TOTAL_VOICE_SITUATION];
    //stock VOICE_PATH[TOTAL_VOICE][TOTAL_VOICE_SITUATION][MAX_VOICE_FOR_EACH_SIT][64];
    stock Float:VOICE_COOLDOWN[TOTAL_VOICE][TOTAL_VOICE_SITUATION][MAX_VOICE_FOR_EACH_SIT];
    stock Float:VOICE_VOLUME[TOTAL_VOICE][TOTAL_VOICE_SITUATION][MAX_VOICE_FOR_EACH_SIT];
    stock VOICE_PRICE[TOTAL_VOICE];
    
    stock giPlyVoiceId[MAX_PLAYERS];
    stock Float:gfPlyVoiceCooldown[MAX_PLAYERS];
    
    stock ZOMBIE_ARM_MODEL_ID;	

    stock gbsPlyUnlockedModel[MAX_PLAYERS][1+(TOTAL_PLAYER_MODEL/32)];
        stock bool:IsPlayerModelUnlocked(client, model) { return (gbsPlyUnlockedModel[client][model/32]&(1<<(model%32)))!=0; } 
        stock UnlockPlayerModel(client, model, bool:dontBroadcast=false)
        {
            if(IsClientValid(client)&&giPlyID[client]!=0 && 0<=model<TOTAL_PLAYER_MODEL)
            {
                gbsPlyUnlockedModel[client][model/32] |= (1<<(model%32));

                if(!dontBroadcast)
                    PrintToChat(client, " \x07人物模組%s已解鎖", NAME_OF_PLAYER_MODEL[model]);
            }
        }
    stock gbsPlayerUnlockedArm[MAX_PLAYERS][1+(TOTAL_ARM_MODEL/32)];
        stock bool:IsPlayerArmUnlocked(client, arm) { return (gbsPlayerUnlockedArm[client][arm/32]&(1<<(arm%32)))!=0; } 
        stock UnlockPlayerArm(client, arm, bool:dontBroadcast=false)
        {
            if(IsClientValid(client)&&giPlyID[client]!=0 && 0<=arm<TOTAL_ARM_MODEL)
            {
                gbsPlayerUnlockedArm[client][arm/32] |= (1<<(arm%32));
                
                if(!dontBroadcast)
                    PrintToChat(client, " \x07手部模組%s已解鎖", NAME_OF_ARM_MODEL[arm]);
            }
        }

    stock gbsPlayerUnlockedVoice[MAX_PLAYERS][1+(TOTAL_VOICE/32)];
        stock bool:IsPlayerVoiceUnlocked(client, sound) { return (gbsPlayerUnlockedVoice[client][sound/32]&(1<<(sound%32)))!=0; } 
        stock UnlockPlayerVoice(client, sound)
        {
            if(IsClientValid(client)&&giPlyID[client]!=0 && 0<=sound<TOTAL_VOICE)
            {
                gbsPlayerUnlockedVoice[client][sound/32] |= (1<<(sound%32));
                PrintToChat(client, " \x07語音%s已解鎖", NAME_OF_VOICE[sound]);
            }
        }

/*  Camera Variables  */

    stock giPlyCameraMode[MAX_PLAYERS];
    stock giPlyCameraEntId[MAX_PLAYERS] = {0, ...};
    stock bool:gbCameraOn[MAX_PLAYERS] = {false, ...};

/*  NPC Variables  */

    #define TOTAL_NPC	1+14
        #define NPC_MINI_CM			1
        #define NPC_MODEL_SHOP		2
        #define NPC_ARM_SHOP		3
        #define NPC_VOICE_SHOP		4
        #define NPC_WAREHOUSE		5
        #define NPC_CRAFT1			6
        #define NPC_PLAYER_INFO		7
        #define NPC_STORE			8
        #define NPC_MCDONALD		9
        #define NPC_DRINK_SHOP1 	10
        #define NPC_CD_SHOP		 	11
        #define NPC_EXPENSIVE_SHOP 	12
        #define NPC_COFFEE_SHOP 	13
        #define NPC_EXPENSIVE_CRAFT	14

    #define TOTAL_MINI_CM_SHOP	
        #define MINI_CM_PROFILE		1
        #define MINI_CM_MISSION		2
        #define MINI_CM_BP_TO_WH	3
        #define MINI_CM_WAREHOUSE	4	
        #define MINI_CM_MINISHOP	5
        #define MINI_CM_CRAFT		6

    stock NAME_OF_NPC[TOTAL_NPC][] =
    {
        "",
        "迷你Citymall",	//1			
        "人物模組商店",	//2	
        "手部模組商店",	//3	
        "人物聲音商店",	//4	
        "銀行",			//5
        "合成",			//6
        "玩家資訊",		//7
        "雜物房",		//8
        "快餐店",		//9
        "飲料商店",		//10
        "CD舗",			//11
        "高級商店",		//12
        "Cafe",			//13
        "高級合成商店"	//14	
    };
    stock SHOP_MODEL_ID;

    #define MAX_CRAFT_ITEM		8
        #define MAX_ITEM_NEEDED_FOR_CRAFT	5

        stock CRAFT_ITEM_ID[MAX_CRAFT_ITEM];
        stock CRAFT_ITEM_PRICE[MAX_CRAFT_ITEM];
        stock CRAFT_ITEM_LIST[MAX_CRAFT_ITEM][MAX_ITEM_NEEDED_FOR_CRAFT];

    #define TOTAL_ITEM_SHOP		1+6
    #define MAX_SHOP_ITEM 		8
        #define SHOP_ID_MINICM_SHOP		1
        #define SHOP_ID_MCDONALD		2
        #define SHOP_ID_DRINK_SHOP_1	3
        #define SHOP_ID_CD_SHOP			4
        #define SHOP_ID_EXPENSIVE_SHOP	5
        #define SHOP_ID_COFFEE_SHOP		6


    stock NAME_OF_SHOP[TOTAL_ITEM_SHOP][] = {
        "",
        "",
        "快餐店",
        "飲料店",
        "CD店",
        "黑市商人",
        "咖啡店"
    };
    stock SHOP_ITEM_COUNTER[TOTAL_ITEM_SHOP];
    stock SHOP_ITEM_ID[TOTAL_ITEM_SHOP][MAX_SHOP_ITEM];
    stock SHOP_ITEM_PRICE[TOTAL_ITEM_SHOP][MAX_SHOP_ITEM];
    stock SHOP_ITEM_FLAG[TOTAL_ITEM_SHOP][MAX_SHOP_ITEM];

/*	Mission Variables  */

    #define MISSION_BONUS_CASH	2500
    #define TOTAL_MISSION		128
    stock MISSION_COMPLETE_SOUND[] = "tsubaki/old_kanobi/levelup.mp3";
    #define EMIT_MISSION_COMPLETE_SOUND(%0, %1) emit_sound(%0, CHAN_BODY, MISSION_COMPLETE_SOUND, %1, ATTN_NONE, 0, PITCH_NORM)
    stock SORTED_MISSION[TOTAL_MISSION];

    stock gbsPlyFinishedMission[MAX_PLAYERS][1+(TOTAL_MISSION/32)];
        stock bool:IsPlayerMissionUnlocked(client, mission_id) { return (gbsPlyFinishedMission[client][mission_id/32]&(1<<(mission_id%32)))!=0;}
        stock UnlockPlayerMission(client, mission_id, bool:dontBroadcast=false, bool:play_sound=false)
        {
            if(!IsFakeClient(client) && !IsPlayerMissionUnlocked(client, mission_id))
            {
                gbsPlyFinishedMission[client][mission_id/32] |= (1<<(mission_id%32));
                GivePlayerCash(client, MISSION_BONUS_CASH);
                LevelUp(client, 1);

                new mission[64];
                GetMissionName(mission_id, mission, sizeof(mission));
                
                if(!dontBroadcast)
                {
                    client_print(0, print_chat, " \x04%N %s", client, mission);
                    
                    if(play_sound)
                        EMIT_MISSION_COMPLETE_SOUND(client, 1.0);
                }
                else
                    client_print(client, print_chat, " \x04%N %s", client, mission);
            }
        }


    stock bool:GetMissionName(mission_id, output[], size)
    {
        output[0] = 0;
        switch(mission_id)
        {
            case  0:formatex(output, size, "テスト");
            case  1:formatex(output, size, "第一次進入此伺服器");
            case  2:formatex(output, size, "第一次使用Mini Citymall");
            case  3:formatex(output, size, "第一次重生成為人類");
            case  4:formatex(output, size, "第一次進行BOSS戰");
            case  5:formatex(output, size, "第一次進入BOSS FINAL");
            case  6:formatex(output, size, "第一次BOSS FINAL後生存");
            case  7:formatex(output, size, "第一次FINAL ALPHA後生存");
            case  8:formatex(output, size, "第一次FINAL DELTA後生存");
            case  9:formatex(output, size, "第一次FINAL GAMMA後生存");
            case 10:formatex(output, size, "第一次FINAL LAMBDA後生存");
            case 11:formatex(output, size, "第一次在人類VS喪屍模式生存");
            case 12:formatex(output, size, "第一次在BOSS戰後生存");
            case 13:formatex(output, size, "第一次在VS BOSS模式生存");
            case 14:formatex(output, size, "FINAL 200S PROTOTYPE I 中生存50秒");
            case 15:formatex(output, size, "FINAL 200S PROTOTYPE I 中生存100秒");
            case 16:formatex(output, size, "FINAL 200S PROTOTYPE I 中生存150秒");
            case 17:formatex(output, size, "完成FINAL 200S PROTOTYPE I");
            case 18:formatex(output, size, "成為BR勝利者一次");
            case 19:formatex(output, size, "以隊長身分勝利一次");
            case 20:formatex(output, size, "以BOSS身分勝利一次");
            case 21:formatex(output, size, "解除一次大爆炸");
            case 22:formatex(output, size, "裝備真B1情況下勝利一次");
            case 23:formatex(output, size, "裝備真B2情況下勝利一次");
            case 24:formatex(output, size, "裝備真B3情況下勝利一次");
            case 25:formatex(output, size, "成為最後一位人類");
            case 26:formatex(output, size, "1HP 下存活");
            case 27:formatex(output, size, "殺死一隻喪屍");
            case 28:formatex(output, size, "一局內殺死五隻喪屍");
            case 29:formatex(output, size, "一局內殺死十五隻喪屍");
            case 30:formatex(output, size, "殺夠了未？");
            case 31:formatex(output, size, "殺死人類隊長");
            case 32:formatex(output, size, "對BOSS進行最後一擊");
            case 33:formatex(output, size, "殺死1位人類");
            case 34:formatex(output, size, "殺死2位人類");
            case 35:formatex(output, size, "殺死3位人類");
            case 36:formatex(output, size, "殺死5位人類");
            case 37:formatex(output, size, "殺死10位人類");
            case 38:formatex(output, size, "20位= =?");
            case 39:formatex(output, size, "裝備真B4情況下勝利一次");
            case 40:formatex(output, size, "未開放");
            case 41:formatex(output, size, "刀死1隻喪屍");
            case 42:formatex(output, size, "刀死3隻喪屍");
            case 43:formatex(output, size, "刀死5隻喪屍");
            case 44:formatex(output, size, "刀死10隻喪屍");
            case 45:formatex(output, size, "不入虎穴，焉得虎子");
            case 46:formatex(output, size, "第一次合成物品");
            case 47:formatex(output, size, "這個世界其實很美好");
            case 48:formatex(output, size, "你的性命對BOSS 來說不值一提");
            case 49:formatex(output, size, "神風敢死隊");
            case 50:formatex(output, size, "爆了1個門");
            case 51:formatex(output, size, "爆了2個門");
            case 52:formatex(output, size, "爆了3個門");
            case 53:formatex(output, size, "爆了6個門");
            case 54:formatex(output, size, "這地圖為什麼有這麼多門...");
            case 55:formatex(output, size, "第一次以爆門方式重生");
            case 56:formatex(output, size, "裝備真B5情況下勝利一次");
            case 57:formatex(output, size, "我愛Costco!");
            case 58:formatex(output, size, "我是傳奇");
            case 59:formatex(output, size, "第一次以人類身份觸發全地圖系攻擊");
            case 60:formatex(output, size, "TRICK");
            case 61:formatex(output, size, "TREAT");
            case 62:formatex(output, size, "以一次自殺式攻擊對1隻喪屍進行攻擊");
            case 63:formatex(output, size, "以一次自殺式攻擊對5隻喪屍進行攻擊");
            case 64:formatex(output, size, "以一次自殺式攻擊對10隻喪屍進攻擊");
            case 65:formatex(output, size, "核彈");
            case 66:formatex(output, size, "以一次自殺式攻擊殺死1隻喪屍");
            case 67:formatex(output, size, "以一次自殺式攻擊殺死5隻喪屍");
            case 68:formatex(output, size, "以一次自殺式攻擊殺死12隻喪屍");
            case 69:formatex(output, size, "未能阻止BOSS將毒素散佈在空氣中");
            case 70:formatex(output, size, "成功將毒素散佈到空氣中");
            //case :format(output, size, "");
            default:return false;
        }
        return true;
    }

/*  Mentor System  */
    
    stock giPlyMentor[MAX_PLAYERS];


/*  Task ID  */

    //#define TASKID_GAME_STATUS    101
    //#define TASKID_BGM            105
    //#define TASKID_SPACE_COMPRESSION        501
    //#define TASKID_SPACE_COMPRESSION_HUD    502
    //#define TASKID_PLAYER_RESPAWN(%0)   		(3000+%0)
    //#define TASKID_CALCULATE_FINAL_ABILITY  	(4000+%0)
    //#define TASKID_PLAYER_WEAPON_CHECK(%0)  	(5000+%0)
    //#define TASKID_PLAYER_WEAPON_SUPPLY(%0) 	(6000+%0)
    //#define TASKID_AUTO_ACTIVE_SKILL(%0)		(7000+%0)
    //#define TASKID_PLAYER_HUD(%0)				(8000+%0)
    //#define TASKID_SERVER_GAME_PAUSE_HUD		8501
    //#define TASKID_ENTITY_REMOVE(%0)			(9000+%0)
    //#define TASKID_ENTITY_TASK_BASE           11000
    //#define TASKID_ENTITY_TASK(%0)            (11000+%0)
    //#define TASKID_ITEM_REMOVE_BASE       1000000
    //#define TASKID_ITEM_EFFECT_BASE       2000000
    //#define TASKID_ABNORMAL_EFFECT_BASE   3000000
    //#define TASKID_ABNORMAL_REMOVE_BASE   4000000
    //#define TASKID_EQUIP_EFFECT_BASE      5000000



/*  Game Event  */

    stock RegisterTsubakiEvents() {
        register_forward(FM_Spawn, "EntitySpawnForward");

        BuildAvailiableWeaponIds();
    }

    public EntitySpawnForward(ent) 
    {
        if(!pev_valid(ent))
            return FMRES_IGNORED;
        
        static clsname[32], i;
        entity_get_string(ent, EV_SZ_classname, clsname, 32);

        for(i=0; i<sizeof(BLOCKED_ENTITY_NAME); i++) {
            if(equal(clsname, BLOCKED_ENTITY_NAME[i])) {
                engfunc(EngFunc_RemoveEntity, ent);
                return FMRES_SUPERCEDE;
            }
        }

        #if defined DEBUG
            static bool:write=false, entity_list_log[256], map_name[32], msg[256];
            if(entity_list_log[0] == 0)
            {
                get_mapname(map_name, 32);

                get_basedir(entity_list_log, 256);
                format(entity_list_log, 256, "%s/logs/research/%s_spawned_entity.log", entity_list_log, map_name);

                write = !(file_exists(entity_list_log));

                //console_print(0, "Research Data : %s", entity_list_log);
            }

            if(write)
            {
                formatex(msg, 256, "%d.%s", ent, clsname);
                write_file(entity_list_log, msg, -1);
            }
        #endif

        return FMRES_IGNORED;
    }

/*  Player Event  */

    TsubakiRegisterPlayerEvent(client) {

        #if defined DEBUG
            server_print("Register Player %d Event", client);
        #endif

        switch(giCurRoundMode)
        {
            // //TO-DO VS Boss Events
            // case GAMEMODE_HVZ, GAMEMODE_VB:{
            // 	if( g_hTraceAttack ) DisableHamForward(g_hTraceAttack);
            // 	g_hTraceAttack = RegisterHamPlayer(Ham_TraceAttack, "HVZOnPlayerTraceAttack");

            // 	if( g_hTakeDamage ) DisableHamForward(g_hTakeDamage);
            // 	g_hTakeDamage = RegisterHamPlayer(Ham_TakeDamage, "HVZOnPlayerTakeDamage");		//No Post in TakeDamage

            // 	if( g_hKilled ) DisableHamForward(g_hKilled);
            // 	g_hKilled = RegisterHam(Ham_Killed, PLAYER, "HVZOnPlayerKilled", 1);
            // }
            // case GAMEMODE_BR: {
            // 	if( g_hTraceAttack ) DisableHamForward(g_hTraceAttack);
            // 	g_hTraceAttack = RegisterHamPlayer(Ham_TraceAttack, "BROnPlayerTraceAttack");

            // 	if( g_hTakeDamage ) DisableHamForward(g_hTakeDamage);
            // 	g_hTakeDamage = RegisterHamPlayer(Ham_TakeDamage, "BROnPlayerTakeDamage");		//No Post in TakeDamage

            // 	if( g_hKilled ) DisableHamForward(g_hKilled);
            // 	g_hKilled = RegisterHam(Ham_Killed, PLAYER, "BROnPlayerKilled", 1);
            // }

        }
    }

    public HVZPlayerTraceAttack(victim, attacker, Float:damage, Float:direction[3], traceresult, damagetype) {
        damagetype |= (get_tr2(traceresult, TR_iHitgroup) == HIT_HEAD) ? CS_DMG_HEADSHOT : 0;

        SetHamParamInteger(6, damagetype);
        return HAM_OVERRIDE;

    }

    public HVZOnPlayerTakeDamage(victim, weapon, attacker, Float:damage, damagetype) {

    }
