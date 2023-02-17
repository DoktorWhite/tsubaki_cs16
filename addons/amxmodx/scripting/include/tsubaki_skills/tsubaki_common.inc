#if defined _tsubaki_common
    #endinput
#endif
#define _tsubaki_common

#include <amxmodx>
#include <amxmisc>
#include <cstrike>
#include <engine>
#include <fakemeta>
#include <fakemeta_util>
#include <fun>
#include <hamsandwich>
#include <msgstocks>

    
    /* Maths  */
    //#define M_PI 3.1415926535     //amxconst.inc
    #if !defined M_1OVER2PI
        #define M_1OVER2PI 1.570796326
    #endif
    #if !defined M_2PI
        #define M_2PI 6.283185307
    #endif
    #if !defined M_STRAIGHT_ANGLE
        #define M_STRAIGHT_ANGLE 180.0
    #endif
    #if !defined M_FULL_ANGLE
        #define M_FULL_ANGLE 360.0
    #endif
    #if !defined ToRadian
        #define ToRadian(%0) (%0 * M_PI / 180.0)
    #endif
    #if !defined ToDegree
        #define ToDegree(%0) (%0 * 180.0 / M_PI)
    #endif
    #if !defined GetRandomFloat
        #define GetRandomFloat random_float
    #endif

    #define GetRGBA_A(%0) ((%0>>24) & 0xFF)
    #define GetRGB_R(%0) ((%0&0xFF0000)>>16)
    #define GetRGB_G(%0) ((%0&0xFF00)>>8)
    #define GetRGB_B(%0) (%0&0xFF)
    stock CreateRGBInt(R, G, B)	{return (R<<16) + (G<<8) + B;}
    stock CreateRGBAInt(R, G, B, A) {return (A<<24) + (R<<16) + (G<<8) + B;}
    stock CreateRGBAIntByArray(colors[4]) {return (colors[3]<<24) + (colors[0]<<16) + (colors[1]<<8) + colors[2];}

    #define CopyVector(%0,%1) %0[0]=%1[0]; %0[1]=%1[1]; %0[2]=%1[2]
    //stock CopyVector(Float:dest[3], Float:source[3]) { dest[0] = source[0]; dest[1] = source[1]; dest[2] = source[2]; }
    #define ResetVector(%0) %0[0]=%0[1]=%0[2]=0.0
    #define GetVectorLength(%0) vector_length(%0)


    //#define MAX_ENTITY 512

    stock tbk_iuser[512][5];
    stock Float:tbk_fuser[512][5];
    stock Float:tbk_vuser[512][5][3];
    stock Float:tbk_damage[512]

    #define TSUBAKI_ENTITY_FLAG     0x02000000
        #define TSUBAKI_ENTITY_BULLET     (TSUBAKI_ENTITY_FLAG|1)
    /*
    *  檢查實體是否TSUBAKI 的實體
    *   
    *   @param %0 entity_id
    *   
    *   @return 非0時即為Tsubaki 的實體
    */
    #define IsTsubakiEntity(%0)     (entity_get_edict(%0, EV_ENT_euser3)&TSUBAKI_ENTITY_FLAG)

    //設定彈幕實體的傷害
    #define SetTsubakiEntityDamage(%0,%1) tbk_damage[%0]=%1
    //獲取彈幕實體的傷害
    #define GetTsubakiEntityDamage(%0) tbk_damage[%0]


    #define IsClientValid(%0)   (1<=%0<=MaxClients && is_user_connected(%0))
    #define IsPlayerAlive(%0)   (is_user_alive(%0))

    #define TSUBAKI_TASKID_ENTITY_REMOVE_BASE       830000000
    #define TSUBAKI_TASKID_ENTITY_REMOVE(%0)        (830000000+%0)
    #define TSUBAKI_TASKID_ENTITY_TASK_BASE         831000000
    #define TSUBAKI_TASKID_ENTITY_TASK(%0)          (831000000+%0)

    #define SetEntityNextThink(%0,%1) entity_set_float(%0, EV_FL_nextthink, %1)
    #define SetRemoveEntityTask(%0,%1) set_task_ex(%1, "RemoveTsubakiEntity", TSUBAKI_TASKID_ENTITY_REMOVE(%0), _, _, SetTask_Once)

    #define TeleportEntity(%0,%1) entity_set_vector(%0, EV_VEC_origin, %1)
    
    #define GetEntityOrigin(%0,%1) entity_get_vector(%0,EV_VEC_origin,%1)
    #define GetClientAbsOrigin(%0,%1) entity_get_vector(%0,EV_VEC_origin,%1)
    #define GetPlayerAimOrigin(%0,%1) fm_get_aim_origin(%0,%1)
    #define GetPlayerViewAngle(%0,%1) entity_get_vector(%0, EV_VEC_v_angle, %1)
    #define GetClientEyeAngles(%0,%1) GetPlayerViewAngle(%0,%1)
    #define GetEntityVelocity(%0,%1) get_user_velocity(%0,%1)
    #define SetEntityVelocity(%0,%1) entity_set_vector(%0, EV_VEC_velocity, %1)
    #define SetEntityGravity(%0,%1) set_pev(%0,pev_gravity,%1)
    #define SetEntityOwner(%0,%1)		entity_set_edict(%0, EV_ENT_owner, %1)
    #define GetEntityOwner(%0)			entity_get_edict(%0, EV_ENT_owner)

    #define GetEntityModel(%0,%1) entity_get_string(%0, EV_SZ_model, %1, sizeof(%1))
    #define SetEntityRenderColor(%0,%1,%2,%3,%4) set_rendering(%0, entity_get_int(%0,EV_INT_renderfx), %1, %2, %3, entity_get_int(%0,EV_INT_rendermode), %4)

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
    /*
    *
    * 實體生成
    *
    * @param clsname        實體classname
    * @param model_route    模組
    * @param solid_type     SOLID_*
    * @param movetype       MOVETYPE_*
    * @param gravity        重力[1.0 為基本]
    * @param owner          主人
    * @param r              Render 紅(0~255)
    * @param g              Render 綠(0~255)
    * @param b              Render 藍(0~255)
    * @param a              Render 強度(0~255)
    * @param render         kRender*
    * @param render_fx      kRenderFx*
    * @param min_size       hit_box negative size
    * @param min_size       hit_box positive size   
    *
    * @return -1：生成失敗
    * @return 其他：生成的實體Id
    */
    stock CreateTsubakiEntity(const clsname[], const model_route[], solid_type=SOLID_TRIGGER, movetype=MOVETYPE_NOCLIP, Float:gravity=0.0, owner=0, r=255, g=255, b=255, a=16, render=kRenderNormal, render_fx=kRenderFxGlowShell, Float:min_size[3]={-1.0,-1.0,-1.0}, Float:max_size[3]={1.0,1.0,1.0}) {
        static ent;
        static info_target[] = "info_target"
        
        ent = create_entity(info_target);
        if(ent) {
            entity_set_string(ent, EV_SZ_classname, clsname);
            engfunc(EngFunc_SetModel, ent, model_route);
            entity_set_int(ent, EV_INT_solid, solid_type);
            entity_set_int(ent, EV_INT_movetype, movetype);
            entity_set_size(ent, min_size, max_size);

            entity_set_edict(ent, EV_ENT_owner, owner);
            
            SetEntityGravity(ent, gravity);
            set_rendering(ent, render_fx, r, g, b, render, a);
        } else {
            return -1;
        }

        return ent;		
    }

    /*
    * 生成隱形的實體【建議只用在隱形的發射器，否則建議使用CreateEntity】
    * 如果無論如何都想其顯示，請在外面叫  set_pev(ent, pev(ent, pev_effects) & ~EF_NODRAW);
    *
    * @param clsname        實體classname
    * @param model_route    模組(使用跟子實體同樣的模組)
    * @param movetype       MOVETYPE_*
    * @param owner          主人
    *
    */
    stock CreateInvisibleLauncher(const clsname[], const model_route[], movetype=MOVETYPE_NONE, owner=0) {
        new ent = create_entity("info_target");
        
        if(ent) {
            entity_set_string(ent, EV_SZ_classname, clsname);
            engfunc(EngFunc_SetModel, ent, model_route);
            entity_set_int(ent, EV_INT_solid, SOLID_NOT);
            entity_set_int(ent, EV_INT_movetype, movetype);
            //entity_set_size(ent, Float:{-1.0,-1.0,-1.0}, Float:{1.0,1.0,1.0});

            entity_set_edict(ent, EV_ENT_owner, owner);
            
            //set_rendering(ent, kRenderFxNone, 0,0, 0, kRenderNormal, 0);
            set_pev(ent, pev_effects, pev(ent, pev_effects) | EF_NODRAW);
        } else {
            return -1;
        }

        return ent;	
    }

    /*
    * 刪除實體【只要是TSUBAKI的實體都建議用這function 移除】
    *
    * 製作task時請以右邊方式，15秒後刪除id為{ent}的實體：SetRemoveEntityTask(ent, 15.0) 
    * 或者 參考右邊寫法  set_task_ex(%1, "RemoveTsubakiEntity", TSUBAKI_TASKID_ENTITY_REMOVE(%0), _, _, SetTask_Once)
    *
    * @param clsname        實體classname
    * @param model_route    模組
    * @param movetype       MOVETYPE_*
    * @param owner          主人
    *
    */
    public RemoveTsubakiEntity(taskid) {
        static ent;
        ent = taskid % TSUBAKI_TASKID_ENTITY_REMOVE_BASE;

        if( pev_valid(ent) ) {
            engfunc(EngFunc_RemoveEntity, ent);

            remove_task(TSUBAKI_TASKID_ENTITY_TASK(ent));
            remove_task(TSUBAKI_TASKID_ENTITY_REMOVE(ent));
        }
    }

/*  Temp Entity  */

    stock TBKGetMsgDestination(&client, &bool:reliable) 
    {
        if(!client)
            return reliable ? MSG_ALL : MSG_BROADCAST;
            
        return reliable ? MSG_ONE : MSG_ONE_UNRELIABLE;
    }

    stock TBKCreateBeamBetweenTwoPoints(const Float:startpos[3], const Float:endpos[3], sprite, startframe = 0, framerate = 30, Float:last_time = 1.0, width = 10, noise = 0, r = 0, g = 0, b = 255, a = 75, speed = 0, receiver = 0, bool:reliable = false)
    {
        if(receiver && !is_user_connected(receiver))
            return 0;

        message_begin(TBKGetMsgDestination(receiver, reliable), SVC_TEMPENTITY, .player = receiver);
        write_byte(TE_BEAMPOINTS);
        engfunc(EngFunc_WriteCoord, startpos[0]);
        engfunc(EngFunc_WriteCoord, startpos[1]);
        engfunc(EngFunc_WriteCoord, startpos[2]);
        engfunc(EngFunc_WriteCoord, endpos[0]);
        engfunc(EngFunc_WriteCoord, endpos[1]);
        engfunc(EngFunc_WriteCoord, endpos[2]);
        write_short(sprite);
        write_byte(startframe);
        write_byte(framerate);
        write_byte(floatround(last_time*10));
        write_byte(width);
        write_byte(noise);
        write_byte(r);
        write_byte(g);
        write_byte(b);
        write_byte(a);
        write_byte(speed);
        message_end();

        return 1;
    }

    stock TBKGunshotDecalTE(Float:position[3], decal = 41, entity = 0, receiver = 0, bool:reliable = false)
    {
        if(receiver && !is_user_connected(receiver))
            return 0;

        message_begin(TBKGetMsgDestination(receiver, reliable), SVC_TEMPENTITY, .player = receiver);
        write_byte(TE_GUNSHOTDECAL);
        engfunc(EngFunc_WriteCoord, position[0]);
        engfunc(EngFunc_WriteCoord, position[1]);
        engfunc(EngFunc_WriteCoord, position[2]);
        write_short(entity);
        write_byte(decal);
        message_end();

        return 1;
    }

    stock TBKCreateExplosionTE(Float:position[3], sprite, scale = 50, framerate = 30, flags = TE_EXPLFLAG_NONE, receiver = 0, bool:reliable = false)
    {
        if(receiver && !is_user_connected(receiver))
            return 0;

        message_begin(TBKGetMsgDestination(receiver, reliable), SVC_TEMPENTITY, .player = receiver);
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

/*  Traceline  */

    stock FireNonPenetrateTraceline(const Float:start[3], const Float:dest[3], Float:output[3], &hit=0, &hit_group=0, const skip_ent=0, &Float:fraction=0.0) {
        hit = trace_line(skip_ent, start, dest, output);

        hit_group = traceresult(TR_Hitgroup);
        traceresult(TR_Fraction, fraction);
    }

    stock FirePenetrateTraceline(const Float:start[3], const Float:dest[3], Float:output[3], hits[MAX_PLAYERS], hit_groups[MAX_PLAYERS]="", const skip_ent=0, &Float:fraction=0.0, Float:penetrate_power=30.0) {
        static previous_hit, Float:previous_fraction, hit, i, Float:angles[3];
        CopyVector(output, start);

        angles[0] = dest[0] - start[0];
        angles[1] = dest[1] - start[1];
        angles[2] = dest[2] - start[2];
        GetVectorAngles(angles, angles);
        angles[0] = ToRadian(angles[0]);
        angles[1] = ToRadian(angles[1]);
        angles[2] = ToRadian(angles[2]);

        hit = skip_ent;
        i = 0;

        do{
            hit = trace_line(hit, output, dest, output);
            traceresult(TR_Fraction, fraction);
            //server_print("previous:%d  hit:%d  fraction:%f", previous_hit, hit, fraction);

            if(1<=hit<=MaxClients) {
                hits[i] = hit;
                hit_groups[i] = traceresult(TR_Hitgroup);
                i++;
            } 

            if(hit == 0) {
                //Assume the traceline hit a hard wall
                if(previous_hit==0 && previous_fraction<=0.00001 && fraction<=0.00001){
                    break;
                }

                output[0] += penetrate_power * Cosine(angles[0]) * Cosine(angles[1]);
                output[1] += penetrate_power * Cosine(angles[0]) *   Sine(angles[1]);
                output[2] += penetrate_power *   Sine(angles[0]);
            }

            previous_hit = hit;
            previous_fraction = fraction;
        }while(fraction!=1.0);

        return i;
    }