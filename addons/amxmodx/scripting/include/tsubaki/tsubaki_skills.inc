#if defined _tsubaki_skills
    #endinput
#endif
#define _tsubaki_skills

#include <tsubaki/tsubaki_method_lib>

#pragma semicolon 1

/*  Skill Variables */

    #define TOTAL_BOSS_PASSIVE_SKILL_FOR_VB	8
    #define TOTAL_BOSS_PASSIVE_SKILL		TOTAL_BOSS_PASSIVE_SKILL_FOR_VB+2
    #if defined DEBUG
        #define MAX_BOSS_SKILL	5
    #else
        #define MAX_BOSS_SKILL	25
    #endif
    stock BOSS_SKILL_COUNTER;
    #define BOSS_TOXIC_ID	999


    #define POINTING_LASER_REFRESH_RATE 0.1
    #define POINTING_LASER_DMG 3.0

    stock Float:gfPlyActiveSkillCooldown[MAX_PLAYERS];
    stock Float:gfPlyUseKeySkillCooldown[MAX_PLAYERS];

    stock SKILL_DISPLAY_FMT_IN_CD[] = "技能:%s (冷卻中 [%.0f秒])";
    stock SKILL_DISPLAY_FMT_USABLE[] = "技能:%s (可使用)";

    #define DEFAULT_FREEZE_BULLET_TIME 1.5
    stock bool:gbPlyNextFireFreeze[MAX_PLAYERS];

    #define BIG_BANG_RANGE 1250.0
    
    #define DMG_AREA_NOTHING	0
    #define DMG_AREA_FREEZE		1
    #define DMG_AREA_FIRE		2

/*  Tsubaki Entity  */

/*  Bullet Skills  */

    new const NORMAL_BULLET[] = "Normal_Bullet";
    new const AVERAGE_CHANGE_SPEED_BULLET[] = "Average_Change_Speed_Bullet";
    new const MOVE_AND_SPIN_ONLY_BULLET[] = "Move_And_Spin_Only_Bullet";
    new const SPIN_AWAY_BULLET[] = "Spin_Away_Bullet";
    new const SPIN_AWAY_SPLIT_BULLET[] = "Spin_Away_Split_Bullet";
    new const RANDOM_MACHINE_BULLET_LAUNCHER[] = "Random_Machine_Bullet_Launcher";
    new const ROUND_SPIN_BULLET[] = "Round_Spin_Bullet";
    new const CHANGE_DEST_BULLET[] = "Change_Dest_Bullet";
    new const CHANGE_ANGLE_BULLET[] = "Change_Angle_Bullet";
    new const ROTATE_MACHINE_BULLET_LAUNCHER[] = "Rotate_Machine_Bullet_Launcher";
    new const SPLIT_STRAIGHT_BULLET[] = "Split_Straight_Bullet";
    new const APPEAR_AND_DISAPPER_BULLET[] = "Appear_And_Disapper_Bullet";
    new const NON_PENETRATE_BULLET[] = "Non_Penetrate_Bullet";
    new const SPLIT_BACKWARD_BULLET[] = "Split_Backward_Bullet";
    new const SLOW_CHANGE_DIR_BULLET_LAUNCHER[] = "Slow_Change_Dir_Bullet_Launcher";
    new const SLOW_CHANGE_DIR_BULLET[] = "Slow_Change_Dir_Bullet";
    new const SWIRL_BULLET[] = "Swirl_Bullet";
    new const TRACER_BULLET[] = "Tracer_Bullet";
    new const SNAKE_PROTECT_BULLET[] = "Snake_Protect_Bullet";
    new const SNAKE_PROTECT_CONTROLLER[] = "Snake_Protect_Controller";
    
    new const EXPLOSIVE_BULLET[] = "Explosive_Bullet";
    #define EXPLOSIVE_BULLET_THINKTIME 0.25

    RegisterTsubakiEntityStuffs() {
        register_touch(NORMAL_BULLET, PLAYER, "NormalBulletTouch");

        register_touch(AVERAGE_CHANGE_SPEED_BULLET, PLAYER, "NormalBulletTouch");
        register_think(AVERAGE_CHANGE_SPEED_BULLET, "AverageChangeSpeedBulletThink");

        register_touch(MOVE_AND_SPIN_ONLY_BULLET, PLAYER, "NormalBulletTouch");
        register_think(MOVE_AND_SPIN_ONLY_BULLET, "MoveAndSpinOnlyBulletThink");

        register_touch(SPIN_AWAY_BULLET, PLAYER, "NormalBulletTouch");
        register_think(SPIN_AWAY_BULLET, "SpinAwayBulletThink");
        
        register_touch(SPIN_AWAY_SPLIT_BULLET, PLAYER, "NormalBulletTouch");
        register_think(SPIN_AWAY_SPLIT_BULLET, "SpinAwaySplitBulletThink");
        
        register_think(RANDOM_MACHINE_BULLET_LAUNCHER, "AllRandomMachineBulletLaunch");

        register_touch(ROUND_SPIN_BULLET, PLAYER, "NormalBulletTouch");
        register_think(ROUND_SPIN_BULLET, "RoundSpinBulletThink");

        register_touch(CHANGE_DEST_BULLET, PLAYER, "NormalBulletTouch");
        register_think(CHANGE_DEST_BULLET, "ChangeDestBulletThink");

        register_touch(CHANGE_ANGLE_BULLET, PLAYER, "NormalBulletTouch");
        register_think(CHANGE_ANGLE_BULLET, "ChangeAngleBulletThink");

        register_think(ROTATE_MACHINE_BULLET_LAUNCHER, "RotateMachineBulletLauncherThink");

        register_touch(SPLIT_STRAIGHT_BULLET, PLAYER, "NormalBulletTouch");
        register_think(SPLIT_STRAIGHT_BULLET, "SplitStraightBulletThink");

        register_touch(APPEAR_AND_DISAPPER_BULLET, PLAYER, "NormalBulletTouch");
        register_think(APPEAR_AND_DISAPPER_BULLET, "AppearAndDisapperBulletThink");

        register_touch(NON_PENETRATE_BULLET, "*", "NormalBulletTouch");

        register_touch(SPLIT_BACKWARD_BULLET, PLAYER, "NormalBulletTouch");
        register_think(SPLIT_BACKWARD_BULLET, "SplitBackwardBulletThink");

        register_think(SLOW_CHANGE_DIR_BULLET_LAUNCHER, "SlowChangeDirBulletLauncherThink");

        register_touch(SLOW_CHANGE_DIR_BULLET, PLAYER, "NormalBulletTouch");
        register_think(SLOW_CHANGE_DIR_BULLET, "SlowChangeDirBulletThink");
        
        register_touch(EXPLOSIVE_BULLET, PLAYER, "OnExplosiveBulletTouch");
        register_think(EXPLOSIVE_BULLET, "ExplosiveBulletThink");

        register_touch(SWIRL_BULLET, PLAYER, "NormalBulletTouch");
        register_think(SWIRL_BULLET, "SwirlBulletThink");

        register_touch(TRACER_BULLET, PLAYER, "NormalBulletTouch");
        register_think(TRACER_BULLET, "TracerBulletThink");

        register_think(SNAKE_PROTECT_CONTROLLER, "SnakeProtectControllerThink");
        register_touch(SNAKE_PROTECT_BULLET, PLAYER, "SnakeProtectBulletTouch");

        //register_touch(, PLAYER, "NormalBulletTouch");
        //register_think(, "");
    }

    public NormalBulletTouch(ent, client) {
        static owner;

        if((1<=client<=MaxClients) && (owner=GetEntityOwner(ent))!=client && (owner==0 || cs_get_user_team(client)!=cs_get_user_team(owner))) {
            ExecuteHamB(Ham_TakeDamage, client, 0, owner, entity_damage[ent], BULLET_DMG_TYPE);

            RemoveBullet(ent);
        }
    }

    public NonPenetrateBulletTouch(ent, client) {
        static owner;

        if((1<=client<=MaxClients) && (owner=GetEntityOwner(ent))!=client && (owner==0 || cs_get_user_team(client)!=cs_get_user_team(owner))) {
            ExecuteHamB(Ham_TakeDamage, client, 0, owner, entity_damage[ent], BULLET_DMG_TYPE);
        }
        
        RemoveBullet(ent);
    }

/*  Bullet Pattern  */

    stock AverageBullet(Float:origin[3], bullet_amount=8, owner=0, 
        Float:speed=100.0, Float:last_time=4.0, Float:angle_offset=0.0, colors[4]={200, 0, 0, 16})
    {
        if(bullet_amount <= 0)
            return;

        new ent, i;
        new Float:velocity[3];
        new Float:spawn_origin[3];
        //Float:velocity[3], angles[3], spawn_origin[3];
        angle_offset *= M_PI / 180.0;
        bullet_amount = (bullet_amount <= 64)?bullet_amount:64;

        for(i=0; i<bullet_amount; i++)
        {
            ent = CreateBullet(NORMAL_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=colors[0], .g=colors[1], .b=colors[2]);
            
            if(ent != -1)
            {
                velocity[0] = speed * Cosine(angle_offset);
                velocity[1] = speed *   Sine(angle_offset);

                spawn_origin[0] = origin[0] + BULLET_SIZE * Cosine(angle_offset);
                spawn_origin[1] = origin[1] + BULLET_SIZE *   Sine(angle_offset);
                spawn_origin[2] = origin[2];

                //TeleportEntity(ent, spawn_origin, NULL_VECTOR, velocity);
                TeleportEntity(ent, spawn_origin);
                SetEntityVelocity(ent, velocity);

                SetRemoveEntityTask(ent, last_time);
            }

            angle_offset += M_2PI / bullet_amount;
        }
    }

    stock AverageChangeSpeedBullet(Float:origin[3], owner, bullet_amount=8,
                                        Float:spd1=50.0, Float:spd2=300.0, Float:angle_offset=0.0,
                                        Float:delay=2.0, Float:last_time=7.0,
                                        colors[4]={0,0,255,16}, Float:damage=DEFAULT_BULLET_DMG)
    {
        new ent, i=0;
        new Float:velocity[3] = {0.0, ...};
        angle_offset = ToRadian(angle_offset);
        RefreshGlobalGametime();

        for(; i<bullet_amount; i++)
        {
            ent = CreateBullet(AVERAGE_CHANGE_SPEED_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=colors[0], .g=colors[1], .b=colors[2], .a=colors[3], .damage=damage);

            velocity[0] = spd1 * Cosine(angle_offset);
            velocity[1] = spd1 *   Sine(angle_offset);

            vuser[ent][0][0] = spd2 * Cosine(angle_offset);
            vuser[ent][0][1] = spd2 *   Sine(angle_offset);
            vuser[ent][0][2] = 0.0;


            TeleportEntity(ent, origin);
            SetEntityVelocity(ent, velocity);

            entity_set_float(ent, EV_FL_nextthink, GAMETIME+delay);
            SetRemoveEntityTask(ent, last_time);

            angle_offset += M_2PI/bullet_amount;
        }
        
    }								

        public AverageChangeSpeedBulletThink(ent) {
            SetEntityVelocity(ent, vuser[ent][0]);
        }

    //Please pass angle and angular_spd in degree
    //Please pass last time after time to radius
    stock MoveAndSpinOnlyBullet(Float:origin[3], owner, bullet_amount=8,
                                    Float:target_radius=100.0, Float:angle_offset=0.0, Float:angular_spd=45.0,
                                    Float:time_to_radius=1.0, Float:last_time=8.0, 
                                    colors[4]={30,144,255,16} )
    {
        new i=0, ent;
        new task_function[] = "MoveAndSpinOnlyBulletPrepare";
        target_radius /= time_to_radius;		//target_radius become spd
        angle_offset = ToRadian(angle_offset);
        angular_spd = ToRadian(angular_spd);

        for(; i<bullet_amount; i++)
        {
            if( (ent=CreateBullet(MOVE_AND_SPIN_ONLY_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=colors[0], .g=colors[1], .b=colors[2], .a=colors[0])) == -1 )
                continue;

            vuser[ent][0][0] = target_radius * Cosine(angle_offset);
            vuser[ent][0][1] = target_radius *   Sine(angle_offset);
            vuser[ent][0][2] = 0.0;

            fuser[ent][0] = angle_offset + M_PI/2.0;		//current_angle
            fuser[ent][1] = angular_spd;			//angular_spd
            fuser[ent][2] = target_radius;		//spd

            TeleportEntity(ent, origin);
            SetEntityVelocity(ent, vuser[ent][0]);

            set_task_ex(time_to_radius-DEFAULT_BULLET_THINKTIME, task_function, TASKID_ENTITY_TASK(ent), _, _, SetTask_Once);
            SetRemoveEntityTask(ent, last_time);

            angle_offset += M_2PI / bullet_amount;
        }
    }

        public MoveAndSpinOnlyBulletPrepare(taskid)
        {
            SetEntityNextThink(taskid-TASKID_ENTITY_TASK_BASE, get_gametime()+DEFAULT_BULLET_THINKTIME);
        }

        public MoveAndSpinOnlyBulletThink(ent)
        {
            vuser[ent][0][0] = fuser[ent][2] * fuser[ent][1] * Cosine(fuser[ent][0]);
            vuser[ent][0][1] = fuser[ent][2] * fuser[ent][1] *   Sine(fuser[ent][0]);

            fuser[ent][0] += fuser[ent][1] * DEFAULT_BULLET_THINKTIME;
            SetEntityNextThink(ent, get_gametime() + DEFAULT_BULLET_THINKTIME);
            SetEntityVelocity(ent, vuser[ent][0]);
        }

    //旋回遠離
    stock SpinAwayBullet(Float:origin[3], owner=0, bullet_amount=8, 
                            Float:initial_radius=50.0, Float:radius_increase_per_second=50.0,
                            Float:angular_spd=30.0, Float:angle_offset=0.0, Float:last_time=8.0,
                            colors[4]={64,224,208,16})
    {
        new i=0, ent;
        new Float:velocity[3];

        angle_offset = ToRadian(angle_offset);
        angular_spd = ToRadian(angular_spd);
        radius_increase_per_second *= DEFAULT_BULLET_THINKTIME;

        RefreshGlobalGametime();

        for(; i<bullet_amount; i++)
        {
            if((ent=CreateBullet(SPIN_AWAY_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=colors[0], .g=colors[1], .b=colors[2], .a=colors[0])) == -1 )
                continue;
                
            velocity[0] = initial_radius * angular_spd * Cosine(angle_offset);
            velocity[1] = initial_radius * angular_spd *   Sine(angle_offset);

            fuser[ent][0] = initial_radius;
            fuser[ent][1] = radius_increase_per_second;		//already *= bullet_thinktime
            fuser[ent][2] = angle_offset;
            fuser[ent][3] = angular_spd;

            TeleportEntity(ent, origin);
            SetEntityVelocity(ent, velocity);
            SetEntityNextThink(ent, GAMETIME+DEFAULT_BULLET_THINKTIME);
            SetRemoveEntityTask(ent, last_time);

            angle_offset += M_2PI / bullet_amount;
        }
    }

        public SpinAwayBulletThink(ent)
        {
            fuser[ent][0] += fuser[ent][1];
            fuser[ent][2] += fuser[ent][3] * DEFAULT_BULLET_THINKTIME;

            vuser[ent][0][0] = fuser[ent][0] * fuser[ent][3] * Cosine(fuser[ent][2]);
            vuser[ent][0][1] = fuser[ent][0] * fuser[ent][3] *   Sine(fuser[ent][2]);
            vuser[ent][0][2] = 0.0;
            
            SetEntityVelocity(ent, vuser[ent][0]);
            SetEntityNextThink(ent, get_gametime()+DEFAULT_BULLET_THINKTIME);
        }

    //旋回遠離、Normal 分裂
    //Please Pass angle in degree
    stock CreateSpinAwaySplitBullet(Float:origin[3], owner=0, bullet_amount=3, 
                                    Float:initial_radius=10.0, Float:radius_increase_per_second=100.0, 
                                    Float:angle_offset=0.0, Float:angular_spd=45.0, Float:last_time=12.0,
                                    split_amount=1, Float:split_angle=-90.0, Float:split_bullet_spd=150.0, 
                                    Float:split_bullet_last_time=7.0, Float:split_interval=1.0, 
                                    colors[4]={153,0,153,16}, split_colors[4]={153,0,153,16})
    {
        new i=0, ent, rgba = CreateRGBAInt(split_colors[0], split_colors[1], split_colors[2], split_colors[3]);
        new Float:spawn_origin[3], Float:velocity[3], Float:split_bullet_data[3];

        angular_spd = ToRadian(angular_spd);
        angle_offset = ToRadian(angle_offset);
        radius_increase_per_second *= DEFAULT_BULLET_THINKTIME;

        split_bullet_data[0] = ToRadian(split_angle);
        split_bullet_data[1] = split_bullet_spd;
        split_bullet_data[2] = split_bullet_last_time;

        for(i=0; i<bullet_amount; i++)
        {
            if((ent=CreateBullet(SPIN_AWAY_SPLIT_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=colors[0], .g=colors[1], .b=colors[2], .a=colors[3])) == -1 )
                continue;

            spawn_origin[0] = origin[0] + initial_radius * Cosine(angle_offset);
            spawn_origin[1] = origin[1] + initial_radius *   Sine(angle_offset);
            spawn_origin[2] = origin[2];

            velocity[0] = initial_radius * angular_spd * Cosine(angle_offset);
            velocity[1] = initial_radius * angular_spd *   Sine(angle_offset);

            iuser[ent][0] = split_amount;
            iuser[ent][1] = rgba;
            iuser[ent][2] = 0;          //End Counter
            iuser[ent][3] = RoundFloat(split_interval/DEFAULT_BULLET_THINKTIME);

            fuser[ent][0] = initial_radius;
            fuser[ent][1] = radius_increase_per_second;		//Already *= thinktime
            fuser[ent][2] = angle_offset;
            fuser[ent][3] = angular_spd;

            CopyVector(vuser[ent][0], split_bullet_data);

            TeleportEntity(ent, spawn_origin);
            SetEntityVelocity(ent, velocity);

            SetEntityNextThink(ent, DEFAULT_BULLET_THINKTIME);
            SetRemoveEntityTask(ent, last_time);

            angle_offset += M_2PI / bullet_amount;
        }
    }

        public SpinAwaySplitBulletThink(ent)
        {
            fuser[ent][0] += fuser[ent][1];
            fuser[ent][2] += fuser[ent][3]*DEFAULT_BULLET_THINKTIME;

            if((++iuser[ent][2])%iuser[ent][3] == 0)
            {
                new sub_ent, i=0, owner=GetEntityOwner(ent), amount=iuser[ent][0];
                new r = GetRGB_R(iuser[ent][1]), g = GetRGB_G(iuser[ent][1]), b = GetRGB_B(iuser[ent][1]), a = GetRGBA_A(iuser[ent][1]);
                new Float:origin[3], Float:velocity[3], Float:angle = fuser[ent][2]+vuser[ent][0][0] , Float:spd = vuser[ent][0][1], Float:lasttime = vuser[ent][0][2];
                
                GetEntityOrigin(ent, origin);

                for(; i<amount; i++)
                {
                    if((sub_ent=CreateBullet(NORMAL_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=r, .g=g, .b=b, .a=a)) == -1 )
                        continue;

                    velocity[0] = spd * Cosine(angle);
                    velocity[1] = spd *   Sine(angle);

                    TeleportEntity(sub_ent, origin);
                    SetEntityVelocity(sub_ent, velocity);

                    SetRemoveEntityTask(sub_ent, lasttime);

                    angle += M_2PI / amount;
                }
            }

            vuser[ent][1][0] = fuser[ent][0] * fuser[ent][3] * Cosine(fuser[ent][2]);
            vuser[ent][1][1] = fuser[ent][0] * fuser[ent][3] *   Sine(fuser[ent][2]);
            vuser[ent][1][2] = 0.0;

            SetEntityVelocity(ent, vuser[ent][1]);
            SetEntityNextThink(ent, get_gametime()+DEFAULT_BULLET_THINKTIME);
        }

    //Full Random, fire once
    stock FireFullRandomBullet(Float:origin[3], owner=0, bullet_amount=64, Float:min_spd=100.0, Float:max_spd=600.0,
                                Float:last_distance=1000.0, colors[4]={100,0,255,16}, Float:damage=5.0)
    {
        new i, ent;
        new Float:spd, Float:angle, Float:velocity[3], Float:spawn_origin[3];

        for(i=0; i<bullet_amount; i++)
        {
            if((ent=CreateBullet(NORMAL_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=colors[0], .g=colors[1], .b=colors[2], .a=colors[0], .damage=damage)) == -1 )
                continue;

            angle = GetRandomFloat(0.0, M_2PI);
            spd = GetRandomFloat(min_spd, max_spd);

            spawn_origin[0] = origin[0] + 50.0 * Cosine(angle);
            spawn_origin[1] = origin[1] + 50.0 *   Sine(angle);
            spawn_origin[2] = origin[2];

            velocity[0] = spd * Cosine(angle);
            velocity[1] = spd *   Sine(angle);

            TeleportEntity(ent, origin);
            SetEntityVelocity(ent, velocity);

            SetRemoveEntityTask(ent, last_distance/spd);
        }
    }

    //Random Machine Bullet
    stock AllRandomMachineBullet(Float:origin[3], owner, 
        min_bullet_amount=2, max_bullet_amount=6, 
        Float:min_spd=100.0, Float:max_spd=500.0, Float:bullet_last_distance=1000.0,
        Float:fire_rate=0.1, Float:launch_time=8.0,
        colors[4]={100,0,255,16}, Float:damage=DEFAULT_BULLET_DMG)
    {
        new ent;

        if( (ent = CreateLauncher(RANDOM_MACHINE_BULLET_LAUNCHER, owner, damage)) == -1)
            return;

        iuser[ent][0] = min_bullet_amount;
        iuser[ent][1] = max_bullet_amount;
        iuser[ent][2] = CreateRGBAInt(colors[0], colors[1], colors[2], colors[3]);

        fuser[ent][0] = min_spd;
        fuser[ent][1] = max_spd;
        fuser[ent][2] = bullet_last_distance;
        fuser[ent][3] = fire_rate;


        TeleportEntity(ent, origin);
        SetEntityNextThink(ent, get_gametime()+fire_rate);
        SetRemoveEntityTask(ent, launch_time);
    }

        public AllRandomMachineBulletLaunch(ent)
        {
            new sub_ent, owner=GetEntityOwner(ent);
            new r=GetRGB_R(iuser[ent][2]), g=GetRGB_G(iuser[ent][2]), b=GetRGB_B(iuser[ent][2]), a=GetRGBA_A(iuser[ent][2]);
            new i=0, bullet_amount = GetRandomInt(iuser[ent][0], iuser[ent][1]);
            new Float:origin[3], Float:angle, Float:velocity[3], Float:speed, Float:lasttime, Float:damage=GetEntityDamage(ent);
            GetEntityOrigin(ent, origin);

            for(; i<bullet_amount; i++)
            {
                if((sub_ent=CreateBullet(NORMAL_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=r, .g=g, .b=b, .a=a, .damage=damage)) == -1 )
                    continue;

                angle = GetRandomFloat(0.0, M_2PI);
                speed = GetRandomFloat(fuser[ent][0], fuser[ent][1]);
                lasttime = fuser[ent][2] / speed;

                velocity[0] = speed * Cosine(angle);
                velocity[1] = speed *   Sine(angle);
                velocity[2] = 0.0;

                TeleportEntity(sub_ent, origin);
                SetEntityVelocity(sub_ent, velocity);
                
                SetRemoveEntityTask(sub_ent, lasttime);
            }

            SetEntityNextThink(ent, get_gametime()+fuser[ent][3]);

        }

    //Straight -> Round Spin -> Straight
    //Please pass angle in Degree
    stock RoundSpinBullet(Float:origin[3], bullet_amount=8, owner=0, 
        Float:angle_offset=0.0, Float:speed1=70.0, Float:speed2=200.0,
        Float:angular_spd=120.0, Float:rotate_spd=80.0, Float:total_rotate=360.0,
        colors[4]={0,255,0,16}, Float:last_time=9.0, Float:rotate_delay_time=2.0, Float:damage=DEFAULT_BULLET_DMG)
    {
        angular_spd = ToRadian(angular_spd) * DEFAULT_BULLET_THINKTIME;
        total_rotate = ToRadian(total_rotate);
        angle_offset = ToRadian(angle_offset);
        RefreshGlobalGametime();

        new ent, i=0;
        new Float:angle, Float:velocity[3];

        for(; i<bullet_amount; i++)
        {
            if((ent=CreateBullet(ROUND_SPIN_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=colors[0], .g=colors[1], .b=colors[2], .a=colors[0], .damage=damage)) == -1 )
                continue;

            angle = angle_offset + i * M_2PI / bullet_amount;
            velocity[0] = speed1 * Cosine(angle);
            velocity[1] = speed1 *   Sine(angle);
        
            fuser[ent][0] = angle;
            fuser[ent][1] = angle + total_rotate;
            fuser[ent][2] = angular_spd;
            fuser[ent][3] = rotate_spd;
            fuser[ent][4] = speed2;

            vuser[ent][0][0] = vuser[ent][0][1] = vuser[ent][0][2] = 0.0;

            TeleportEntity(ent, origin);
            SetEntityVelocity(ent, velocity);
            
            SetEntityNextThink(ent, GAMETIME+rotate_delay_time);
            SetRemoveEntityTask(ent, last_time);
        }
    }

        public RoundSpinBulletThink(ent)
        {
            fuser[ent][0] += fuser[ent][2];

            if(FloatAbs(fuser[ent][0]) < FloatAbs(fuser[ent][1]))
            {
                vuser[ent][0][0] = fuser[ent][3] * Cosine(fuser[ent][0]);
                vuser[ent][0][1] = fuser[ent][3] *   Sine(fuser[ent][0]);
                SetEntityVelocity(ent, vuser[ent][0]);
                SetEntityNextThink(ent, get_gametime()+DEFAULT_BULLET_THINKTIME);
            }
            else
            {
                vuser[ent][0][0] = fuser[ent][4] * Cosine(fuser[ent][1]);
                vuser[ent][0][1] = fuser[ent][4] *   Sine(fuser[ent][1]);
                SetEntityVelocity(ent, vuser[ent][0]);
            }
        }

    //-> Dest A -> Dest B -> Striaght
    //{delay}秒後向dest_origin 移動
    //Please pass angle in Degree
    stock ChangeDestBullet(Float:origin[3], Float:dest_origin[3], owner=0, Float:angle=0.0, Float:start_spd=200.0, Float:end_spd=250.0, 
                                Float:delay=3.0, Float:last_time=8.0, 
                                colors[4]={204,0,102,16}, end_colors[4]={102,255,0,16}, 
                                Float:damage=DEFAULT_BULLET_DMG)
    {
        new ent;

        if((ent=CreateBullet(CHANGE_DEST_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=colors[0], .g=colors[1], .b=colors[2], .a=colors[3], .damage=damage)) == -1 )
            return;
            
        new Float:velocity[3];

        velocity[0] = start_spd * Cosine(angle);
        velocity[1] = start_spd * 	Sine(angle);
        velocity[2] = 0.0;

        iuser[ent][0] = CreateRGBAInt(end_colors[0], end_colors[1], end_colors[2], end_colors[3]);

        fuser[ent][0] = end_spd;		//Speed

        CopyVector(vuser[ent][0], dest_origin);

        TeleportEntity(ent, origin);
        SetEntityVelocity(ent, velocity);
        SetEntityNextThink(ent, get_gametime()+delay);
        SetRemoveEntityTask(ent, last_time);
    }
    
        public ChangeDestBulletThink(ent)
        {
            GetEntityOrigin(ent, vuser[ent][1]);	//origin

            SubtractVectors(vuser[ent][0], vuser[ent][1], vuser[ent][0]);
            GetVectorAngles(vuser[ent][0], vuser[ent][0]);

            vuser[ent][0][1] = ToRadian(vuser[ent][0][1]);		//GetVectorAngles Return Degree

            vuser[ent][4][0] = fuser[ent][0] * Cosine(vuser[ent][0][1]);
            vuser[ent][4][1] = fuser[ent][0] *   Sine(vuser[ent][0][1]);
            vuser[ent][4][2] = 0.0;

            SetEntityVelocity(ent, vuser[ent][4]);

            SetEntityRenderColor(ent, GetRGB_R(iuser[ent][0]), GetRGB_G(iuser[ent][0]), GetRGB_B(iuser[ent][0]), GetRGBA_A(iuser[ent][0]));
        }

    //Please pass angle in Degree
    //每{switch_delay}後增加{angle_change_per_phase}度移動
    stock ChangeAngleBullet(Float:origin[3], owner, Float:spd=200.0, Float:speed_diff=0.0, Float:initial_angle=0.0, 
                                    Float:angle_change_per_phase=90.0, Float:switch_delay=2.0, Float:last_time=8.0,
                                    colors[4]={100,200,55,16}, colors_increase[4]={25,50,0,0},
                                    Float:damage=DEFAULT_BULLET_DMG)
    {
        new ent;
        initial_angle = ToRadian(initial_angle);
        angle_change_per_phase = ToRadian(angle_change_per_phase);
        if( (ent=CreateBullet(CHANGE_ANGLE_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=colors[0], .g=colors[1], .b=colors[2], .a=colors[3], .damage=damage)) == -1 )
            return;

        vuser[ent][0][0] = spd * Cosine(initial_angle);
        vuser[ent][0][1] = spd *   Sine(initial_angle);
        vuser[ent][0][2] = 0.0;

        iuser[ent][0] = CreateRGBAInt(colors[0], colors[1], colors[2], colors[3]);
        iuser[ent][1] = CreateRGBAInt(colors_increase[0], colors_increase[1], colors_increase[2], colors_increase[3]);

        fuser[ent][0] = spd;
        fuser[ent][1] = speed_diff;
        fuser[ent][2] = initial_angle;
        fuser[ent][3] = angle_change_per_phase;
        fuser[ent][4] = switch_delay;

        TeleportEntity(ent, origin);
        SetEntityVelocity(ent, vuser[ent][0]);
        SetEntityNextThink(ent, get_gametime()+switch_delay);
        SetRemoveEntityTask(ent, last_time);
    }

        public ChangeAngleBulletThink(ent)
        {
            fuser[ent][0] += fuser[ent][1];     //spd diff
            fuser[ent][2] += fuser[ent][3];     //angle diff

            vuser[ent][0][0] = fuser[ent][0] * Cosine(fuser[ent][2]);
            vuser[ent][0][1] = fuser[ent][0] *   Sine(fuser[ent][2]);

            iuser[ent][0] = CreateRGBAInt(
                 (GetRGB_R(iuser[ent][0]) + GetRGB_R(iuser[ent][1]))%255,
                 (GetRGB_G(iuser[ent][0]) + GetRGB_G(iuser[ent][1]))%255,
                 (GetRGB_B(iuser[ent][0]) + GetRGB_B(iuser[ent][1]))%255, 
                 (GetRGBA_A(iuser[ent][0]) + GetRGBA_A(iuser[ent][1]))%255
            );

            SetEntityVelocity(ent, vuser[ent][0]);
            SetEntityRenderColor(ent, GetRGB_R(iuser[ent][0]), GetRGB_G(iuser[ent][0]), GetRGB_B(iuser[ent][0]), GetRGBA_A(iuser[ent][0]));
            SetEntityNextThink(ent, get_gametime()+fuser[ent][4]);
        }			

    //Spin a round
    stock SpinBackBullet(Float:origin[3], owner, bullet_amount=16, Float:angle_offset=0.0, Float:spawn_radius=50.0, 
                                Float:spd=200.0, Float:comeback_spd=250.0, Float:comeback_radius=100.0, Float:comeback_angle_shift=45.0,
                                Float:delay=3.0, Float:last_time=8.0,
                                colors[4]={204,0,102,16}, 
                                end_colors[4]={0,102,204,16},
                                Float:damage=DEFAULT_BULLET_DMG)
    {
        new Float:spawn_origin[3], 
            Float:dest_origin[3], 
            Float:angle=ToRadian(angle_offset);
        
        new i=0;
        
        comeback_angle_shift=ToRadian(comeback_angle_shift);

        for(; i<bullet_amount; i++)
        {
            spawn_origin[0] = origin[0] + spawn_radius * Cosine(angle);
            spawn_origin[1] = origin[1] + spawn_radius *   Sine(angle);
            spawn_origin[2] = origin[2];

            dest_origin[0] = origin[0] + comeback_radius * Cosine(angle + comeback_angle_shift);
            dest_origin[1] = origin[1] + comeback_radius *   Sine(angle + comeback_angle_shift);
            dest_origin[2] = origin[2];

            ChangeDestBullet(.origin=spawn_origin, 
                        .dest_origin=dest_origin, 
                        .owner=owner, 
                        .angle=angle, 
                        .start_spd=spd, 
                        .end_spd=comeback_spd, 
                        .delay=delay, 
                        .last_time=last_time,
                        .colors=colors,
                        .end_colors=end_colors,
                        .damage=damage);

            angle += M_2PI / bullet_amount;
        }
    }

    //Draw Shape
    stock ShapeDrawingBullet(Float:origin[3], owner, Float:angle_offset=0.0, bullet_amount=8, side_amount=4,
                                    Float:side_distance=400.0, Float:spd=200.0, Float:speed_diff=0.0, draw_side=8,
                                    colors[4]={100,200,55,16}, colors_increase[4]={25,50,0,0},
                                    Float:damage=DEFAULT_BULLET_DMG)
    {
        new Float:angle_diff=M_FULL_ANGLE/side_amount,      //Angle diff per phase
            Float:switch_delay=side_distance/spd,
            Float:last_time=switch_delay*(draw_side)-0.1;		

        for(new i=0; i<bullet_amount; i++)
        {
            ChangeAngleBullet(.origin=origin,
                        .owner=owner,
                        .initial_angle=angle_offset,
                        .angle_change_per_phase=angle_diff,
                        .spd=spd,
                        .speed_diff=speed_diff,
                        .switch_delay=switch_delay,
                        .last_time=last_time,
                        .colors=colors,
                        .colors_increase=colors_increase,
                        .damage=damage
                        );

            angle_offset += M_FULL_ANGLE/bullet_amount;
        }
    }

    //Machine Bullet
    stock RotateMachineBullet(Float:origin[3], owner=0, 
        clockwise=1, bullet_per_fire=2, Float:fire_interval = 0.1, Float:bullet_speed=180.0,
        Float:angle_offset=0.0, Float:angular_spd=10.0, Float:total_rotation=180.0, 
        Float:bullet_last_time=4.0, colors[4]={255,0,255,16}, Float:damage=DEFAULT_BULLET_DMG)
    {
        bullet_per_fire = Clamp(bullet_per_fire, 0, 5);

        new ent;
        if( (ent = CreateLauncher(ROTATE_MACHINE_BULLET_LAUNCHER, owner, damage)) == -1 )
            return;

        iuser[ent][0] = bullet_per_fire;
        iuser[ent][1] = clockwise;
        iuser[ent][2] = CreateRGBAIntByArray(colors);
        iuser[ent][3] = 0;
        iuser[ent][4] = RoundFloat(total_rotation/angular_spd);

        iuser[ent][0] = bullet_per_fire;
        iuser[ent][1] = clockwise;
        iuser[ent][2] = CreateRGBAIntByArray(colors);

        fuser[ent][0] = fire_interval;
        fuser[ent][1] = bullet_speed;
        fuser[ent][2] = bullet_last_time;
        fuser[ent][3] = ToRadian(angle_offset);
        fuser[ent][4] = ToRadian(angular_spd);

        TeleportEntity(ent, origin);
        
        RotateMachineBulletLauncherThink(ent);
    }

        public RotateMachineBulletLauncherThink(ent)
        {
            new sub_ent, i=0;
            GetEntityOrigin(ent, vuser[ent][2]);

            for(; i<iuser[ent][0]; i++)
            {
                if((sub_ent=CreateBullet(NORMAL_BULLET, 
                                COMMON_BULLET_MODEL, 
                                .solid_type=DEFAULT_BULLET_SOLID, 
                                .movetype=MOVETYPE_NOCLIP, 
                                .owner=GetEntityOwner(ent), 
                                .r= GetRGB_R(iuser[ent][2]), 
                                .g= GetRGB_G(iuser[ent][2]), 
                                .b= GetRGB_B(iuser[ent][2]), 
                                .a=GetRGBA_A(iuser[ent][2]), 
                                .damage=GetEntityDamage(ent))) == -1 )
                    continue;

                fuser[sub_ent][0] = fuser[ent][3] + M_2PI * i / iuser[ent][0];

                //Velocity
                vuser[sub_ent][1][0] = fuser[ent][1] * Cosine(fuser[sub_ent][0]);
                vuser[sub_ent][1][1] = fuser[ent][1] *   Sine(fuser[sub_ent][0]);
                vuser[sub_ent][1][2] = 0.0;
                
                TeleportEntity(sub_ent, vuser[ent][2]);
                SetEntityVelocity(sub_ent, vuser[sub_ent][1]);

                SetRemoveEntityTask(sub_ent, fuser[ent][2]);
            }

            if(++iuser[ent][3] < iuser[ent][4]) {       //Still Rotate?
                fuser[ent][3] += fuser[ent][4];             //Add Angle
                SetEntityNextThink(ent, get_gametime()+fuser[ent][0]);
            }
            else {
                RemoveEntity(ent);
                return;
            }
        }

    //直線推進，分裂，自己消失
    //Please pass angle using Degree
    stock SplitStraightBullet(Float:origin[3], owner, bullet_amount=3, Float:bullet_spd=150.0,
        Float:angle_offset=0.0, Float:angle_difference_min=45.0, Float:angle_difference_max=135.0, 
        Float:split_bullet_max_spd=300.0, total_split=5, total_split_direction=4, Float:split_bullet_last_time=7.0, 
        Float:split_angle_offset=0.0, Float:split_interval=5.0,
        colors[4]={255,51,255,255}, Float:damage=DEFAULT_BULLET_DMG)
    {
        new i=0, ent;
        new Float:angle = ToRadian(angle_offset), Float:velocity[3];

        angle_difference_min = ToRadian(angle_difference_min);
        angle_difference_max = ToRadian(angle_difference_max);
        bullet_amount = (bullet_amount<=5)?bullet_amount:5;
        split_angle_offset = ToRadian(split_angle_offset);
        RefreshGlobalGametime();

        for(; i<bullet_amount; i++, angle += GetRandomFloat(angle_difference_min, angle_difference_max))
        {
            if((ent=CreateBullet(SPLIT_STRAIGHT_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=colors[0], .g=colors[1], .b=colors[2], .a=colors[3], .damage=damage)) == -1 )
                continue;

            velocity[0] = bullet_spd * Cosine(angle);
            velocity[1] = bullet_spd *   Sine(angle);
            velocity[2] = 0.0;

            iuser[ent][0] = total_split;
            iuser[ent][1] = total_split_direction;
            iuser[ent][2] = CreateRGBAIntByArray(colors);

            fuser[ent][0] = split_bullet_max_spd;
            fuser[ent][1] = split_bullet_last_time;
            fuser[ent][2] = split_angle_offset;
            
            TeleportEntity(ent, origin);
            SetEntityVelocity(ent, velocity);
            SetEntityNextThink(ent, GAMETIME+split_interval);
        }
    }

        public SplitStraightBulletThink(ent)
        {
            new owner = GetEntityOwner(ent), r=GetRGB_R(iuser[ent][2]), g=GetRGB_G(iuser[ent][2]), b=GetRGB_B(iuser[ent][2]), a=GetRGBA_A(iuser[ent][2]), i=0, j=0, sub_ent;
            new Float:spd = fuser[ent][0], Float:split_bullet_max_spd=fuser[ent][0], Float:split_bullet_last_time = fuser[ent][1], Float:angle, Float:origin[3], Float:velocity[3], Float:damage=GetEntityDamage(ent);
            
            GetEntityOrigin(ent, origin);

            for(; i<iuser[ent][0]; i++)
            {
                angle = fuser[ent][2];			//angle = angle_offset

                for(j=0; j<iuser[ent][1]; j++, angle += M_2PI / iuser[ent][1])
                {
                    if((sub_ent=CreateBullet(NORMAL_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=r, .g=g, .b=b, .a=a, .damage=damage)) == -1 )
                        continue;

                    velocity[0] = spd * Cosine(angle);
                    velocity[1] = spd *   Sine(angle);
                    velocity[2] = 0.0;

                    TeleportEntity(sub_ent, origin);
                    SetEntityVelocity(sub_ent, velocity);

                    SetRemoveEntityTask(sub_ent, split_bullet_last_time);
                }
                
                spd -= split_bullet_max_spd / iuser[ent][0];
            }

            RemoveEntity(ent);
            return;
        }

    //出現，消失繰り返し
    stock AppearAndDisapperBullet(Float:origin[3], owner=0, bullet_amount=3, Float:bullet_spd=150.0, 
        Float:angle_offset=0.0, Float:angle_difference_min=120.0, Float:angle_difference_max=120.0,
        Float:bullet_last_time=8.0, Float:phase_interval=1.0, 
        colors[4]={150,205,25,16}, Float:damage=DEFAULT_BULLET_DMG)
    {
        new i=0, ent;
        new Float:velocity[3], Float:angle=angle_offset;

        angle_difference_min = ToRadian(angle_difference_min);
        angle_difference_max = ToRadian(angle_difference_max);
        RefreshGlobalGametime();
        
        for(; i<bullet_amount; i++, angle += GetRandomFloat(angle_difference_min, angle_difference_max))
        {
            if((ent=CreateBullet(APPEAR_AND_DISAPPER_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=colors[0], .g=colors[1], .b=colors[2], .a=colors[3], .damage=damage)) == -1 )
                continue;
            
            iuser[ent][0] = 0;

            fuser[ent][0] = phase_interval;

            velocity[0] = bullet_spd * Cosine(angle);
            velocity[1] = bullet_spd *   Sine(angle);
            velocity[2] = 0.0;

            TeleportEntity(ent, origin);
            SetEntityVelocity(ent, velocity);
            SetEntityNextThink(ent, GAMETIME+phase_interval);
            SetRemoveEntityTask(ent, bullet_last_time);
        }		

    }
        
        public AppearAndDisapperBulletThink(ent)
        {
            switch(iuser[ent][0]++ % 2)
            {	
                case 0:			//Disapper
                {
                    entity_set_int(ent, EV_INT_solid, SOLID_NOT);
                    set_pev(ent, pev_effects, pev(ent, pev_effects) | EF_NODRAW);
                }
                case 1:			//Appear
                {
                    entity_set_int(ent, EV_INT_solid, DEFAULT_BULLET_SOLID);
                    set_pev(ent, pev_effects, pev(ent, pev_effects) & ~EF_NODRAW);
                }
            }

            SetEntityNextThink(ent, get_gametime()+fuser[ent][0]);
        }


    //Pass angle in degree
    stock FireNonPenetrateBullet(Float:origin[3], Float:angles[3], owner, Float:speed=200.0, Float:last_distance=0.0, 
                                            colors[4]={200,200,0,16}, Float:damage=DEFAULT_BULLET_DMG)
    {
        new ent;

        if( (ent = CreateBullet(NON_PENETRATE_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=colors[0], .g=colors[1], .b=colors[2], .a=colors[3], .damage=damage)) == -1 )
            return;

        angles[0] = ToRadian(angle[0]);
        angles[1] = ToRadian(angle[1]);

        vuser[ent][0][0] = speed * Cosine(angles[0]) * Cosine(angles[1]);
        vuser[ent][0][1] = speed * Cosine(angles[0]) *   Sine(angles[1]);
        vuser[ent][0][2] = speed *   Sine(angles[0]);

        TeleportEntity(ent, origin);
        SetEntityVelocity(ent, vuser[ent][0]);

        SetRemoveEntityTask(ent, (last_distance>0.0)?last_distance/speed:20.0);
    }

    //Pointing Bullet
    stock FirePointingBullet(Float:origin[3], owner, bullet_amount=1, Float:speed=200.0, Float:last_distance=1000.0,
        colors[4]={200,200,0,16}, Float:damage=DEFAULT_BULLET_DMG)
    {
        new ent, i=0;
        new Float:angles[3], Float:velocity[3], Float:pitch, Float:yaw;
        
        last_distance /= speed;
        GetClientEyeAngles(owner, angles);
        pitch = ToRadian(-1.0*angles[0]);
        yaw = ToRadian(angles[1]);

        for(; i<bullet_amount; i++, yaw += M_2PI/bullet_amount)
        {
            if((ent=CreateBullet(NORMAL_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=colors[0], .g=colors[1], .b=colors[2], .a=colors[3], .damage=damage)) == -1 )
                continue;

            velocity[0] = speed * Cosine(pitch) * Cosine(yaw);
            velocity[1] = speed * Cosine(pitch) *   Sine(yaw);
            velocity[2] = speed *   Sine(pitch);

            TeleportEntity(ent, origin);
            SetEntityVelocity(ent, velocity);

            SetRemoveEntityTask(ent, last_distance);    //last distance has been divided before
        }
    }

    //直線推進，背後分裂
    stock SplitBackwardBullet(Float:origin[3], owner=0, Float:angle_offset=0.0, Float:split_base_angle=90.0, Float:angle_per_each_split = 60.0,
                                Float:angle_diff_min=15.0, Float:angle_diff_max=70.0, bullet_amount=3,
                                Float:split_interval=2.0, Float:bullet_spd=150.0, Float:split_bullet_spd=250.0,
                                total_split_direction=4, Float:bullet_last_time=7.0, Float:split_bullet_last_time=5.0,
                                colors[4]={0,204,0,16}, colors_split[4]={255,51,255,16}, Float:damage=DEFAULT_BULLET_DMG)
    {
        new i=0, ent;
        new Float:velocity[3];

        angle_offset = ToRadian(angle_offset);
        split_base_angle = ToRadian(split_base_angle);
        angle_per_each_split = ToRadian(angle_per_each_split);
        angle_diff_min = ToRadian(angle_diff_min);
        angle_diff_max = ToRadian(angle_diff_max);

        bullet_amount = Clamp(bullet_amount, 1, 8);
        total_split_direction = Clamp(total_split_direction, 1, 5);

        RefreshGlobalGametime();

        for(; i<bullet_amount; i++, angle_offset+=GetRandomFloat(angle_diff_min, angle_diff_max))
        {
            if((ent = CreateBullet(SPLIT_BACKWARD_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=colors[0], .g=colors[1], .b=colors[2], .a=colors[3], .damage=damage)) == -1 )
                continue;

            velocity[0] = bullet_spd * Cosine(angle_offset);
            velocity[1] = bullet_spd *   Sine(angle_offset);

            iuser[ent][0] = total_split_direction;
            iuser[ent][1] = CreateRGBAIntByArray(colors_split);

            fuser[ent][0] = split_bullet_spd;
            fuser[ent][1] = split_bullet_last_time;
            fuser[ent][2] = angle_offset + split_base_angle;
            fuser[ent][3] = angle_per_each_split;
            fuser[ent][4] = split_interval;

            TeleportEntity(ent, origin);
            SetEntityVelocity(ent, velocity);
            SetEntityNextThink(ent, GAMETIME+split_interval);
            SetRemoveEntityTask(ent, bullet_last_time);
        }
    }

        public SplitBackwardBulletThink(ent)
        {
            new i=0, sub_ent, r=GetRGB_R(iuser[ent][1]), g=GetRGB_G(iuser[ent][1]), b=GetRGB_B(iuser[ent][1]), a=GetRGBA_A(iuser[ent][1]), total_split_direction=iuser[ent][0], owner=GetEntityOwner(ent);
            new Float:origin[3], Float:velocity[3], Float:angle=fuser[ent][2], Float:speed = fuser[ent][0], Float:last_time = fuser[ent][1], Float:damage=GetEntityDamage(ent);

            GetEntityOrigin(ent, origin);

            for(; i<total_split_direction; i++)
            {
                if((sub_ent = CreateBullet(NORMAL_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=r, .g=g, .b=b, .a=a, .damage=damage)) == -1 )
                    continue;

                velocity[0] = speed * Cosine(angle);
                velocity[1] = speed *   Sine(angle);

                TeleportEntity(sub_ent, origin);
                SetEntityVelocity(sub_ent, velocity);

                SetRemoveEntityTask(sub_ent, last_time);

                angle += fuser[ent][3];
            }

            SetEntityNextThink(ent, get_gametime()+fuser[ent][4]);
        }

    //Decel -> New Angle -> Accel
    stock LaunchSlowChangeDirBullet(Float:origin[3], owner=0, Float:initial_angle=0.0, Float:angle_difference=10.0, bullet_amount=36,
                                        Float:fire_rate=0.1, Float:spd=300.0, Float:deceleration=80.0, Float:bullet_last_time=10.0, 
                                        colors[4]={255,178,102,16}, Float:damage=DEFAULT_BULLET_DMG)
    {
        new ent;
        
        if( (ent = CreateLauncher(SLOW_CHANGE_DIR_BULLET_LAUNCHER, owner, damage)) == -1 )
            return;

        iuser[ent][0] = 0;
        iuser[ent][1] = bullet_amount;
        iuser[ent][2] = CreateRGBAIntByArray(colors);
        iuser[ent][3] = RoundFloat(fire_rate*1000);

        fuser[ent][0] = spd;
        fuser[ent][1] = deceleration * DEFAULT_BULLET_THINKTIME;
        fuser[ent][2] = bullet_last_time;
        fuser[ent][3] = ToRadian(initial_angle);
        fuser[ent][4] = ToRadian(angle_difference);

        TeleportEntity(ent, origin);
        SlowChangeDirBulletLauncherThink(ent);

    }

        public SlowChangeDirBulletLauncherThink(ent)
        {
            new sub_ent, r=GetRGB_R(iuser[ent][2]), g=GetRGB_G(iuser[ent][2]), b=GetRGB_B(iuser[ent][2]), a=GetRGBA_A(iuser[ent][2]), owner=GetEntityOwner(ent);
            new Float:origin[3], Float:velocity[3], Float:spd=fuser[ent][0], Float:angle=fuser[ent][3]+iuser[ent][0]*fuser[ent][4], Float:damage=GetEntityDamage(ent);

            if((sub_ent=CreateBullet(SLOW_CHANGE_DIR_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=r, .g=g, .b=b, .a=a, .damage=damage)) != -1 ) {
                
                GetEntityOrigin(ent, origin);
                velocity[0] = spd * Cosine(angle);
                velocity[1] = spd *   Sine(angle);

                iuser[sub_ent][0] = 0;		//phase

                fuser[sub_ent][0] = spd;
                fuser[sub_ent][1] = fuser[ent][1];		//Deceleration
                fuser[sub_ent][2] = angle;

                TeleportEntity(sub_ent, origin);
                SetEntityVelocity(sub_ent, velocity);
                SetEntityNextThink(sub_ent, get_gametime()+DEFAULT_BULLET_THINKTIME);
                SetRemoveEntityTask(sub_ent, fuser[ent][2]);
            }

            if(++iuser[ent][0] < iuser[ent][1]) {
                SetEntityNextThink(ent, get_gametime()+iuser[ent][3]/1000.0);
            }
            else{
                RemoveEntity(ent);
                return;
            }
        }

        public SlowChangeDirBulletThink(ent)
        {
            new Float:velocity[3], Float:spd, Float:deceleration, Float:angle;

            spd = fuser[ent][0];
            deceleration = fuser[ent][1];
            angle = fuser[ent][2];

            if(iuser[ent][0] == 0)
            {
                if((spd-=deceleration) < 0.0)
                {
                    iuser[ent][0] = 1;
                    spd = 0.0;
                    fuser[ent][2] += M_PI + GetRandomFloat(-M_1OVER2PI, M_1OVER2PI);
                    
                    SetEntityNextThink(ent, get_gametime()+0.5);
                } else {
                    SetEntityNextThink(ent, get_gametime()+DEFAULT_BULLET_THINKTIME);
                }
            }
            else
            {
                spd += deceleration;
                SetEntityNextThink(ent, get_gametime()+DEFAULT_BULLET_THINKTIME);
            }

            fuser[ent][0] = spd;
            velocity[0] = spd * Cosine(angle);
            velocity[1] = spd *   Sine(angle);

            SetEntityVelocity(ent, velocity);
        }

    stock ExplosiveBullet(Float:origin[3], owner=0, bullet_amount=3, Float:bullet_spd=150.0,
                            Float:angle_offset=0.0, Float:last_time=7.0, 
                            Float:damage_radius=300.0, Float:damage=30.0,
                            colors[4]={200,0,0,16})
    {
        new i=0, ent;
        new Float:velocity[3];

        angle_offset = ToRadian(angle_offset);
        RefreshGlobalGametime();

        for(; i<bullet_amount; i++)
        {
            if((ent=CreateBullet(EXPLOSIVE_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=colors[0], .g=colors[1], .b=colors[2], .a=colors[3], .damage=damage)) == -1 )
                continue;
                    
            iuser[ent][0] = 0;
            iuser[ent][1] = CreateRGBAIntByArray(colors);

            fuser[ent][0] = damage_radius;
            fuser[ent][1] = damage;

            velocity[0] = bullet_spd * Cosine(angle_offset);
            velocity[1] = bullet_spd *   Sine(angle_offset);
            
            TeleportEntity(ent, origin);
            SetEntityVelocity(ent, velocity);
            SetEntityNextThink(ent, GAMETIME+EXPLOSIVE_BULLET_THINKTIME);
            SetRemoveEntityTask(ent, last_time);
        
            angle_offset += M_2PI / bullet_amount;
        }
    }

        public ExplosiveBulletThink(ent)
        {
            switch((++iuser[ent][0])%2)
            {
                case 1:set_ent_rendering(ent, kRenderFxNone, 0, 0, 0, kRenderNormal, 0);
                case 0:set_ent_rendering(ent, kRenderFxGlowShell, GetRGB_R(iuser[ent][1]), GetRGB_G(iuser[ent][1]), GetRGB_B(iuser[ent][1]), kRenderTransAlpha, GetRGBA_A(iuser[ent][1]));
            }
            
            SetEntityNextThink(ent, get_gametime()+EXPLOSIVE_BULLET_THINKTIME);
        }
        
        public OnExplosiveBulletTouch(ent, target)
        {
            new owner=GetEntityOwner(ent);
            if(target!=owner && IsClientValid(target))
            {
                new i=0;
                new Float:origin[3], Float:distance, Float:max_radius=fuser[ent][0], Float:damage=fuser[ent][1];
                GetEntityOrigin(ent, origin);
                for(; i<=MaxClients; i++)
                {
                    if( IsClientValid(i) && IsPlayerAlive(i) && (owner==0 || (i!=owner && cs_get_user_team(owner)!=cs_get_user_team(i))) )
                    {
                        distance = 1.0 * get_entity_distance(ent, i);
                        if(distance < max_radius)
                            ExecuteHamB(Ham_TakeDamage, i, 0, owner, damage*distance/max_radius, DMG_BLAST|DMG_ALWAYSGIB);
                        
                    }
                }
                te_create_explosion_f(origin, EXPLOSION_SPRITE_ID);

                RemoveEntity(ent);
                return;
            }
        }

    //Speed Remain Unchange
    //Only Change Motion Angle
    stock SwirlBullet(Float:origin[3], owner, bullet_amount=6, 
                        Float:angle_offset=0.0, Float:spd=200.0, Float:angular_spd=30.0, 
                        Float:last_time=6.0, Float:damage=6.0, 
                        colors[4]={255,20,147,16})
    {
        new ent, i=0;
        new Float:velocity[3];
        RefreshGlobalGametime();
        angle_offset = ToRadian(angle_offset);

        for(; i<bullet_amount; i++)
        {
            if((ent=CreateBullet(SWIRL_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=colors[0], .g=colors[1], .b=colors[2], .a=colors[3], .damage=damage)) == -1 )
                continue;
                
            velocity[0] = spd * Cosine(angle_offset);
            velocity[1] = spd *   Sine(angle_offset);

            fuser[ent][0] = spd;
            fuser[ent][1] = angle_offset;
            fuser[ent][2] = DEFAULT_BULLET_THINKTIME * ToRadian(angular_spd);

            //For Think velocity
            vuser[ent][0][2] = 0.0;

            TeleportEntity(ent, origin);
            SetEntityVelocity(ent , velocity);

            SetEntityNextThink(ent, GAMETIME+DEFAULT_BULLET_THINKTIME);
            SetRemoveEntityTask(ent, last_time);

            angle_offset += M_2PI/bullet_amount;
        }
    }

        public SwirlBulletThink(ent)
        {
            vuser[ent][0][0] = fuser[ent][0] * Cosine(fuser[ent][1]);
            vuser[ent][0][1] = fuser[ent][0] *   Sine(fuser[ent][1]);

            fuser[ent][1] += fuser[ent][2];
            SetEntityVelocity(ent, vuser[ent][0]);
            SetEntityNextThink(ent, get_gametime()+DEFAULT_BULLET_THINKTIME);
        }

    // Last Think ~ Remove	<-- Please pass this as last time, dont pass full time
    stock TracerBullet(Float:origin[3], owner, bullet_amount=6, stages=2,
                        Float:min_radius=250.0, Float:max_radius=400.0, 
                        Float:switch_delay=2.0, Float:last_time=3.0, Float:damage=7.0,
                        colors[4]={0,102,0,16}, colors_final[4]={255,185,15,16})
    {
        new ent, i, int_colors=CreateRGBAIntByArray(colors_final), players[MAX_PLAYERS], player_amount;
        new Float:spd, Float:velocity[3]={0.0, ...}, Float:angle;

        get_players_ex(players, player_amount, GetPlayers_ExcludeDead);
        if(player_amount<=0) return;

        RefreshGlobalGametime();

        for(; i<bullet_amount; i++)
        {
            if((ent=CreateBullet(TRACER_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=colors[0], .g=colors[1], .b=colors[2], .a=colors[3], .damage=damage)) == -1 )
                continue;

            angle = GetRandomAngleRadian();
            spd = GetRandomFloat(max_radius, max_radius) / switch_delay;

            velocity[0] = spd * Cosine(angle);
            velocity[1] = spd *   Sine(angle);

            iuser[ent][0] = 0;
            iuser[ent][1] = 2*stages;
            iuser[ent][2] = int_colors;
            iuser[ent][3] = players[GetRandomInt(0, player_amount-1)];

            fuser[ent][0] = angle + GetRandomFloat(M_PI/2.0, M_PI*3.0/2.0);
            fuser[ent][1] = min_radius;
            fuser[ent][2] = max_radius;
            fuser[ent][3] = switch_delay;
            fuser[ent][4] = last_time;


            //Used for velocity
            vuser[ent][1][0] = 0.0;
            vuser[ent][1][1] = 0.0;
            vuser[ent][1][2] = 0.0;

            CopyVector(vuser[ent][0], origin);
            TeleportEntity(ent, origin);
            SetEntityVelocity(ent, velocity);

            SetEntityNextThink(ent, GAMETIME+switch_delay);
            SetRemoveEntityTask(ent, (1+stages*2)*switch_delay+last_time);    
        }
    }

        public TracerBulletThink(ent)
        {
            if(iuser[ent][0] >= iuser[ent][1]-1)
            {
                //Trace Player
                if(IsClientValid(iuser[ent][3]) && IsPlayerAlive(iuser[ent][3])) 
                {
                    GetClientAbsOrigin(iuser[ent][3], vuser[ent][0]);
                }

                SetEntityRenderColor(ent, GetRGB_R(iuser[ent][2]), GetRGB_G(iuser[ent][2]), GetRGB_B(iuser[ent][2]), GetRGB_B(iuser[ent][3]));
                GetEntityOrigin(ent, vuser[ent][2]);
                fuser[ent][4] = GetVectorDistance(vuser[ent][0], vuser[ent][2])*2.0/fuser[ent][4];			//Speed
                SubtractVectors(vuser[ent][0], vuser[ent][2], vuser[ent][0]);
                GetVectorAngles(vuser[ent][0], vuser[ent][2]);
                vuser[ent][2][1] = ToRadian(vuser[ent][2][1]);

                vuser[ent][0][0] = fuser[ent][4] * Cosine(vuser[ent][2][1]);
                vuser[ent][0][1] = fuser[ent][4] *   Sine(vuser[ent][2][1]);

                SetEntityVelocity(ent, vuser[ent][0]);
            }
            else
            {
                if(++iuser[ent][0]%2)
                {
                    vuser[ent][1][0] = 0.0;
                    vuser[ent][1][1] = 0.0;
                    vuser[ent][1][2] = 0.0;
                    
                }
                else
                {
                    new Float:spd = GetRandomFloat(fuser[ent][1], fuser[ent][2]) / fuser[ent][3];		//Speed
                    vuser[ent][1][0] = spd * Cosine(fuser[ent][0]);
                    vuser[ent][1][1] = spd *   Sine(fuser[ent][0]);
                    fuser[ent][0] = fuser[ent][0] + GetRandomFloat(M_PI/2.0, M_PI*3.0/2.0);
                }

                SetEntityNextThink(ent, get_gametime()+fuser[ent][3]);
                SetEntityVelocity(ent, vuser[ent][1]);
            }
            
        }

    // LaserBullet(Float:origin[3], owner, bullet_amount=6, 
    // 				Float:min_radius=200.0, Float:max_radius=400.0, Float:time_to_dest=2.0,
    // 				Float:ready_time=1.0, laser_amount=4, fire_time=2, 
    // 				Float:bullet_damage=12.0, Float:laser_damage=2.0, 
    // 				r=255, g=255, b=0)
    // {
    // 	i=0, ent, ce_counter=GetCustomEntityCustomRefKV(), color_int=CreateRGBInt(r, g, b);
    // 	Float:spd, angle, velocity[3] = {0.0, ...}, last_time=(RoundFloat(time_to_dest/ready_time)+2)*fire_time*ready_time-0.1;

    // 	for(; ce_counter!=-1&&i<bullet_amount; i++, ce_counter=GetCustomEntityCustomRefKV())
    // 	{
    // 		ent = CreateBullet(COMMON_BULLET_MODEL, ce_counter, DEFAULT_BULLET_SOLID, MOVETYPE_NOCLIP, 0.0, 0.6, owner, r, g, b, 255, .damage=bullet_damage);

    // 		if(ent != -1)
    // 		{
    // 			spd = GetRandomFloat(min_radius, max_radius) / time_to_dest;
    // 			angle = GetRandomAngle();

    // 			velocity[0] = spd * Cosine(ToRadian(angle));
    // 			velocity[1] = spd *   Sine(ToRadian(angle));

    // 			iuser[ce_counter][0] = 0;		//counter
    // 			iuser[ce_counter][1] = RoundFloat(time_to_dest/ready_time);
    // 			iuser[ce_counter][2] = 0;		//phase		0:move 1:stop 2:fire 3:determine next origin
    // 			iuser[ce_counter][3] = laser_amount;
    // 			iuser[ce_counter][4] = color_int;

    // 			fuser[ce_counter][0] = ready_time;
    // 			fuser[ce_counter][1] = min_radius / time_to_dest;
    // 			fuser[ce_counter][2] = max_radius / time_to_dest;
    // 			fuser[ce_counter][3] = laser_damage;

    // 			//Used for new velocity
    // 			vuser[ce_counter][0] = {0.0, 0.0, 0.0};
    // 			//Moving and laser angle
    // 			vuser[ce_counter][1][0] = 0.0; vuser[ce_counter][1][1] = angle; vuser[ce_counter][1][2] = 0.0;

    // 			TeleportEntity(ent, origin, NULL_VECTOR, velocity);
    // 			g_hEntityThinkTask[ce_counter] = CreateTimer(ready_time, LaserBulletThink, ce_counter, TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);
    // 			g_hEntityRemoveTask[ce_counter] = CreateTimer(last_time, RemoveBullet, ce_counter, TIMER_FLAG_NO_MAPCHANGE);
    // 		}
            
    // 	}
    // }

    // 	public Action LaserBulletThink(Handle timer, ce_counter)
    // 	{
    // 		switch(iuser[ce_counter][2])
    // 		{
    // 			case 0:			//wait till destination
    // 			{
    // 				if(++iuser[ce_counter][0] >= iuser[ce_counter][1])
    // 				{
    // 					iuser[ce_counter][0] = 0;
    // 					iuser[ce_counter][2] = 1;
    // 					TeleportEntity(g_iCustomEntityRef[ce_counter], NULL_VECTOR, NULL_VECTOR, {0.0, 0.0, 0.0});
    // 				}

    // 				if(iuser[ce_counter][0]%2)
    // 					SetEntityRenderColor(g_iCustomEntityRef[ce_counter], 255, 255, 255, 255);
    // 				else
    // 					SetEntityRenderColor(g_iCustomEntityRef[ce_counter], GetRGB_R(iuser[ce_counter][4]), GetRGB_G(iuser[ce_counter][4]), GetRGB_B(iuser[ce_counter][4]), 255);
    // 			}
    // 			case 1:			//FIRE
    // 			{
    // 				iuser[ce_counter][2] = 2;
    // 				GetEntityOrigin(g_iCustomEntityRef[ce_counter], vuser[ce_counter][2]);

    // 				i=0, laser_amount=iuser[ce_counter][3];
    // 				Float:origin[3], angle[3];
    // 				for(; i<laser_amount; i++)
    // 				{
    // 					vuser[ce_counter][1][1] += 360.0 / laser_amount;
    // 					CopyVector(vuser[ce_counter][2], origin);
    // 					CopyVector(vuser[ce_counter][1], angle);
    // 					FireLaser(GetEntityOwner(g_iCustomEntityRef[ce_counter]), origin, angle, 9999.9, .life=fuser[ce_counter][0], .width=2.5, .damage=fuser[ce_counter][3], .r=GetRGB_R(iuser[ce_counter][4]), .g=GetRGB_G(iuser[ce_counter][4]), .b=GetRGB_B(iuser[ce_counter][4]), .a=255, .slap=true);
    // 				}
    // 			}
    // 			case 2:			//Search for new origin
    // 			{
    // 				fuser[ce_counter][4] = GetRandomFloat(fuser[ce_counter][1], fuser[ce_counter][2]);
    // 				vuser[ce_counter][1][1] = GetRandomFloat(90.0, 270.0);

    // 				vuser[ce_counter][0][0] = fuser[ce_counter][4] * Cosine(ToRadian(vuser[ce_counter][1][1]));
    // 				vuser[ce_counter][0][1] = fuser[ce_counter][4] *   Sine(ToRadian(vuser[ce_counter][1][1]));

    // 				TeleportEntity(g_iCustomEntityRef[ce_counter], NULL_VECTOR, NULL_VECTOR, vuser[ce_counter][0]);

    // 				iuser[ce_counter][2] = 0;
    // 			}
    // 		}

    // 		return Plugin_Continue;
    // 	}

    //Snake Protect
    #define MAX_SNAKE_PROTECT_AMOUNT	8
    stock SnakeProtect(Float:origin[3], owner, bullet_amount=8, Float:acceleration=200.0, 
                        Float:last_time=15.0, Float:delay_per_bullet=0.1, 
                        colors[4]={255,255,0,16}, Float:damage=DEFAULT_BULLET_DMG)
    {
        new i=1, ent, previous_ent, sub_ent;

        if((ent=CreateLauncher(SNAKE_PROTECT_CONTROLLER, owner, damage)) == -1)
            return;
            
        acceleration *= delay_per_bullet;

        // Bullet
        previous_ent = CreateBullet(SNAKE_PROTECT_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=colors[0], .g=colors[1], .b=colors[2], .a=colors[3], .damage=damage);
        if(previous_ent == -1) return;
    
        te_create_beam_between_entities(owner, previous_ent, LASER_BEAM_SPRITE_ID, .life=RoundFloat(last_time*10-1), .framerate=0, .width=5, .r=255, .b=255, .a=200, .reliable=false);

        iuser[previous_ent][0] = 0;         //Head
        TeleportEntity(previous_ent, origin);
        ResetVector(vuser[previous_ent][0]);
        
        iuser[ent][4] = RoundFloat(last_time/delay_per_bullet);
        
        fuser[ent][1] = acceleration;
        fuser[ent][2] = delay_per_bullet;
        
        for(; i<bullet_amount; i++) {
            if((sub_ent=CreateBullet(SNAKE_PROTECT_BULLET, COMMON_BULLET_MODEL, .solid_type=DEFAULT_BULLET_SOLID, .movetype=MOVETYPE_NOCLIP, .owner=owner, .r=colors[0], .g=colors[1], .b=colors[2], .a=colors[3], .damage=damage)) == -1)
                continue;

            iuser[sub_ent][0] = previous_ent;       //Tail -> Head
            iuser[previous_ent][1] = sub_ent;       //Head -> Tail
            
            ResetVector(vuser[sub_ent][0]);

            TeleportEntity(sub_ent, origin);

            previous_ent = sub_ent;
        }

        iuser[ent][1] = sub_ent;        //Tail
        SetEntityNextThink(ent, get_gametime()+delay_per_bullet);
    }

        public SnakeProtectControllerThink(launcher) {
            new sub_ent, owner=GetEntityOwner(launcher);
            new Float:velocity[3], Float:target_origin[3], Float:origin[3];

            if(iuser[launcher][4]--<=0 || !IsClientValid(owner) || !IsPlayerAlive(owner))
            {
                for(sub_ent=iuser[launcher][1]; sub_ent!=0; sub_ent=iuser[sub_ent][0]) {
                    RemoveEntity(sub_ent);
                }
                
                RemoveEntity(launcher);
                return;
            }

            for(sub_ent=iuser[launcher][1]; iuser[sub_ent][0]!=0; sub_ent=iuser[sub_ent][0]) {
                GetEntityVelocity(iuser[sub_ent][0], velocity);

                SetEntityVelocity(sub_ent, velocity);
            }

            //Process Head
            GetClientAbsOrigin(owner, target_origin);		//Get Owner Origin
            GetEntityOrigin(sub_ent, origin);	            	//Get Bullet Origin

            SubtractVectors(target_origin, origin, target_origin);
            GetVectorAngles(target_origin, target_origin);
            GetEntityVelocity(sub_ent, velocity);

            //Angle to Radian
            target_origin[0] = ToRadian(target_origin[0]);
            target_origin[1] = ToRadian(target_origin[1]);

            velocity[0] = velocity[0] + fuser[launcher][1] * Cosine(target_origin[0]) * Cosine(target_origin[1]);
            velocity[1] = velocity[1] + fuser[launcher][1] * Cosine(target_origin[0]) *   Sine(target_origin[1]);
            velocity[2] = velocity[2] + fuser[launcher][1] *   Sine(target_origin[0]);
            
            SetEntityVelocity(sub_ent, velocity);
            SetEntityNextThink(launcher, get_gametime()+fuser[launcher][2]);
        }

        public SnakeProtectBulletTouch(ent, client) {
            new owner=GetEntityOwner(ent);

            if((1<=client<=MaxClients) && (owner!=client) && (owner==0 || cs_get_user_team(client)!=cs_get_user_team(owner))) {
                ExecuteHamB(Ham_TakeDamage, client, 0, owner, entity_damage[ent], BULLET_DMG_TYPE);

                entity_set_int(ent, EV_INT_solid, SOLID_NOT);
                set_pev(ent, pev_effects, pev(ent, pev_effects) | EF_NODRAW);
            }

        }
   