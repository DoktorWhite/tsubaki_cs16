#if defined _tsubaki_bullet
    #endinput
#endif
#define _tsubaki_bullet

#include <amxmodx>
#include <amxmisc>
#include <cstrike>
#include <engine>
#include <fakemeta>
#include <fakemeta_util>
#include <fun>
#include <hamsandwich>
#include <msgstocks>

#include <tsubaki_skills/tsubaki_common>

#pragma semicolon 1

new bool:BULLET_REGISTERED = false;     //Is Touch and Think being registered?

/*  Bullet Name (Allcoate memory first to reduce memory allocation)  */
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


/*  Checking Functions  */

    #define CheckTsubakiBulletRegistration()    if(!BULLET_REGISTERED) RegisterTsubakiBulletStuffs()

    //為實體設定為彈幕
    #define SetAsTsubakiBullet(%0) entity_set_edict(%0, EV_ENT_euser3, TSUBAKI_ENTITY_BULLET))
    #define IsTsubakiBullet(%0) (entity_get_edict(%0, EV_ENT_euser3)==TSUBAKI_ENTITY_BULLET))



/*  Bullet Properties  */
    stock const BULLET_DMG_TYPE = DMG_CLUB;
    #define DEFAULT_BULLET_DMG          8.0
    #define DEFAULT_BULLET_THINKTIME    0.1
    #define DEFAULT_BULLET_SOLID        SOLID_TRIGGER

    stock Float:BULLET_MIN_SIZE[3] = {-16.0,-16.0,-16.0};
    stock Float:BULLET_MAX_SIZE[3] = { 16.0, 16.0, 16.0};

/*  Initiation  */

    stock RegisterTsubakiBulletStuffs() {
        register_touch(NORMAL_BULLET, "player", "NormalBulletTouch");

        register_touch(AVERAGE_CHANGE_SPEED_BULLET, "player", "NormalBulletTouch");
        register_think(AVERAGE_CHANGE_SPEED_BULLET, "AverageChangeSpeedBulletThink");

        register_touch(MOVE_AND_SPIN_ONLY_BULLET, "player", "NormalBulletTouch");
        register_think(MOVE_AND_SPIN_ONLY_BULLET, "MoveAndSpinOnlyBulletThink");

        register_touch(SPIN_AWAY_BULLET, "player", "NormalBulletTouch");
        register_think(SPIN_AWAY_BULLET, "SpinAwayBulletThink");
        
        register_touch(SPIN_AWAY_SPLIT_BULLET, "player", "NormalBulletTouch");
        register_think(SPIN_AWAY_SPLIT_BULLET, "SpinAwaySplitBulletThink");
        
        register_think(RANDOM_MACHINE_BULLET_LAUNCHER, "AllRandomMachineBulletLaunch");

        register_touch(ROUND_SPIN_BULLET, "player", "NormalBulletTouch");
        register_think(ROUND_SPIN_BULLET, "RoundSpinBulletThink");

        register_touch(CHANGE_DEST_BULLET, "player", "NormalBulletTouch");
        register_think(CHANGE_DEST_BULLET, "ChangeDestBulletThink");

        register_touch(CHANGE_ANGLE_BULLET, "player", "NormalBulletTouch");
        register_think(CHANGE_ANGLE_BULLET, "ChangeAngleBulletThink");

        register_think(ROTATE_MACHINE_BULLET_LAUNCHER, "RotateMachineBulletLauncherThink");

        register_touch(SPLIT_STRAIGHT_BULLET, "player", "NormalBulletTouch");
        register_think(SPLIT_STRAIGHT_BULLET, "SplitStraightBulletThink");

        register_touch(APPEAR_AND_DISAPPER_BULLET, "player", "NormalBulletTouch");
        register_think(APPEAR_AND_DISAPPER_BULLET, "AppearAndDisapperBulletThink");

        register_touch(NON_PENETRATE_BULLET, "", "NonPenetrateBulletTouch");

        register_touch(SPLIT_BACKWARD_BULLET, "player", "NormalBulletTouch");
        register_think(SPLIT_BACKWARD_BULLET, "SplitBackwardBulletThink");

        register_think(SLOW_CHANGE_DIR_BULLET_LAUNCHER, "SlowChangeDirBulletLauncherThink");

        register_touch(SLOW_CHANGE_DIR_BULLET, "player", "NormalBulletTouch");
        register_think(SLOW_CHANGE_DIR_BULLET, "SlowChangeDirBulletThink");
        
        register_touch(EXPLOSIVE_BULLET, "player", "OnExplosiveBulletTouch");
        register_think(EXPLOSIVE_BULLET, "ExplosiveBulletThink");

        register_touch(SWIRL_BULLET, "player", "NormalBulletTouch");
        register_think(SWIRL_BULLET, "SwirlBulletThink");

        register_touch(TRACER_BULLET, "player", "NormalBulletTouch");
        register_think(TRACER_BULLET, "TracerBulletThink");

        register_think(SNAKE_PROTECT_CONTROLLER, "SnakeProtectControllerThink");
        register_touch(SNAKE_PROTECT_BULLET, "player", "SnakeProtectBulletTouch");

        BULLET_REGISTERED = true;
        //register_touch(, "player", "NormalBulletTouch");
        //register_think(, "");
    }

/*  Touch  */

    public NormalBulletTouch(ent, client) {
        static owner;

        if((1<=client<=MaxClients) && (owner=entity_get_edict(ent, EV_ENT_owner))!=client && (owner==0 || cs_get_user_team(client)!=cs_get_user_team(owner))) {
            ExecuteHamB(Ham_TakeDamage, client, ent, owner, GetTsubakiEntityDamage(ent), BULLET_DMG_TYPE);

            RemoveTsubakiEntity(ent);
        }
    }

    public NonPenetrateBulletTouch(ent, client) {
        static owner;
        
        if((owner=GetEntityOwner(ent))==client)
            return;

        if((1<=client<=MaxClients) && (owner==0 || cs_get_user_team(client)!=cs_get_user_team(owner))) {
            ExecuteHamB(Ham_TakeDamage, client, 0, owner, GetTsubakiEntityDamage(ent), BULLET_DMG_TYPE);
        }
        
        RemoveTsubakiEntity(ent);
    }


/*  Non-Penetrate Bullet  */

  
    /*
    * 發射一粒不穿透的的彈幕
    *
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param angles 運動角度 (Pitch, Yaw, Roll) (degree)
    * @param owner 主人
    * @param speed 彈幕速度
    * @param last_distance 移動距離
    * @param colors 彈幕顏色
    * @param damage 彈幕傷害
    *
    */  
    stock FireNonPenetrateBullet(const bullet_model[], Float:origin[3], Float:angles[3], owner, Float:speed=200.0, Float:last_distance=0.0, 
                                            colors[4]={200,200,0,16}, Float:damage=DEFAULT_BULLET_DMG)
    {
        CheckTsubakiBulletRegistration();

        new ent = CreateTsubakiEntity(NON_PENETRATE_BULLET
                            ,bullet_model
                            ,.solid_type=DEFAULT_BULLET_SOLID
                            ,.movetype=MOVETYPE_FLYMISSILE
                            ,.owner=owner
                            ,.r=colors[0]
                            ,.g=colors[1]
                            ,.b=colors[2]
                            ,.a=colors[3]
                            ,.min_size=BULLET_MIN_SIZE
                            ,.max_size=BULLET_MAX_SIZE);
        
        if(ent == -1)
            return;

        angles[0] = ToRadian(angles[0]);
        angles[1] = ToRadian(angles[1]);

        tbk_vuser[ent][0][0] = speed * Cosine(angles[0]) * Cosine(angles[1]);
        tbk_vuser[ent][0][1] = speed * Cosine(angles[0]) *   Sine(angles[1]);
        tbk_vuser[ent][0][2] = speed *   Sine(angles[0]);

        TeleportEntity(ent, origin);
        SetEntityVelocity(ent, tbk_vuser[ent][0]);
        SetTsubakiEntityDamage(ent, damage);

        SetRemoveEntityTask(ent, (last_distance>0.0)?last_distance/speed:20.0);
    }


/*  Average Bullet  */

  
    /*
    * 以平均角度生成以{speed}移動的彈幕
    *
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param bullet_amount 生成數
    * @param owner 主人
    * @param speed 彈幕速度
    * @param last_time 存在時間
    * @param angle_offset 生成角度偏差(degree)
    * @param colors 彈幕顏色
    * @param damage 彈幕傷害
    *
    */  
    stock AverageBullet(const bullet_model[], Float:origin[3], bullet_amount=8, owner=0, 
        Float:speed=100.0, Float:last_time=4.0, Float:angle_offset=0.0, colors[4]={200, 0, 0, 16}, Float:damage=DEFAULT_BULLET_DMG)
    {
        CheckTsubakiBulletRegistration();

        if(bullet_amount <= 0)
            return;

        new ent, i;
        new Float:velocity[3];
        
        angle_offset = ToRadian(angle_offset);

        for(i=0; i<bullet_amount; i++)
        {
            ent = CreateTsubakiEntity(NORMAL_BULLET
                                        ,bullet_model
                                        ,.solid_type=DEFAULT_BULLET_SOLID
                                        ,.movetype=MOVETYPE_NOCLIP
                                        ,.owner=owner 
                                        ,.r=colors[0] 
                                        ,.g=colors[1] 
                                        ,.b=colors[2]
                                        ,.a=colors[3]
                                        ,.min_size=BULLET_MIN_SIZE
                                        ,.max_size=BULLET_MAX_SIZE);

            if(ent == -1)
                continue;
            
            velocity[0] = speed * Cosine(angle_offset);
            velocity[1] = speed *   Sine(angle_offset);

            TeleportEntity(ent, origin);
            SetEntityVelocity(ent, velocity);
            SetTsubakiEntityDamage(ent, damage);

            SetRemoveEntityTask(ent, last_time);

            angle_offset += M_2PI / bullet_amount;
        }
    }

/*  Avarage Change Speed Bullet  */


    /*
    * 生成時以{spd1} 移動，{delay}秒後以{spd2}移動
    *
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param owner 主人
    * @param bullet_amount 生成數
    * @param spd1 起始速度
    * @param spd2 終止速度
    * @param angle_offset 生成角度偏差(degree)
    * @param delay N秒後速度改為 spd2
    * @param last_time 存在時間
    * @param colors 彈幕顏色
    * @param damage 彈幕傷害
    *
    */
    stock AverageChangeSpeedBullet(const bullet_model[], Float:origin[3], owner=0, bullet_amount=8,
                                        Float:spd1=50.0, Float:spd2=300.0, Float:angle_offset=0.0,
                                        Float:delay=2.0, Float:last_time=7.0,
                                        colors[4]={0,0,255,16}, Float:damage=DEFAULT_BULLET_DMG)
    {
        CheckTsubakiBulletRegistration();

        new ent, i=0;
        new Float:velocity[3] = {0.0, ...};
        angle_offset = ToRadian(angle_offset);

        for(; i<bullet_amount; i++)
        {
            ent = CreateTsubakiEntity(AVERAGE_CHANGE_SPEED_BULLET
                                        ,bullet_model
                                        ,.solid_type=DEFAULT_BULLET_SOLID
                                        ,.movetype=MOVETYPE_NOCLIP
                                        ,.owner=owner
                                        ,.r=colors[0]
                                        ,.g=colors[1]
                                        ,.b=colors[2]
                                        ,.a=colors[3]
                                        ,.min_size=BULLET_MIN_SIZE
                                        ,.max_size=BULLET_MAX_SIZE);

            if(ent == -1)
                continue;

            velocity[0] = spd1 * Cosine(angle_offset);
            velocity[1] = spd1 *   Sine(angle_offset);

            tbk_vuser[ent][0][0] = spd2 * Cosine(angle_offset);
            tbk_vuser[ent][0][1] = spd2 *   Sine(angle_offset);
            tbk_vuser[ent][0][2] = 0.0;

            TeleportEntity(ent, origin);
            SetEntityVelocity(ent, velocity);
            SetTsubakiEntityDamage(ent, damage);

            entity_set_float(ent, EV_FL_nextthink, get_gametime()+delay);
            SetRemoveEntityTask(ent, last_time);

            angle_offset += M_2PI/bullet_amount;
        }
    }								

        public AverageChangeSpeedBulletThink(ent) {
            entity_set_vector(ent, EV_VEC_velocity, tbk_vuser[ent][0]);
        }

/*  Move And Spin Only Bullet */
   
    /*
    * 生成後在{time_to_radius}秒後移動至{target_radius}距離，再以{angular_spd} 旋轉
    * 彈幕消失時間為{time_to_radius}+{last_time}
    *
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param owner 主人
    * @param bullet_amount 生成數
    * @param target_radius 半徑
    * @param angle_offset 生成角度偏差(degree)
    * @param angular_spd 每秒角速度(degree)
    * @param time_to_radius 抵達半徑所需時間
    * @param last_time 存在時間
    * @param colors 彈幕顏色
    * @param damage 彈幕傷害
    *
    */
    stock MoveAndSpinOnlyBullet(const bullet_model[], Float:origin[3], owner=0, bullet_amount=8,
                                    Float:target_radius=100.0, Float:angle_offset=0.0, Float:angular_spd=45.0,
                                    Float:time_to_radius=1.0, Float:last_time=8.0, 
                                    colors[4]={30,144,255,16}, Float:damage=DEFAULT_BULLET_DMG )
    {
        CheckTsubakiBulletRegistration();

        new i=0, ent;
        target_radius /= time_to_radius;		//target_radius become spd
        angle_offset = ToRadian(angle_offset);
        angular_spd = ToRadian(angular_spd);

        for(; i<bullet_amount; i++)
        {
            ent=CreateTsubakiEntity(MOVE_AND_SPIN_ONLY_BULLET
                                    ,bullet_model
                                    ,.solid_type=DEFAULT_BULLET_SOLID
                                    ,.movetype=MOVETYPE_NOCLIP
                                    ,.owner=owner
                                    ,.r=colors[0]
                                    ,.g=colors[1]
                                    ,.b=colors[2]
                                    ,.a=colors[3]
                                    ,.min_size=BULLET_MIN_SIZE
                                    ,.max_size=BULLET_MAX_SIZE);
            
            if(ent==-1)
                continue;

            tbk_vuser[ent][0][0] = target_radius * Cosine(angle_offset);
            tbk_vuser[ent][0][1] = target_radius *   Sine(angle_offset);
            tbk_vuser[ent][0][2] = 0.0;

            tbk_fuser[ent][0] = angle_offset + M_PI/2.0;		//current_angle
            tbk_fuser[ent][1] = angular_spd;			//angular_spd
            tbk_fuser[ent][2] = target_radius;		//spd

            TeleportEntity(ent, origin);
            SetEntityVelocity(ent, tbk_vuser[ent][0]);
            SetTsubakiEntityDamage(ent, damage);

            set_task_ex(time_to_radius-DEFAULT_BULLET_THINKTIME, "MoveAndSpinOnlyBulletPrepare", TSUBAKI_TASKID_ENTITY_TASK(ent), _, _, SetTask_Once);
            SetRemoveEntityTask(ent, last_time);

            angle_offset += M_2PI / bullet_amount;
        }
    }

        public MoveAndSpinOnlyBulletPrepare(taskid)
        {
            SetEntityNextThink(taskid-TSUBAKI_TASKID_ENTITY_TASK_BASE, get_gametime()+DEFAULT_BULLET_THINKTIME);
        }

        public MoveAndSpinOnlyBulletThink(ent)
        {
            tbk_vuser[ent][0][0] = tbk_fuser[ent][2] * tbk_fuser[ent][1] * Cosine(tbk_fuser[ent][0]);
            tbk_vuser[ent][0][1] = tbk_fuser[ent][2] * tbk_fuser[ent][1] *   Sine(tbk_fuser[ent][0]);

            tbk_fuser[ent][0] += tbk_fuser[ent][1] * DEFAULT_BULLET_THINKTIME;
            SetEntityNextThink(ent, get_gametime() + DEFAULT_BULLET_THINKTIME);
            SetEntityVelocity(ent, tbk_vuser[ent][0]);
        }

/* Spin Away Bullet */
   
    /*
    * 生成後以 {angular_spd}旋轉，並以每秒{radius_increase_per_second}單位距離遠離中心點
    *
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param owner 主人
    * @param bullet_amount 生成數
    * @param initial_radius 生成時與中心點半徑距離
    * @param radius_increase_per_second 每秒遠離中心點的單位距離
    * @param angle_offset 生成角度偏差(degree)
    * @param angular_spd 每秒旋轉角速度(degree)
    * @param last_time 存在時間
    * @param colors 彈幕顏色
    * @param damage 彈幕傷害
    *
    *
    */
    stock SpinAwayBullet(const bullet_model[], Float:origin[3], owner=0, bullet_amount=8, 
                            Float:initial_radius=50.0, Float:radius_increase_per_second=50.0,
                            Float:angle_offset=0.0, Float:angular_spd=30.0, Float:last_time=8.0,
                            colors[4]={64,224,208,16}, Float:damage=DEFAULT_BULLET_DMG)
    {
        CheckTsubakiBulletRegistration();

        new i=0, ent;
        new Float:velocity[3];

        angle_offset = ToRadian(angle_offset);
        angular_spd = ToRadian(angular_spd);
        radius_increase_per_second *= DEFAULT_BULLET_THINKTIME;

        for(; i<bullet_amount; i++)
        {
            ent=CreateTsubakiEntity(SPIN_AWAY_BULLET
                                    ,bullet_model
                                    ,.solid_type=DEFAULT_BULLET_SOLID
                                    ,.movetype=MOVETYPE_NOCLIP, .owner=owner
                                    ,.r=colors[0]
                                    ,.g=colors[1]
                                    ,.b=colors[2]
                                    ,.a=colors[3]
                                    ,.min_size=BULLET_MIN_SIZE
                                    ,.max_size=BULLET_MAX_SIZE);
            
            
            if(ent==-1)
                continue;
                
            velocity[0] = initial_radius * angular_spd * Cosine(angle_offset);
            velocity[1] = initial_radius * angular_spd *   Sine(angle_offset);

            tbk_fuser[ent][0] = initial_radius;
            tbk_fuser[ent][1] = radius_increase_per_second;		//already *= bullet_thinktime
            tbk_fuser[ent][2] = angle_offset;
            tbk_fuser[ent][3] = angular_spd;

            TeleportEntity(ent, origin);
            SetEntityVelocity(ent, velocity);
            SetTsubakiEntityDamage(ent, damage);

            SetEntityNextThink(ent, get_gametime()+DEFAULT_BULLET_THINKTIME);
            SetRemoveEntityTask(ent, last_time);

            angle_offset += M_2PI / bullet_amount;
        }
    }

        public SpinAwayBulletThink(ent)
        {
            tbk_fuser[ent][0] += tbk_fuser[ent][1];
            tbk_fuser[ent][2] += tbk_fuser[ent][3] * DEFAULT_BULLET_THINKTIME;

            tbk_vuser[ent][0][0] = tbk_fuser[ent][0] * tbk_fuser[ent][3] * Cosine(tbk_fuser[ent][2]);
            tbk_vuser[ent][0][1] = tbk_fuser[ent][0] * tbk_fuser[ent][3] *   Sine(tbk_fuser[ent][2]);
            tbk_vuser[ent][0][2] = 0.0;
            
            SetEntityVelocity(ent, tbk_vuser[ent][0]);
            SetEntityNextThink(ent, get_gametime()+DEFAULT_BULLET_THINKTIME);
        }

/*  Spin Away Split Bullet  */
    /*
    * 生成後以 {angular_spd}旋轉，並以每秒{radius_increase_per_second}單位距離遠離中心點
    * 同時，每{split_interval}物分裂出{split_amount}粒維持{split_bullet_last_time}秒的彈幕
    *
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param owner 主人
    * @param bullet_amount 生成數
    * @param initial_radius 生成時與中心點半徑距離
    * @param radius_increase_per_second 每秒遠離中心點的單位距離
    * @param angle_offset 生成角度偏差(degree)
    * @param angular_spd 每秒旋轉角速度(degree)
    * @param last_time 存在時間
    * @param split_amount 每次分裂出來的彈幕數
    * @param split_angle 分裂的彈幕與母體的運動角度偏差(degree)
    * @param split_bullet_spd 分裂的彈幕速度
    * @param split_bullet_last_time 分裂的彈幕存在時間
    * @param split_interval 每次分裂所需時間
    * @param colors 母體彈幕顏色
    * @param split_colors 分裂彈幕的顏色
    * @param damage 母體傷害
    * @param split_damage 分裂彈幕傷害
    *
    */
    stock SpinAwaySplitBullet(const bullet_model[], Float:origin[3], owner=0, bullet_amount=3, 
                                    Float:initial_radius=10.0, Float:radius_increase_per_second=100.0, 
                                    Float:angle_offset=0.0, Float:angular_spd=45.0, Float:last_time=12.0,
                                    split_amount=1, Float:split_angle=-90.0, Float:split_bullet_spd=150.0, 
                                    Float:split_bullet_last_time=7.0, Float:split_interval=1.0, 
                                    colors[4]={153,0,153,16}, split_colors[4]={153,0,153,16},
                                    Float:damage=DEFAULT_BULLET_DMG, 
                                    Float:split_damage=DEFAULT_BULLET_DMG)
    {
        CheckTsubakiBulletRegistration();

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
            ent=CreateTsubakiEntity(SPIN_AWAY_SPLIT_BULLET
                                ,bullet_model
                                ,.solid_type=DEFAULT_BULLET_SOLID
                                ,.movetype=MOVETYPE_NOCLIP
                                ,.owner=owner
                                ,.r=colors[0]
                                ,.g=colors[1]
                                ,.b=colors[2]
                                ,.a=colors[3]
                                ,.min_size=BULLET_MIN_SIZE
                                ,.max_size=BULLET_MAX_SIZE);
            
            if(ent==-1)
                continue;

            spawn_origin[0] = origin[0] + initial_radius * Cosine(angle_offset);
            spawn_origin[1] = origin[1] + initial_radius *   Sine(angle_offset);
            spawn_origin[2] = origin[2];

            velocity[0] = initial_radius * angular_spd * Cosine(angle_offset);
            velocity[1] = initial_radius * angular_spd *   Sine(angle_offset);

            tbk_iuser[ent][0] = split_amount;
            tbk_iuser[ent][1] = rgba;
            tbk_iuser[ent][2] = 0;          //End Counter
            tbk_iuser[ent][3] = RoundFloat(split_interval/DEFAULT_BULLET_THINKTIME);

            tbk_fuser[ent][0] = initial_radius;
            tbk_fuser[ent][1] = radius_increase_per_second;		//Already *= thinktime
            tbk_fuser[ent][2] = angle_offset;
            tbk_fuser[ent][3] = angular_spd;
            tbk_fuser[ent][4] = split_damage;

            CopyVector(tbk_vuser[ent][0], split_bullet_data);

            TeleportEntity(ent, spawn_origin);
            SetEntityVelocity(ent, velocity);
            SetTsubakiEntityDamage(ent, damage);

            SetEntityNextThink(ent, DEFAULT_BULLET_THINKTIME);
            SetRemoveEntityTask(ent, last_time);

            angle_offset += M_2PI / bullet_amount;
        }
    }

        public SpinAwaySplitBulletThink(ent)
        {
            tbk_fuser[ent][0] += tbk_fuser[ent][1];
            tbk_fuser[ent][2] += tbk_fuser[ent][3]*DEFAULT_BULLET_THINKTIME;

            if((++tbk_iuser[ent][2])%tbk_iuser[ent][3] == 0)
            {
                new sub_ent, i=0, owner=GetEntityOwner(ent), amount=tbk_iuser[ent][0];
                new r = GetRGB_R(tbk_iuser[ent][1]), g = GetRGB_G(tbk_iuser[ent][1]), b = GetRGB_B(tbk_iuser[ent][1]), a = GetRGBA_A(tbk_iuser[ent][1]);
                new Float:origin[3], Float:velocity[3], Float:angle = tbk_fuser[ent][2]+tbk_vuser[ent][0][0] , Float:spd = tbk_vuser[ent][0][1], Float:lasttime = tbk_vuser[ent][0][2], Float:damage = tbk_fuser[ent][4];
                new model_route[128];

                entity_get_string(ent, EV_SZ_model, model_route, sizeof(model_route));
                GetEntityOrigin(ent, origin);

                for(; i<amount; i++)
                {
                    sub_ent=CreateTsubakiEntity(NORMAL_BULLET
                                            ,model_route
                                            ,.solid_type=DEFAULT_BULLET_SOLID
                                            ,.movetype=MOVETYPE_NOCLIP
                                            ,.owner=owner
                                            ,.r=r
                                            ,.g=g
                                            ,.b=b
                                            ,.a=a
                                            ,.min_size=BULLET_MIN_SIZE
                                            ,.max_size=BULLET_MAX_SIZE);
                    
                    if(sub_ent==-1)
                        continue;

                    velocity[0] = spd * Cosine(angle);
                    velocity[1] = spd *   Sine(angle);

                    TeleportEntity(sub_ent, origin);
                    SetEntityVelocity(sub_ent, velocity);
                    SetTsubakiEntityDamage(sub_ent, damage);

                    SetRemoveEntityTask(sub_ent, lasttime);

                    angle += M_2PI / amount;
                }
            }

            tbk_vuser[ent][1][0] = tbk_fuser[ent][0] * tbk_fuser[ent][3] * Cosine(tbk_fuser[ent][2]);
            tbk_vuser[ent][1][1] = tbk_fuser[ent][0] * tbk_fuser[ent][3] *   Sine(tbk_fuser[ent][2]);
            tbk_vuser[ent][1][2] = 0.0;

            SetEntityVelocity(ent, tbk_vuser[ent][1]);
            SetEntityNextThink(ent, get_gametime()+DEFAULT_BULLET_THINKTIME);
        }


/*  Full Random Bullet  */

    /*
    * 生成隨機角度的方向前進及以隨機速度前進的彈幕【一斉発射】
    *　
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param owner 主人
    * @param bullet_amount 生成數
    * @param min_spd 隨機速度最少值
    * @param min_spd 隨機速度最大值
    * @param last_distance 彈幕最大移動距離
    * @param colors 彈幕顏色
    * @param damage 彈幕傷害
    *
    */
    stock FullRandomBullet(const bullet_model[], Float:origin[3], owner=0, bullet_amount=64, Float:min_spd=100.0, Float:max_spd=600.0,
                                Float:last_distance=1000.0, colors[4]={100,0,255,16}, Float:damage=5.0)
    {
        CheckTsubakiBulletRegistration();

        new i, ent;
        new Float:spd, Float:angle, Float:velocity[3], Float:spawn_origin[3];

        for(i=0; i<bullet_amount; i++)
        {
            ent=CreateTsubakiEntity(NORMAL_BULLET
                                ,bullet_model
                                ,.solid_type=DEFAULT_BULLET_SOLID
                                ,.movetype=MOVETYPE_NOCLIP
                                ,.owner=owner
                                ,.r=colors[0]
                                ,.g=colors[1]
                                ,.b=colors[2]
                                ,.a=colors[3]
                                ,.min_size=BULLET_MIN_SIZE
                                ,.max_size=BULLET_MAX_SIZE);
            
            if( ent==-1 )
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
            SetTsubakiEntityDamage(ent, damage);

            SetRemoveEntityTask(ent, last_distance/spd);
        }
    }


/*  All Random Machine Bullet  */


    /*
    * 生成隨機角度的方向前進及以隨機速度前進的彈幕【連続発射】
    *　
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param owner 主人
    * @param min_bullet_amount 每次生成最小數
    * @param max_bullet_amount 每次生成最大數
    * @param min_spd 隨機速度最少值
    * @param min_spd 隨機速度最大值
    * @param last_distance 彈幕最大移動距離
    * @param fire_rate 每 n 秒發射一次(最小0.1)
    * @param last_time 發射總時間
    * @param colors 彈幕顏色
    * @param damage 彈幕傷害
    *
    */
    stock AllRandomMachineBullet(const model_route[], Float:origin[3], owner, 
        min_bullet_amount=2, max_bullet_amount=6, 
        Float:min_spd=100.0, Float:max_spd=500.0, Float:bullet_last_distance=1000.0,
        Float:fire_rate=0.1, Float:last_time=8.0,
        colors[4]={100,0,255,16}, Float:damage=DEFAULT_BULLET_DMG)
    {
        CheckTsubakiBulletRegistration();

        new ent = CreateInvisibleLauncher(RANDOM_MACHINE_BULLET_LAUNCHER, 
                                            model_route,
                                            .owner=owner);

        if(ent==-1)
            return;

        tbk_iuser[ent][0] = min_bullet_amount;
        tbk_iuser[ent][1] = max_bullet_amount;
        tbk_iuser[ent][2] = CreateRGBAInt(colors[0], colors[1], colors[2], colors[3]);

        tbk_fuser[ent][0] = min_spd;
        tbk_fuser[ent][1] = max_spd;
        tbk_fuser[ent][2] = bullet_last_distance;
        tbk_fuser[ent][3] = fire_rate;

        TeleportEntity(ent, origin);
        SetEntityNextThink(ent, get_gametime()+fire_rate);
        SetTsubakiEntityDamage(ent, damage);

        SetRemoveEntityTask(ent, last_time);
    }

        public AllRandomMachineBulletLaunch(ent)
        {
            new sub_ent, owner=GetEntityOwner(ent);
            new r=GetRGB_R(tbk_iuser[ent][2]), g=GetRGB_G(tbk_iuser[ent][2]), b=GetRGB_B(tbk_iuser[ent][2]), a=GetRGBA_A(tbk_iuser[ent][2]);
            new i=0, bullet_amount = GetRandomInt(tbk_iuser[ent][0], tbk_iuser[ent][1]);
            new Float:origin[3], Float:angle, Float:velocity[3], Float:speed, Float:lasttime, Float:damage=GetTsubakiEntityDamage(ent);
            new bullet_model[128];
            entity_get_string(ent, EV_SZ_model, bullet_model, charsmax(bullet_model));
            GetEntityOrigin(ent, origin);

            for(; i<bullet_amount; i++)
            {
                sub_ent=CreateTsubakiEntity(NORMAL_BULLET
                                        ,bullet_model
                                        ,.solid_type=DEFAULT_BULLET_SOLID
                                        ,.movetype=MOVETYPE_NOCLIP
                                        ,.owner=owner
                                        ,.r=r
                                        ,.g=g
                                        ,.b=b
                                        ,.a=a
                                        ,.min_size=BULLET_MIN_SIZE
                                        ,.max_size=BULLET_MAX_SIZE);
                if(sub_ent == -1)
                    continue;

                angle = GetRandomFloat(0.0, M_2PI);
                speed = GetRandomFloat(tbk_fuser[ent][0], tbk_fuser[ent][1]);
                lasttime = tbk_fuser[ent][2] / speed;

                velocity[0] = speed * Cosine(angle);
                velocity[1] = speed *   Sine(angle);
                velocity[2] = 0.0;

                TeleportEntity(sub_ent, origin);
                SetEntityVelocity(sub_ent, velocity);
                SetTsubakiEntityDamage(sub_ent, damage);
                
                SetRemoveEntityTask(sub_ent, lasttime);
            }

            SetEntityNextThink(ent, get_gametime()+tbk_fuser[ent][3]);
        }


/* Rotate Machine Bullet */

    /*
    * 連続発射直線前進的彈幕
    *　
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param owner 主人
    * @param bullet_per_fire 每次生成數
    * @param fire_interval 每 n 秒發射一次(最小0.1)
    * @param bullet_speed 彈幕速度
    * @param angle_offset 生成時角度偏差
    * @param angular_spd 每次發射角度變化
    * @param total_rotation 總角度變化
    * @param last_time 彈幕維持時間
    * @param colors 彈幕顏色
    * @param damage 彈幕傷害
    *
    */
    stock RotateMachineBullet(const bullet_model[], Float:origin[3], owner=0, 
        bullet_per_fire=2, Float:fire_interval = 0.1, Float:bullet_speed=180.0,
        Float:angle_offset=0.0, Float:angular_spd=10.0, Float:total_rotation=180.0, 
        Float:last_time=4.0, colors[4]={255,0,255,16}, Float:damage=DEFAULT_BULLET_DMG)
    {
        CheckTsubakiBulletRegistration();
        bullet_per_fire = Clamp(bullet_per_fire, 0, 5);

        new ent = CreateInvisibleLauncher(.clsname=ROTATE_MACHINE_BULLET_LAUNCHER, 
                                            .model_route=bullet_model, 
                                            .owner=owner);

        if(ent == -1)
            return;

        tbk_iuser[ent][0] = bullet_per_fire;
        tbk_iuser[ent][1] = CreateRGBAIntByArray(colors);
        tbk_iuser[ent][2] = 0;
        tbk_iuser[ent][3] = abs(RoundFloat(total_rotation/angular_spd));

        tbk_fuser[ent][0] = fire_interval;
        tbk_fuser[ent][1] = bullet_speed;
        tbk_fuser[ent][2] = last_time;
        tbk_fuser[ent][3] = ToRadian(angle_offset);
        tbk_fuser[ent][4] = ToRadian(angular_spd);

        TeleportEntity(ent, origin);
        SetTsubakiEntityDamage(ent, damage);
        
        RotateMachineBulletLauncherThink(ent);
    }

        public RotateMachineBulletLauncherThink(ent)
        {
            new sub_ent, i=0, bullet_model[128], Float:damage=GetTsubakiEntityDamage(ent), Float:origin[3], Float:velocity[3], Float:angle;
            GetEntityModel(ent, bullet_model);
            GetEntityOrigin(ent, origin);

            for(; i<tbk_iuser[ent][0]; i++)
            {
                sub_ent=CreateTsubakiEntity(NORMAL_BULLET
                                ,bullet_model
                                ,.solid_type=DEFAULT_BULLET_SOLID
                                ,.movetype=MOVETYPE_NOCLIP
                                ,.owner=GetEntityOwner(ent)
                                ,.r= GetRGB_R(tbk_iuser[ent][1])
                                ,.g= GetRGB_G(tbk_iuser[ent][1])
                                ,.b= GetRGB_B(tbk_iuser[ent][1])
                                ,.a=GetRGBA_A(tbk_iuser[ent][1])
                                ,.min_size=BULLET_MIN_SIZE
                                ,.max_size=BULLET_MAX_SIZE);
                
                if(sub_ent == -1)
                    continue;

                angle = tbk_fuser[ent][3] + M_2PI * i / tbk_iuser[ent][0];

                //Velocity
                velocity[0] = tbk_fuser[ent][1] * Cosine(angle);
                velocity[1] = tbk_fuser[ent][1] *   Sine(angle);
                velocity[2] = 0.0;
                
                TeleportEntity(sub_ent, origin);
                SetEntityVelocity(sub_ent, velocity);
                SetTsubakiEntityDamage(sub_ent, damage);

                SetRemoveEntityTask(sub_ent, tbk_fuser[ent][2]);
            }

            if(++tbk_iuser[ent][2] < tbk_iuser[ent][3]) {       //Still Rotate?
            
                tbk_fuser[ent][3] += tbk_fuser[ent][4];             //Add Angle
                SetEntityNextThink(ent, get_gametime()+tbk_fuser[ent][0]);
            }
            else {
                RemoveTsubakiEntity(ent);
                return;
            }
        }

/*  Machine Slow Change Dir Bullet  */


    /*
    * 連続発射直線前進，並以每秒{deceleration}減速的彈幕
    * 當彈幕速度歸0時，
    * 向本來運動角度相差90度~270度的隨機角度以每秒{deceleration}加速
    *　
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param owner 主人
    * @param bullet_per_fire 每次生成數
    * @param total_fire 總發射次數
    * @param initial_angle 生成時角度
    * @param angular_spd 每次發射角度變化
    * @param fire_rate 每 n 秒發射一次(最小0.1)
    * @param spd 彈幕初速
    * @param deceleration 每秒減速・每秒加速
    * @param bullet_last_time 彈幕維持時間
    * @param colors 彈幕顏色
    * @param damage 彈幕傷害
    *
    */
    stock LaunchSlowChangeDirBullet(const bullet_model[], Float:origin[3], owner=0, 
                                        bullet_per_fire=2, total_fire=36,
                                        Float:initial_angle=0.0, Float:angular_spd=10.0,
                                        Float:fire_rate=0.1, Float:spd=300.0, Float:deceleration=80.0, Float:bullet_last_time=10.0, 
                                        colors[4]={255,178,102,16}, Float:damage=DEFAULT_BULLET_DMG)
    {
        CheckTsubakiBulletRegistration();
        new ent = CreateInvisibleLauncher(SLOW_CHANGE_DIR_BULLET_LAUNCHER, bullet_model, .owner=owner);

        if(ent == -1)
            return;

        tbk_iuser[ent][0] = 0;
        tbk_iuser[ent][1] = total_fire;
        tbk_iuser[ent][2] = CreateRGBAIntByArray(colors);
        tbk_iuser[ent][3] = RoundFloat(fire_rate*1000);
        tbk_iuser[ent][4] = bullet_per_fire;

        tbk_fuser[ent][0] = spd;
        tbk_fuser[ent][1] = deceleration * DEFAULT_BULLET_THINKTIME;
        tbk_fuser[ent][2] = bullet_last_time;
        tbk_fuser[ent][3] = ToRadian(initial_angle);
        tbk_fuser[ent][4] = ToRadian(angular_spd);

        SetTsubakiEntityDamage(ent, damage);

        TeleportEntity(ent, origin);
        SlowChangeDirBulletLauncherThink(ent);
    }

        public SlowChangeDirBulletLauncherThink(ent)
        {
            new sub_ent, r=GetRGB_R(tbk_iuser[ent][2]), g=GetRGB_G(tbk_iuser[ent][2]), b=GetRGB_B(tbk_iuser[ent][2]), a=GetRGBA_A(tbk_iuser[ent][2]), owner=GetEntityOwner(ent);
            new i=0, bullet_amount=tbk_iuser[ent][4], bullet_model[128];
            new Float:origin[3], Float:velocity[3], Float:spd=tbk_fuser[ent][0], Float:angle=tbk_fuser[ent][3]+tbk_iuser[ent][0]*tbk_fuser[ent][4], Float:damage=GetTsubakiEntityDamage(ent);
            
            GetEntityModel(ent, bullet_model);

            for(; i<bullet_amount; i++) {
                
                sub_ent=CreateTsubakiEntity(SLOW_CHANGE_DIR_BULLET
                                            ,bullet_model
                                            ,.solid_type=DEFAULT_BULLET_SOLID
                                            ,.movetype=MOVETYPE_NOCLIP
                                            ,.owner=owner
                                            ,.r=r
                                            ,.g=g
                                            ,.b=b
                                            ,.a=a
                                            ,.min_size=BULLET_MIN_SIZE
                                            ,.max_size=BULLET_MAX_SIZE);

                if(sub_ent == -1)
                    continue;

                GetEntityOrigin(ent, origin);
                velocity[0] = spd * Cosine(angle);
                velocity[1] = spd *   Sine(angle);

                tbk_iuser[sub_ent][0] = 0;		//phase

                tbk_fuser[sub_ent][0] = spd;
                tbk_fuser[sub_ent][1] = tbk_fuser[ent][1];		//Deceleration
                tbk_fuser[sub_ent][2] = angle;

                TeleportEntity(sub_ent, origin);
                SetEntityVelocity(sub_ent, velocity);
                SetTsubakiEntityDamage(sub_ent, damage);

                SetEntityNextThink(sub_ent, get_gametime()+DEFAULT_BULLET_THINKTIME);
                SetRemoveEntityTask(sub_ent, tbk_fuser[ent][2]);
            }

            if(++tbk_iuser[ent][0] < tbk_iuser[ent][1]) {
                SetEntityNextThink(ent, get_gametime()+tbk_iuser[ent][3]/1000.0);
            }
            else{
                RemoveTsubakiEntity(ent);
                return;
            }
        }

        public SlowChangeDirBulletThink(ent)
        {
            new Float:velocity[3], Float:spd, Float:deceleration, Float:angle;

            spd = tbk_fuser[ent][0];
            deceleration = tbk_fuser[ent][1];
            angle = tbk_fuser[ent][2];

            if(tbk_iuser[ent][0] == 0)
            {
                if((spd-=deceleration) < 0.0)
                {
                    tbk_iuser[ent][0] = 1;
                    spd = 0.0;
                    tbk_fuser[ent][2] += M_PI + GetRandomFloat(-M_1OVER2PI, M_1OVER2PI);
                    
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

            tbk_fuser[ent][0] = spd;
            velocity[0] = spd * Cosine(angle);
            velocity[1] = spd *   Sine(angle);

            SetEntityVelocity(ent, velocity);
        }


/* Round Spin Bullet */

    /*
    * 生成以速度{speed1}直線前進，
    * {rotate_delay_time}秒後以半徑{radius}及角速度{agnular_spd}圓周運動
    * 最後以{speed2}直線前進的彈幕【連続発射】
    *
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param owner 主人
    * @param bullet_amount 彈幕生成數
    * @param angle_offset 生成時角度偏差(degree)
    * @param speed1 圓周運動前速難難
    * @param rotate_delay_time n秒後進入圓周運動
    * @param radius 圓周運動時半徑
    * @param angular_spd 圓周運動時角速度(degree)
    * @param total_total_rotate 圓周運動總角度(degree)
    * @param speed2 圓周運動後速度
    * @param last_time 彈幕維持時間
    * @param colors 彈幕顏色
    * @param damage 彈幕傷害
    *
    */
    stock RoundSpinBullet(const bullet_model[], Float:origin[3], owner=0, bullet_amount=8, 
        Float:angle_offset=0.0, Float:speed1=70.0, 
        Float:rotate_delay_time=2.0, 
        Float:radius=80.0, Float:angular_spd=120.0, Float:total_rotate=360.0, 
        Float:speed2=200.0,
        Float:last_time=9.0, colors[4]={0,255,0,16}, Float:damage=DEFAULT_BULLET_DMG)
    {
        CheckTsubakiBulletRegistration();

        angular_spd = ToRadian(angular_spd) * DEFAULT_BULLET_THINKTIME;
        total_rotate = ToRadian(total_rotate);
        angle_offset = ToRadian(angle_offset);

        new ent, i=0;
        new Float:angle, Float:velocity[3];

        for(; i<bullet_amount; i++)
        {
            ent=CreateTsubakiEntity(ROUND_SPIN_BULLET
                                    ,bullet_model
                                    ,.solid_type=DEFAULT_BULLET_SOLID
                                    ,.movetype=MOVETYPE_NOCLIP
                                    ,.owner=owner
                                    ,.r=colors[0]
                                    ,.g=colors[1]
                                    ,.b=colors[2]
                                    ,.a=colors[3]
                                    ,.min_size=BULLET_MIN_SIZE
                                    ,.max_size=BULLET_MAX_SIZE);
            
            if(ent == -1)
                continue;

            angle = angle_offset + i * M_2PI / bullet_amount;
            velocity[0] = speed1 * Cosine(angle);
            velocity[1] = speed1 *   Sine(angle);
        
            tbk_fuser[ent][0] = angle;
            tbk_fuser[ent][1] = angle + total_rotate;
            tbk_fuser[ent][2] = radius;
            tbk_fuser[ent][3] = angular_spd;
            tbk_fuser[ent][4] = speed2;

            tbk_vuser[ent][0][0] = tbk_vuser[ent][0][1] = tbk_vuser[ent][0][2] = 0.0;

            TeleportEntity(ent, origin);
            SetEntityVelocity(ent, velocity);
            SetTsubakiEntityDamage(ent, damage);
            
            SetEntityNextThink(ent, get_gametime()+rotate_delay_time);
            SetRemoveEntityTask(ent, last_time);
        }
    }

        public RoundSpinBulletThink(ent)
        {
            tbk_fuser[ent][0] += tbk_fuser[ent][3];

            if(FloatAbs(tbk_fuser[ent][0]) < FloatAbs(tbk_fuser[ent][1]))
            {
                tbk_vuser[ent][0][0] = tbk_fuser[ent][2] * tbk_fuser[ent][3] * Cosine(tbk_fuser[ent][0]);
                tbk_vuser[ent][0][1] = tbk_fuser[ent][2] * tbk_fuser[ent][3] *   Sine(tbk_fuser[ent][0]);
                SetEntityVelocity(ent, tbk_vuser[ent][0]);
                SetEntityNextThink(ent, get_gametime()+DEFAULT_BULLET_THINKTIME);
            }
            else
            {
                tbk_vuser[ent][0][0] = tbk_fuser[ent][4] * Cosine(tbk_fuser[ent][1]);
                tbk_vuser[ent][0][1] = tbk_fuser[ent][4] *   Sine(tbk_fuser[ent][1]);
                SetEntityVelocity(ent, tbk_vuser[ent][0]);
            }
        }


/*  Change Dest Bullet  */

    /*
    * 在{origin}生成一個直線移動，
    * 在{delay}秒後向{dest_origin}移動的彈幕
    *
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param dest_origin 目的地位置
    * @param owner 主人
    * @param angle 初速的移動方位角(degree)
    * @param start_spd 初速
    * @param end_spd 向目的地移動時的速度
    * @param delay N秒後開始向目的地移動時
    * @param last_time 彈幕維持時間
    * @param colors 彈幕顏色
    * @param end_colors 向目的地移動時彈幕顏色
    * @param damage 彈幕傷害
    *
    */  
    stock ChangeDestBullet(const bullet_model[], Float:origin[3], Float:dest_origin[3], owner=0, Float:angle=0.0, Float:start_spd=200.0, Float:end_spd=250.0, 
                            Float:delay=3.0, Float:last_time=8.0, 
                            colors[4]={204,0,102,16}, end_colors[4]={102,255,0,16}, 
                            Float:damage=DEFAULT_BULLET_DMG)
    {
        CheckTsubakiBulletRegistration();
        
        new ent=CreateTsubakiEntity(CHANGE_DEST_BULLET
                                ,bullet_model
                                ,.solid_type=DEFAULT_BULLET_SOLID
                                ,.movetype=MOVETYPE_NOCLIP
                                ,.owner=owner
                                ,.r=colors[0]
                                ,.g=colors[1]
                                ,.b=colors[2]
                                ,.a=colors[3]
                                ,.min_size=BULLET_MIN_SIZE
                                ,.max_size=BULLET_MAX_SIZE);
        
        if(ent==-1)
            return;
            
        new Float:velocity[3];

        velocity[0] = start_spd * Cosine(angle);
        velocity[1] = start_spd * 	Sine(angle);
        velocity[2] = 0.0;

        tbk_iuser[ent][0] = CreateRGBAInt(end_colors[0], end_colors[1], end_colors[2], end_colors[3]);

        tbk_fuser[ent][0] = end_spd;		//Speed

        CopyVector(tbk_vuser[ent][0], dest_origin);

        TeleportEntity(ent, origin);
        SetEntityVelocity(ent, velocity);
        SetTsubakiEntityDamage(ent, damage);

        SetEntityNextThink(ent, get_gametime()+delay);
        SetRemoveEntityTask(ent, last_time);
    }
    
        public ChangeDestBulletThink(ent)
        {
            GetEntityOrigin(ent, tbk_vuser[ent][1]);	//origin

            SubtractVectors(tbk_vuser[ent][0], tbk_vuser[ent][1], tbk_vuser[ent][0]);
            GetVectorAngles(tbk_vuser[ent][0], tbk_vuser[ent][0]);

            tbk_vuser[ent][0][1] = ToRadian(tbk_vuser[ent][0][1]);		//GetVectorAngles Return Degree

            tbk_vuser[ent][4][0] = tbk_fuser[ent][0] * Cosine(tbk_vuser[ent][0][1]);
            tbk_vuser[ent][4][1] = tbk_fuser[ent][0] *   Sine(tbk_vuser[ent][0][1]);
            tbk_vuser[ent][4][2] = 0.0;

            SetEntityVelocity(ent, tbk_vuser[ent][4]);

            set_rendering(ent, entity_get_int(ent,EV_INT_renderfx), GetRGB_R(tbk_iuser[ent][0]), GetRGB_G(tbk_iuser[ent][0]), GetRGB_B(tbk_iuser[ent][0]), entity_get_int(ent,EV_INT_rendermode), GetRGBA_A(tbk_iuser[ent][0]));
        }

/*  Change Angle Bullet  */

    /*
    * 在{origin}生成一個直線移動，
    * 每{switch_delay}後運動角度增加{angle_change_per_phase}的彈幕
    *
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param owner 主人
    * @param spd 初速
    * @param speed_diff 每次運動變化時上昇速度
    * @param initial_angle 生成時運動角度
    * @param angle_change_per_phase 每次運動變化時上昇角度(degree)
    * @param switch_delay 運動變化時間
    * @param last_time 彈幕維持時間
    * @param colors 彈幕顏色
    * @param colors_increase 每次角度變化時彈幕顏色變化
    * @param damage 彈幕傷害
    *
    */ 
    stock ChangeAngleBullet(const bullet_model[], Float:origin[3], owner=0, Float:spd=200.0, Float:speed_diff=0.0, Float:initial_angle=0.0, 
                                    Float:angle_change_per_phase=90.0, Float:switch_delay=2.0, Float:last_time=8.0,
                                    colors[4]={100,200,55,16}, colors_increase[4]={25,50,0,0},
                                    Float:damage=DEFAULT_BULLET_DMG)
    {
        CheckTsubakiBulletRegistration();

        initial_angle = ToRadian(initial_angle);
        angle_change_per_phase = ToRadian(angle_change_per_phase);
        new ent=CreateTsubakiEntity(CHANGE_ANGLE_BULLET
                                    ,bullet_model
                                    ,.solid_type=DEFAULT_BULLET_SOLID
                                    ,.movetype=MOVETYPE_NOCLIP
                                    ,.owner=owner
                                    ,.r=colors[0]
                                    ,.g=colors[1]
                                    ,.b=colors[2]
                                    ,.a=colors[3]
                                    ,.min_size=BULLET_MIN_SIZE
                                    ,.max_size=BULLET_MAX_SIZE);

        if(ent == -1)
            return;

        tbk_vuser[ent][0][0] = spd * Cosine(initial_angle);
        tbk_vuser[ent][0][1] = spd *   Sine(initial_angle);
        tbk_vuser[ent][0][2] = 0.0;

        tbk_iuser[ent][0] = CreateRGBAInt(colors[0], colors[1], colors[2], colors[3]);
        tbk_iuser[ent][1] = CreateRGBAInt(colors_increase[0], colors_increase[1], colors_increase[2], colors_increase[3]);

        tbk_fuser[ent][0] = spd;
        tbk_fuser[ent][1] = speed_diff;
        tbk_fuser[ent][2] = initial_angle;
        tbk_fuser[ent][3] = angle_change_per_phase;
        tbk_fuser[ent][4] = switch_delay;

        TeleportEntity(ent, origin);
        SetEntityVelocity(ent, tbk_vuser[ent][0]);
        SetTsubakiEntityDamage(ent, damage);

        SetEntityNextThink(ent, get_gametime()+switch_delay);
        SetRemoveEntityTask(ent, last_time);
    }

        public ChangeAngleBulletThink(ent)
        {
            tbk_fuser[ent][0] += tbk_fuser[ent][1];     //spd diff
            tbk_fuser[ent][2] += tbk_fuser[ent][3];     //angle diff

            tbk_vuser[ent][0][0] = tbk_fuser[ent][0] * Cosine(tbk_fuser[ent][2]);
            tbk_vuser[ent][0][1] = tbk_fuser[ent][0] *   Sine(tbk_fuser[ent][2]);

            tbk_iuser[ent][0] = CreateRGBAInt(
                 (GetRGB_R(tbk_iuser[ent][0]) + GetRGB_R(tbk_iuser[ent][1]))%255,
                 (GetRGB_G(tbk_iuser[ent][0]) + GetRGB_G(tbk_iuser[ent][1]))%255,
                 (GetRGB_B(tbk_iuser[ent][0]) + GetRGB_B(tbk_iuser[ent][1]))%255, 
                 (GetRGBA_A(tbk_iuser[ent][0]) + GetRGBA_A(tbk_iuser[ent][1]))%255
            );

            SetEntityVelocity(ent, tbk_vuser[ent][0]);
            SetEntityRenderColor(ent, GetRGB_R(tbk_iuser[ent][0]), GetRGB_G(tbk_iuser[ent][0]), GetRGB_B(tbk_iuser[ent][0]), GetRGBA_A(tbk_iuser[ent][0]));
            SetEntityNextThink(ent, get_gametime()+tbk_fuser[ent][4]);
        }			


/*  Spin Back Bullet  */
    /*
    * 在{origin}生成直線移動，
    * 在{delay}秒後改向生成位置移動的彈幕
    *
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param owner 主人
    * @param bullet_amount 彈幕數
    * @param angle_offset 生成時運動角度偏差
    * @param spawn_radius 生成時與{origin}距離
    * @param spd 初速
    * @param comeback_spd 向生成位置移動時速度
    * @param comeback_radius 目的地半徑偏差
    * @param comeback_angle_shift 目的地半徑角度偏差
    * @param delay 運動變化時間
    * @param last_time 彈幕維持時間
    * @param colors 彈幕顏色
    * @param end_colors 運動變化後顏色
    * @param damage 彈幕傷害
    *
    */ 
    stock SpinBackBullet(const bullet_model[], Float:origin[3], owner=0, bullet_amount=16, Float:angle_offset=0.0, Float:spawn_radius=50.0, 
                                Float:spd=200.0, Float:comeback_spd=250.0, 
                                Float:comeback_radius=100.0, Float:comeback_angle_shift=45.0,
                                Float:delay=3.0, Float:last_time=8.0,
                                colors[4]={204,0,102,16}, 
                                end_colors[4]={0,102,204,16},
                                Float:damage=DEFAULT_BULLET_DMG)
    {
        CheckTsubakiBulletRegistration();

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

            ChangeDestBullet(.bullet_model=bullet_model
                        ,.origin=spawn_origin
                        ,.dest_origin=dest_origin
                        ,.owner=owner
                        ,.angle=angle
                        ,.start_spd=spd
                        ,.end_spd=comeback_spd
                        ,.delay=delay
                        ,.last_time=last_time
                        ,.colors=colors
                        ,.end_colors=end_colors
                        ,.damage=damage);

            angle += M_2PI / bullet_amount;
        }
    }


/*  Shape Drawing Bullet  */


    /*
    * 在{origin}生成直線移動，
    * 以畫{side_amount}邊形的方式移動{draw_side}/{side_amount}次
    * 例如 side_amount=4, draw_side=12 時就畫3次正方形
    *
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param owner 主人
    * @param bullet_amount 彈幕數
    * @param side_amount 運動時圖形的邊數
    * @param side_distance 每一邊長度
    * @param spd 運動速度
    * @param speed_diff 每次轉邊時速度變化
    * @param draw_side 總邊數
    * @param colors 彈幕顏色
    * @param colors_increase 每次角度變化時彈幕顏色變化
    * @param damage 彈幕傷害
    *
    */ 
    stock ShapeDrawingBullet(const bullet_model[], Float:origin[3], owner=0, Float:angle_offset=0.0, bullet_amount=8, side_amount=4,
                                    Float:side_distance=400.0, Float:spd=200.0, Float:speed_diff=0.0, draw_side=8,
                                    colors[4]={100,200,55,16}, colors_increase[4]={25,50,0,0},
                                    Float:damage=DEFAULT_BULLET_DMG)
    {
        CheckTsubakiBulletRegistration();

        new Float:angle_diff=M_FULL_ANGLE/side_amount,      //Angle diff per phase
            Float:switch_delay=side_distance/spd,
            Float:last_time=switch_delay*(draw_side)-0.1;		

        for(new i=0; i<bullet_amount; i++)
        {
            ChangeAngleBullet(.bullet_model=bullet_model
                        ,.origin=origin
                        ,.owner=owner
                        ,.initial_angle=angle_offset
                        ,.angle_change_per_phase=angle_diff
                        ,.spd=spd
                        ,.speed_diff=speed_diff
                        ,.switch_delay=switch_delay
                        ,.last_time=last_time
                        ,.colors=colors
                        ,.colors_increase=colors_increase
                        ,.damage=damage
                        );

            angle_offset += M_FULL_ANGLE/bullet_amount;
        }
    }

/*  Split Straight Bullet  */


    /*
    * 在{origin}生成直線移動的彈幕。
    * 該彈幕在{split_interval}後分裂成{total_split_direction}個方向，每個方向{total_split}粒彈幕
    *
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param owner 主人
    * @param bullet_amount 彈幕數
    * @param spd 初速
    * @param angle_offset 生成時角度偏差
    * @param angle_difference_min 每粒母體生成角度偏差最小值
    * @param angle_difference_max 每粒母體生成角度偏差最大值(均分時請和angle_difference_min分配同樣值)
    * @param split_bullet_max_spd 分裂後彈幕最大速度
    * @param total_split 每個方向分裂數
    * @param total_split_direction 分裂方向數
    * @param split_bullet_last_time 分裂彈幕維持時間
    * @param split_angle_offset 分裂彈幕生成時與運動角度偏差
    * @param split_interval 分裂時間
    * @param colors 彈幕顏色
    * @param split_colors 分裂彈幕顏色
    * @param damage 母體彈幕傷害
    * @param split_damage 分裂後彈幕傷害
    *
    */ 
    //直線推進，分裂，自己消失
    //Please pass angle using Degree
    stock SplitStraightBullet(const bullet_model[], Float:origin[3], owner=0, bullet_amount=3, Float:spd=150.0,
        Float:angle_offset=0.0, Float:angle_difference_min=45.0, Float:angle_difference_max=135.0, 
        Float:split_bullet_max_spd=300.0, total_split=5, total_split_direction=4, Float:split_bullet_last_time=7.0, 
        Float:split_angle_offset=0.0, Float:split_interval=5.0,
        colors[4]={255,51,255,50}, split_colors[4]={255,51,255,16}, Float:damage=20.0, Float:split_damage=DEFAULT_BULLET_DMG)
    {
        CheckTsubakiBulletRegistration();

        new i=0, ent;
        new Float:angle = ToRadian(angle_offset), Float:velocity[3];

        angle_difference_min = ToRadian(angle_difference_min);
        angle_difference_max = ToRadian(angle_difference_max);
        bullet_amount = (bullet_amount<=5)?bullet_amount:5;
        split_angle_offset = ToRadian(split_angle_offset);

        for(; i<bullet_amount; i++, angle += GetRandomFloat(angle_difference_min, angle_difference_max))
        {
            ent=CreateTsubakiEntity(SPLIT_STRAIGHT_BULLET
                                    ,bullet_model
                                    ,.solid_type=DEFAULT_BULLET_SOLID
                                    ,.movetype=MOVETYPE_NOCLIP
                                    ,.owner=owner
                                    ,.r=colors[0]
                                    ,.g=colors[1]
                                    ,.b=colors[2]
                                    ,.a=colors[3]
                                    ,.min_size=BULLET_MIN_SIZE
                                    ,.max_size=BULLET_MAX_SIZE);

            if(ent == -1)
                continue;

            velocity[0] = spd * Cosine(angle);
            velocity[1] = spd *   Sine(angle);
            velocity[2] = 0.0;

            tbk_iuser[ent][0] = total_split;
            tbk_iuser[ent][1] = total_split_direction;
            tbk_iuser[ent][2] = CreateRGBAIntByArray(split_colors);

            tbk_fuser[ent][0] = split_bullet_max_spd;
            tbk_fuser[ent][1] = split_bullet_last_time;
            tbk_fuser[ent][2] = split_angle_offset;
            tbk_fuser[ent][3] = split_damage;
            
            TeleportEntity(ent, origin);
            SetEntityVelocity(ent, velocity);
            SetTsubakiEntityDamage(ent, damage);

            SetEntityNextThink(ent, get_gametime()+split_interval);
        }
    }

        public SplitStraightBulletThink(ent)
        {
            new owner = GetEntityOwner(ent), r=GetRGB_R(tbk_iuser[ent][2]), g=GetRGB_G(tbk_iuser[ent][2]), b=GetRGB_B(tbk_iuser[ent][2]), a=GetRGBA_A(tbk_iuser[ent][2]), i=0, j=0, sub_ent, bullet_model[128];
            new Float:spd = tbk_fuser[ent][0], Float:split_bullet_max_spd=tbk_fuser[ent][0], Float:split_bullet_last_time = tbk_fuser[ent][1], Float:angle, Float:origin[3], Float:velocity[3], Float:damage=GetTsubakiEntityDamage(ent);
            
            GetEntityModel(ent, bullet_model);
            GetEntityOrigin(ent, origin);

            for(; i<tbk_iuser[ent][0]; i++)
            {
                angle = tbk_fuser[ent][2];			//angle = angle_offset

                for(j=0; j<tbk_iuser[ent][1]; j++, angle += M_2PI / tbk_iuser[ent][1])
                {
                    sub_ent=CreateTsubakiEntity(NORMAL_BULLET
                                                ,bullet_model
                                                ,.solid_type=DEFAULT_BULLET_SOLID
                                                ,.movetype=MOVETYPE_NOCLIP
                                                ,.owner=owner
                                                ,.r=r
                                                ,.g=g
                                                ,.b=b
                                                ,.a=a
                                                ,.min_size=BULLET_MIN_SIZE
                                                ,.max_size=BULLET_MAX_SIZE);

                    if(sub_ent == -1)
                        continue;

                    velocity[0] = spd * Cosine(angle);
                    velocity[1] = spd *   Sine(angle);
                    velocity[2] = 0.0;

                    TeleportEntity(sub_ent, origin);
                    SetEntityVelocity(sub_ent, velocity);
                    SetTsubakiEntityDamage(sub_ent, damage);

                    SetRemoveEntityTask(sub_ent, split_bullet_last_time);
                }
                
                spd -= split_bullet_max_spd / tbk_iuser[ent][0];
            }

            RemoveTsubakiEntity(ent);
            return;
        }

 
    /*
    * 在{origin}生成直線移動的彈幕。
    * 該彈幕在每{phase_interval}秒後隱形，然後每{phase_interval}秒重現，LOOP
    *
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param owner 主人
    * @param bullet_amount 彈幕數
    * @param spd 初速
    * @param angle_offset 生成時角度偏差
    * @param angle_difference_min 每粒母體生成角度偏差最小值
    * @param angle_difference_max 每粒母體生成角度偏差最大值(均分時請和angle_difference_min分配同樣值)
    * @param last_time 彈幕維持時間
    * @param phase_interval 每N秒進行一次變化
    * @param colors 彈幕顏色
    * @param damage 彈幕傷害
    *
    */ 
    stock AppearAndDisapperBullet(const bullet_model[], Float:origin[3], owner=0, bullet_amount=3, Float:spd=150.0, 
        Float:angle_offset=0.0, Float:angle_difference_min=120.0, Float:angle_difference_max=120.0,
        Float:last_time=8.0, Float:phase_interval=1.0, 
        colors[4]={150,205,25,16}, Float:damage=DEFAULT_BULLET_DMG)
    {
        CheckTsubakiBulletRegistration();

        new i=0, ent;
        new Float:velocity[3], Float:angle=angle_offset;

        angle_difference_min = ToRadian(angle_difference_min);
        angle_difference_max = ToRadian(angle_difference_max);
        
        for(; i<bullet_amount; i++, angle += GetRandomFloat(angle_difference_min, angle_difference_max))
        {
            ent=CreateTsubakiEntity(APPEAR_AND_DISAPPER_BULLET
                                    ,bullet_model
                                    ,.solid_type=DEFAULT_BULLET_SOLID
                                    ,.movetype=MOVETYPE_NOCLIP
                                    ,.owner=owner
                                    ,.r=colors[0]
                                    ,.g=colors[1]
                                    ,.b=colors[2]
                                    ,.a=colors[3]
                                    ,.min_size=BULLET_MIN_SIZE
                                    ,.max_size=BULLET_MAX_SIZE);

            if(ent == -1)
                continue;
            
            tbk_iuser[ent][0] = 0;

            tbk_fuser[ent][0] = phase_interval;

            velocity[0] = spd * Cosine(angle);
            velocity[1] = spd *   Sine(angle);
            velocity[2] = 0.0;

            TeleportEntity(ent, origin);
            SetEntityVelocity(ent, velocity);
            SetTsubakiEntityDamage(ent, damage);

            SetEntityNextThink(ent, get_gametime()+phase_interval);
            SetRemoveEntityTask(ent, last_time);
        }		

    }
        
        public AppearAndDisapperBulletThink(ent)
        {
            switch(tbk_iuser[ent][0]++ % 2)
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

            SetEntityNextThink(ent, get_gametime()+tbk_fuser[ent][0]);
        }

/*  Pointing Bullet  */
    
    /*
    * 從玩家視角發射彈幕
    *
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param owner 主人
    * @param bullet_amount 彈幕數
    * @param speed 彈幕速度
    * @param last_distance 彈幕維持距離
    * @param colors 彈幕顏色
    * @param damage 彈幕傷害
    *
    */ 
    stock FirePointingBullet(const bullet_model[], Float:origin[3], owner, bullet_amount=1, Float:speed=200.0, Float:last_distance=1000.0,
        colors[4]={200,200,0,16}, Float:damage=DEFAULT_BULLET_DMG)
    {
        CheckTsubakiBulletRegistration();
        
        new ent, i=0;
        new Float:angles[3], Float:velocity[3], Float:pitch, Float:yaw;
        
        last_distance /= speed;
        GetClientEyeAngles(owner, angles);
        pitch = ToRadian(-1.0*angles[0]);
        yaw = ToRadian(angles[1]);

        for(; i<bullet_amount; i++, yaw += M_2PI/bullet_amount)
        {
            ent = CreateTsubakiEntity(NORMAL_BULLET
                                    , bullet_model
                                    , .solid_type=DEFAULT_BULLET_SOLID
                                    , .movetype=MOVETYPE_NOCLIP
                                    , .owner=owner
                                    , .r=colors[0]
                                    , .g=colors[1]
                                    , .b=colors[2]
                                    , .a=colors[3],
                                    ,.min_size=BULLET_MIN_SIZE
                                    ,.max_size=BULLET_MAX_SIZE);

            if(ent == -1)
                continue;

            velocity[0] = speed * Cosine(pitch) * Cosine(yaw);
            velocity[1] = speed * Cosine(pitch) *   Sine(yaw);
            velocity[2] = speed *   Sine(pitch);

            TeleportEntity(ent, origin);
            SetEntityVelocity(ent, velocity);
            SetTsubakiEntityDamage(ent, damage);

            SetRemoveEntityTask(ent, last_distance);    //last distance has been divided before
        }
    }

/* Split Backward Bullet */

    //直線推進，背後分裂
    /*
    * 從玩家視角發射彈幕
    *
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param owner 主人
    * @param bullet_amount 彈幕數
    * @param bullet_spd 母體速度
    * @param angle_offset 母體生成時角度偏差
    * @param angle_diff_min 每個母體生成角度差距最小值
    * @param angle_diff_max 每個母體生成角度差距最大值
    * @param split_interval 每次分裂間隔時間
    * @param split_bullet_spd 分裂彈幕速度
    * @param total_split_direction 分裂數
    * @param split_base_angle 最初分裂的角度與母體運動角度差距
    * @param angle_per_each_split 每個分裂彈幕生成角度差距最大值
    * @param bullet_last_time 母體存在時間
    * @param split_bullet_last_time 分裂彈幕存在時間
    * @param last_distance 彈幕維持距離
    * @param colors 彈幕顏色
    * @param split_colors 分裂彈幕顏色
    * @param damage 彈幕傷害
    *
    */ 
    stock SplitBackwardBullet(const bullet_model[], Float:origin[3], owner=0, bullet_amount=3,
                                Float:bullet_spd=150.0,
                                Float:angle_offset=0.0, Float:angle_diff_min=15.0, Float:angle_diff_max=70.0, 
                                Float:split_interval=2.0,  
                                Float:split_bullet_spd=250.0, total_split_direction=4, 
                                Float:split_base_angle=90.0, Float:angle_per_each_split = 60.0,
                                Float:bullet_last_time=7.0, Float:split_bullet_last_time=5.0,
                                colors[4]={0,204,0,16}, split_colors[4]={255,51,255,16}, Float:damage=DEFAULT_BULLET_DMG)
    {
        CheckTsubakiBulletRegistration();

        new i=0, ent;
        new Float:velocity[3];

        angle_offset = ToRadian(angle_offset);
        split_base_angle = ToRadian(split_base_angle);
        angle_per_each_split = ToRadian(angle_per_each_split);
        angle_diff_min = ToRadian(angle_diff_min);
        angle_diff_max = ToRadian(angle_diff_max);

        bullet_amount = Clamp(bullet_amount, 1, 8);
        total_split_direction = Clamp(total_split_direction, 1, 5);

        for(; i<bullet_amount; i++, angle_offset+=GetRandomFloat(angle_diff_min, angle_diff_max))
        {
            ent = CreateTsubakiEntity(SPLIT_BACKWARD_BULLET
                                        ,bullet_model
                                        ,.solid_type=DEFAULT_BULLET_SOLID
                                        ,.movetype=MOVETYPE_NOCLIP
                                        ,.owner=owner
                                        ,.r=colors[0]
                                        ,.g=colors[1]
                                        ,.b=colors[2]
                                        ,.a=colors[3]
                                        ,.min_size=BULLET_MIN_SIZE
                                        ,.max_size=BULLET_MAX_SIZE);
            
            if(ent == -1)
                continue;

            velocity[0] = bullet_spd * Cosine(angle_offset);
            velocity[1] = bullet_spd *   Sine(angle_offset);

            tbk_iuser[ent][0] = total_split_direction;
            tbk_iuser[ent][1] = CreateRGBAIntByArray(split_colors);

            tbk_fuser[ent][0] = split_bullet_spd;
            tbk_fuser[ent][1] = split_bullet_last_time;
            tbk_fuser[ent][2] = angle_offset + split_base_angle;
            tbk_fuser[ent][3] = angle_per_each_split;
            tbk_fuser[ent][4] = split_interval;

            TeleportEntity(ent, origin);
            SetEntityVelocity(ent, velocity);
            SetTsubakiEntityDamage(ent, damage);

            SetEntityNextThink(ent, get_gametime()+split_interval);
            SetRemoveEntityTask(ent, bullet_last_time);
        }
    }

        public SplitBackwardBulletThink(ent)
        {
            new i=0, sub_ent, r=GetRGB_R(tbk_iuser[ent][1]), g=GetRGB_G(tbk_iuser[ent][1]), b=GetRGB_B(tbk_iuser[ent][1]), a=GetRGBA_A(tbk_iuser[ent][1]), total_split_direction=tbk_iuser[ent][0], owner=GetEntityOwner(ent);
            new Float:origin[3], Float:velocity[3], Float:angle=tbk_fuser[ent][2], Float:speed = tbk_fuser[ent][0], Float:last_time = tbk_fuser[ent][1], Float:damage=GetTsubakiEntityDamage(ent);
            new bullet_model[128];
            GetEntityModel(ent, bullet_model);

            GetEntityOrigin(ent, origin);

            for(; i<total_split_direction; i++)
            {
                sub_ent = CreateTsubakiEntity(NORMAL_BULLET
                                                ,bullet_model
                                                ,.solid_type=DEFAULT_BULLET_SOLID
                                                ,.movetype=MOVETYPE_NOCLIP
                                                ,.owner=owner
                                                ,.r=r
                                                ,.g=g
                                                ,.b=b
                                                ,.a=a
                                                ,.min_size=BULLET_MIN_SIZE
                                                ,.max_size=BULLET_MAX_SIZE);
                
                if( sub_ent==-1 )
                    continue;

                velocity[0] = speed * Cosine(angle);
                velocity[1] = speed *   Sine(angle);

                TeleportEntity(sub_ent, origin);
                SetEntityVelocity(sub_ent, velocity);
                SetTsubakiEntityDamage(sub_ent, damage);

                SetRemoveEntityTask(sub_ent, last_time);

                angle += tbk_fuser[ent][3];
            }

            SetEntityNextThink(ent, get_gametime()+tbk_fuser[ent][4]);
        }

/*  Explosive Bullet  */

    /*
    * 從{origin}發射觸碰後會爆炸的彈幕
    *
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param owner 主人
    * @param bullet_amount 彈幕數
    * @param bullet_spd 速度
    * @param angle_offset 生成時角度偏差
    * @param last_time 彈幕存在時間
    * @param damage_radius 傷害半徑，傷害計算式為 damage * (radius-distance) / radius
    * @param colors 彈幕顏色
    * @param damage 彈幕傷害
    *
    */ 
    stock ExplosiveBullet(const bullet_model[], explosion_sprite_id, Float:origin[3], owner=0, bullet_amount=3, Float:bullet_spd=150.0,
                            Float:angle_offset=0.0, Float:last_time=7.0, 
                            Float:damage_radius=300.0, colors[4]={200,0,0,16}, Float:damage=35.0)
    {
        CheckTsubakiBulletRegistration();

        new i=0, ent;
        new Float:velocity[3];

        angle_offset = ToRadian(angle_offset);

        for(; i<bullet_amount; i++)
        {
            ent=CreateTsubakiEntity(EXPLOSIVE_BULLET
                                    ,bullet_model
                                    ,.solid_type=DEFAULT_BULLET_SOLID
                                    ,.movetype=MOVETYPE_NOCLIP
                                    ,.owner=owner
                                    ,.r=colors[0]
                                    ,.g=colors[1]
                                    ,.b=colors[2]
                                    ,.a=colors[3]
                                    ,.min_size=BULLET_MIN_SIZE
                                    ,.max_size=BULLET_MAX_SIZE);
            
            if(ent==-1)
                continue;
                    
            tbk_iuser[ent][0] = 0;
            tbk_iuser[ent][1] = CreateRGBAIntByArray(colors);
            tbk_iuser[ent][2] = explosion_sprite_id;

            tbk_fuser[ent][0] = damage_radius;
            tbk_fuser[ent][1] = damage;

            velocity[0] = bullet_spd * Cosine(angle_offset);
            velocity[1] = bullet_spd *   Sine(angle_offset);
            
            TeleportEntity(ent, origin);
            SetEntityVelocity(ent, velocity);
            SetTsubakiEntityDamage(ent, damage);

            SetEntityNextThink(ent, get_gametime()+EXPLOSIVE_BULLET_THINKTIME);
            SetRemoveEntityTask(ent, last_time);
        
            angle_offset += M_2PI / bullet_amount;
        }
    }

        public ExplosiveBulletThink(ent)
        {
            switch((++tbk_iuser[ent][0])%2)
            {
                case 1:set_ent_rendering(ent, kRenderFxNone, 0, 0, 0, kRenderNormal, 0);
                case 0:set_ent_rendering(ent, kRenderFxGlowShell, GetRGB_R(tbk_iuser[ent][1]), GetRGB_G(tbk_iuser[ent][1]), GetRGB_B(tbk_iuser[ent][1]), kRenderTransAlpha, GetRGBA_A(tbk_iuser[ent][1]));
            }
            
            SetEntityNextThink(ent, get_gametime()+EXPLOSIVE_BULLET_THINKTIME);
        }
        
        public OnExplosiveBulletTouch(ent, target)
        {
            new owner=GetEntityOwner(ent);
            if(target!=owner && IsClientValid(target))
            {
                new i=0;
                new Float:origin[3], Float:distance, Float:max_radius=tbk_fuser[ent][0], Float:damage=GetTsubakiEntityDamage(ent);
                GetEntityOrigin(ent, origin);
                for(; i<=MaxClients; i++)
                {
                    if( IsClientValid(i) && IsPlayerAlive(i) && (owner==0 || (i!=owner && cs_get_user_team(owner)!=cs_get_user_team(i))) )
                    {
                        distance = 1.0 * get_entity_distance(ent, i);
                        if(distance < max_radius) {
                            client_print(i, print_chat, "distance : %.4f  damage : %.4f", distance, damage*(max_radius-distance)/max_radius);
                            ExecuteHamB(Ham_TakeDamage, i, 0, owner, damage*(max_radius-distance)/max_radius, DMG_BLAST|DMG_ALWAYSGIB);
                        }
                        
                    }
                }
                TBKCreateExplosionTE(origin, tbk_iuser[ent][2]);

                RemoveTsubakiEntity(ent);
                return;
            }
        }


/*  Swirl Bullet  */

    /*
    * 從{origin}生成轉一圈回到{origin}的彈幕
    *
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param owner 主人
    * @param bullet_amount 彈幕數
    * @param angle_offset 生成時角度偏差
    * @param bullet_spd 速度
    * @param angular_spd 角速度(degree)
    * @param last_time 彈幕存在時間
    * @param damage 傷害半徑，傷害計算式為 damage * (radius-distance) / radius
    * @param colors 彈幕顏色
    * @param damage 彈幕傷害
    *
    */ 
    stock SwirlBullet(const bullet_model[], Float:origin[3], owner, bullet_amount=6, 
                        Float:angle_offset=0.0, Float:spd=200.0, Float:angular_spd=60.0, 
                        Float:last_time=12.0, colors[4]={255,20,147,16}, Float:damage=6.0)
    {
        CheckTsubakiBulletRegistration();

        new ent, i=0;
        new Float:velocity[3];
        angle_offset = ToRadian(angle_offset);

        for(; i<bullet_amount; i++)
        {
            ent=CreateTsubakiEntity(SWIRL_BULLET
                                    ,bullet_model
                                    ,.solid_type=DEFAULT_BULLET_SOLID
                                    ,.movetype=MOVETYPE_NOCLIP
                                    ,.owner=owner
                                    ,.r=colors[0]
                                    ,.g=colors[1]
                                    ,.b=colors[2]
                                    ,.a=colors[3]
                                    ,.min_size=BULLET_MIN_SIZE
                                    ,.max_size=BULLET_MAX_SIZE);
            
            if(ent == -1)
                continue;
                
            velocity[0] = spd * Cosine(angle_offset);
            velocity[1] = spd *   Sine(angle_offset);

            tbk_fuser[ent][0] = spd;
            tbk_fuser[ent][1] = angle_offset;
            tbk_fuser[ent][2] = DEFAULT_BULLET_THINKTIME * ToRadian(angular_spd);

            //For Think velocity
            tbk_vuser[ent][0][2] = 0.0;

            TeleportEntity(ent, origin);
            SetEntityVelocity(ent , velocity);
            SetTsubakiEntityDamage(ent, damage);

            SetEntityNextThink(ent, get_gametime()+DEFAULT_BULLET_THINKTIME);
            SetRemoveEntityTask(ent, last_time);

            angle_offset += M_2PI/bullet_amount;
        }
    }

        public SwirlBulletThink(ent)
        {
            tbk_vuser[ent][0][0] = tbk_fuser[ent][0] * Cosine(tbk_fuser[ent][1]);
            tbk_vuser[ent][0][1] = tbk_fuser[ent][0] *   Sine(tbk_fuser[ent][1]);

            tbk_fuser[ent][1] += tbk_fuser[ent][2];
            SetEntityVelocity(ent, tbk_vuser[ent][0]);
            SetEntityNextThink(ent, get_gametime()+DEFAULT_BULLET_THINKTIME);
        }

/* Tracer Bullet */

    /*
    * 從{origin}生成隨機移動<->休息
    * 最後向{target}移動的彈幕
    *
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param target 目標
    * @param owner 主人
    * @param bullet_amount 彈幕數
    * @param stage 移動<->休息的次數
    * @param min_radius 隨機移動時，目的地距離生成位置的最小半徑
    * @param max_radius 隨機移動時，目的地距離生成位置的最大半徑
    * @param switch=delay 速度
    * @param last_time 彈幕存在時間
    * @param damage 彈幕傷害
    * @param colors 彈幕顏色
    * @param colors_final 向目標移動時彈幕顏色
    * @param damage 彈幕傷害
    *
    */ 
    stock TracerBullet(const bullet_model[], Float:origin[3], target, owner=0, bullet_amount=6, stages=2,
                        Float:min_radius=250.0, Float:max_radius=400.0, 
                        Float:switch_delay=2.0, Float:last_time=3.0,
                        colors[4]={0,102,0,16}, colors_final[4]={255,185,15,16}, Float:damage=7.0)
    {
        CheckTsubakiBulletRegistration();

        new ent, i, int_colors=CreateRGBAIntByArray(colors_final);
        new Float:spd, Float:velocity[3]={0.0, ...}, Float:angle;

        for(; i<bullet_amount; i++)
        {
            ent=CreateTsubakiEntity(TRACER_BULLET
                                    ,bullet_model
                                    ,.solid_type=DEFAULT_BULLET_SOLID
                                    ,.movetype=MOVETYPE_NOCLIP
                                    ,.owner=owner
                                    ,.r=colors[0]
                                    ,.g=colors[1]
                                    ,.b=colors[2]
                                    ,.a=colors[3]
                                    ,.min_size=BULLET_MIN_SIZE
                                    ,.max_size=BULLET_MAX_SIZE);
            
            if(ent == -1)
                continue;

            angle = GetRandomFloat(0.0, M_2PI);
            spd = GetRandomFloat(max_radius, max_radius) / switch_delay;

            velocity[0] = spd * Cosine(angle);
            velocity[1] = spd *   Sine(angle);

            tbk_iuser[ent][0] = 0;
            tbk_iuser[ent][1] = 2*stages;
            tbk_iuser[ent][2] = int_colors;
            tbk_iuser[ent][3] = target;

            tbk_fuser[ent][0] = angle + GetRandomFloat(M_PI/2.0, M_PI*3.0/2.0);
            tbk_fuser[ent][1] = min_radius;
            tbk_fuser[ent][2] = max_radius;
            tbk_fuser[ent][3] = switch_delay;
            tbk_fuser[ent][4] = last_time;


            //Used for velocity
            tbk_vuser[ent][1][0] = 0.0;
            tbk_vuser[ent][1][1] = 0.0;
            tbk_vuser[ent][1][2] = 0.0;

            CopyVector(tbk_vuser[ent][0], origin);
            
            TeleportEntity(ent, origin);
            SetEntityVelocity(ent, velocity);
            SetTsubakiEntityDamage(ent, damage);

            SetEntityNextThink(ent, get_gametime()+switch_delay);
            SetRemoveEntityTask(ent, (1+stages*2)*switch_delay+last_time);    
        }
    }

        public TracerBulletThink(ent)
        {
            if(tbk_iuser[ent][0] >= tbk_iuser[ent][1]-1)
            {
                //Trace Player
                if(pev_valid(tbk_iuser[ent][3]))
                {
                    GetClientAbsOrigin(tbk_iuser[ent][3], tbk_vuser[ent][0]);
                }

                SetEntityRenderColor(ent, GetRGB_R(tbk_iuser[ent][2]), GetRGB_G(tbk_iuser[ent][2]), GetRGB_B(tbk_iuser[ent][2]), GetRGB_B(tbk_iuser[ent][3]));
                GetEntityOrigin(ent, tbk_vuser[ent][2]);
                tbk_fuser[ent][4] = GetVectorDistance(tbk_vuser[ent][0], tbk_vuser[ent][2])*2.0/tbk_fuser[ent][4];			//Speed
                SubtractVectors(tbk_vuser[ent][0], tbk_vuser[ent][2], tbk_vuser[ent][0]);
                GetVectorAngles(tbk_vuser[ent][0], tbk_vuser[ent][2]);
                tbk_vuser[ent][2][1] = ToRadian(tbk_vuser[ent][2][1]);

                tbk_vuser[ent][0][0] = tbk_fuser[ent][4] * Cosine(tbk_vuser[ent][2][1]);
                tbk_vuser[ent][0][1] = tbk_fuser[ent][4] *   Sine(tbk_vuser[ent][2][1]);

                SetEntityVelocity(ent, tbk_vuser[ent][0]);
            }
            else
            {
                if(++tbk_iuser[ent][0]%2)
                {
                    tbk_vuser[ent][1][0] = 0.0;
                    tbk_vuser[ent][1][1] = 0.0;
                    tbk_vuser[ent][1][2] = 0.0;
                    
                }
                else
                {
                    new Float:spd = GetRandomFloat(tbk_fuser[ent][1], tbk_fuser[ent][2]) / tbk_fuser[ent][3];		//Speed
                    tbk_vuser[ent][1][0] = spd * Cosine(tbk_fuser[ent][0]);
                    tbk_vuser[ent][1][1] = spd *   Sine(tbk_fuser[ent][0]);
                    tbk_fuser[ent][0] = tbk_fuser[ent][0] + GetRandomFloat(M_PI/2.0, M_PI*3.0/2.0);
                }

                SetEntityNextThink(ent, get_gametime()+tbk_fuser[ent][3]);
                SetEntityVelocity(ent, tbk_vuser[ent][1]);
            }
            
        }


/* Snake Protect */

    /*
    * Snake Protect
    *
    * @param bullet_model 彈幕模組
    * @param origin 生成位置
    * @param target 目標
    * @param owner 主人
    * @param bullet_amount 彈幕數
    * @param stage 移動<->休息的次數
    * @param min_radius 隨機移動時，目的地距離生成位置的最小半徑
    * @param max_radius 隨機移動時，目的地距離生成位置的最大半徑
    * @param switch=delay 速度
    * @param last_time 彈幕存在時間
    * @param damage 彈幕傷害
    * @param colors 彈幕顏色
    * @param colors_final 向目標移動時彈幕顏色
    * @param damage 彈幕傷害
    *
    */
    
    //Snake Protect
    #define MAX_SNAKE_PROTECT_AMOUNT	8
    stock SnakeProtect(const bullet_model[], laser_sprite_id, Float:origin[3], owner, bullet_amount=8, Float:acceleration=250.0, 
                        Float:last_time=15.0, Float:delay_per_bullet=0.1, 
                        colors[4]={255,255,0,16}, Float:damage=DEFAULT_BULLET_DMG)
    {
        CheckTsubakiBulletRegistration();
        
        new i=1, launcher, previous_ent, sub_ent;

        launcher=CreateInvisibleLauncher(SNAKE_PROTECT_CONTROLLER, bullet_model, .owner=owner);

        if(launcher == -1)
            return;
            
        acceleration *= delay_per_bullet;

        // Head Bullet
        previous_ent = CreateTsubakiEntity(SNAKE_PROTECT_BULLET
                                            ,bullet_model
                                            ,.solid_type=DEFAULT_BULLET_SOLID
                                            ,.movetype=MOVETYPE_NOCLIP
                                            ,.owner=owner
                                            ,.r=colors[0]
                                            ,.g=colors[1]
                                            ,.b=colors[2]
                                            ,.a=colors[3]
                                            ,.min_size=BULLET_MIN_SIZE
                                            ,.max_size=BULLET_MAX_SIZE);
        if(previous_ent == -1)
        {
            RemoveTsubakiEntity(launcher);
            return;
        }
        
    
        te_create_beam_between_entities(owner, previous_ent, laser_sprite_id, .life=RoundFloat(last_time*10-1), .framerate=0, .width=5, .r=255, .b=255, .a=200, .reliable=false);

        tbk_iuser[previous_ent][0] = 0;         //Head
        TeleportEntity(previous_ent, origin);
        ResetVector(tbk_vuser[previous_ent][0]);
        SetTsubakiEntityDamage(previous_ent, damage);
        
        tbk_iuser[launcher][4] = RoundFloat(last_time/delay_per_bullet);
        
        tbk_fuser[launcher][1] = acceleration;
        tbk_fuser[launcher][2] = delay_per_bullet;
        
        for(; i<bullet_amount; i++) {
            sub_ent = CreateTsubakiEntity(SNAKE_PROTECT_BULLET
                                            ,bullet_model
                                            ,.solid_type=DEFAULT_BULLET_SOLID
                                            ,.movetype=MOVETYPE_NOCLIP
                                            ,.owner=owner
                                            ,.r=colors[0]
                                            ,.g=colors[1]
                                            ,.b=colors[2]
                                            ,.a=colors[3]
                                            ,.min_size=BULLET_MIN_SIZE
                                            ,.max_size=BULLET_MAX_SIZE);

            if(sub_ent == -1)                                        
                continue;

            tbk_iuser[sub_ent][0] = previous_ent;       //Tail -> Head
            tbk_iuser[previous_ent][1] = sub_ent;       //Head -> Tail
            
            ResetVector(tbk_vuser[sub_ent][0]);
            SetTsubakiEntityDamage(sub_ent, damage);

            TeleportEntity(sub_ent, origin);

            previous_ent = sub_ent;
        }

        tbk_iuser[launcher][1] = sub_ent;        //Tail
        SetEntityNextThink(launcher, get_gametime()+delay_per_bullet);
    }

        public SnakeProtectControllerThink(launcher) {
            new sub_ent, owner=GetEntityOwner(launcher);
            new Float:velocity[3], Float:target_origin[3], Float:origin[3];

            if(tbk_iuser[launcher][4]--<=0 || !IsClientValid(owner) || !IsPlayerAlive(owner))
            {
                for(sub_ent=tbk_iuser[launcher][1]; sub_ent!=0; sub_ent=tbk_iuser[sub_ent][0]) {
                    RemoveTsubakiEntity(sub_ent);
                }
                
                RemoveTsubakiEntity(launcher);
                return;
            }

            for(sub_ent=tbk_iuser[launcher][1]; tbk_iuser[sub_ent][0]!=0; sub_ent=tbk_iuser[sub_ent][0]) {
                GetEntityVelocity(tbk_iuser[sub_ent][0], velocity);

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

            velocity[0] = velocity[0] + tbk_fuser[launcher][1] * Cosine(target_origin[0]) * Cosine(target_origin[1]);
            velocity[1] = velocity[1] + tbk_fuser[launcher][1] * Cosine(target_origin[0]) *   Sine(target_origin[1]);
            velocity[2] = velocity[2] + tbk_fuser[launcher][1] *   Sine(target_origin[0]);
            
            SetEntityVelocity(sub_ent, velocity);
            SetEntityNextThink(launcher, get_gametime()+tbk_fuser[launcher][2]);
        }

        public SnakeProtectBulletTouch(ent, client) {
            new owner=GetEntityOwner(ent);

            if((1<=client<=MaxClients) && (owner!=client) && (owner==0 || cs_get_user_team(client)!=cs_get_user_team(owner))) {
                ExecuteHamB(Ham_TakeDamage, client, 0, owner, GetTsubakiEntityDamage(ent), BULLET_DMG_TYPE);

                entity_set_int(ent, EV_INT_solid, SOLID_NOT);
                set_pev(ent, pev_effects, pev(ent, pev_effects) | EF_NODRAW);
            }

        }