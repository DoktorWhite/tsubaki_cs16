#if defined _tsubaki_laser
    #endinput
#endif
#define _tsubaki_laser

#include <tsubaki_skills/tsubaki_common>

#pragma semicolon 1

#define TBK_NORMAL_LASER_DMG_TYPE DMG_SHOCK|DMG_BURN|DMG_PARALYZE

/*  Normal Laser  */

    /**
    *
    * 發射一支不穿透的普通雷射
    * 
    * @param sprite_id  雷射Sprite ID
    * @param skip_ent   第一個無視實體(發射源)
    * @param start  發射位置
    * @param dest   目的地位置
    * @param owner  主人(傷害用)
    * @param fire_ent   發射實體(傷害用)
    * @param damage 傷害
    * @param colors 顏色RGBA
    * @param width  雷射闊度(0~255)
    * @param laser_last_time 雷射維持時間
    *
    */
    NormalLaser(const sprite_id, const skip_ent, const Float:start[3], const Float:dest[3], 
                owner=0, fire_ent=0, Float:damage=5.0, colors[4]={255, 255, 0, 255}, width=200, Float:laser_last_time=0.1) {

        static hit, Float:end[3];
        FireNonPenetrateTraceline(.start=start, 
                                    .dest=dest, 
                                    .output=end, 
                                    .hit=hit, 
                                    .skip_ent=skip_ent);

        if(IsClientValid(hit) && IsPlayerAlive(hit)) {
            ExecuteHamB(Ham_TakeDamage, hit, fire_ent, owner, damage, TBK_NORMAL_LASER_DMG_TYPE);
        }

        TBKCreateBeamBetweenTwoPoints(start, end, sprite_id, .r=colors[0], .g=colors[1], .b=colors[2], .a=colors[3], .width=width, .last_time=laser_last_time);
    }


/*  Normal Penetrate Laser  */
    /**
    *
    * 發射一支穿透的普通雷射
    * 
    * @param sprite_id  雷射Sprite ID
    * @param skip_ent   第一個無視實體(發射源)
    * @param start  發射位置
    * @param dest   目的地位置
    * @param owner  主人(傷害用)
    * @param fire_ent   發射實體(傷害用)
    * @param damage 傷害
    * @param colors 顏色RGBA
    * @param width  雷射闊度(0~255)
    * @param laser_last_time 雷射維持時間
    *
    */
    NormalPenetrateLaser(const sprite_id, const skip_ent, const Float:start[3], const Float:dest[3], 
                owner=0, fire_ent=0, Float:damage=5.0, colors[4]={255, 0, 255, 255}, width=200, Float:laser_last_time=0.1) {

        static hits[MAX_PLAYERS], Float:end[3], total, i;
        total = FirePenetrateTraceline(.start=start, 
                                    .dest=dest, 
                                    .output=end, 
                                    .hits=hits, 
                                    .skip_ent=skip_ent);

        for(i=0; i<total; i++) {
            if(IsClientValid(hits[i]) && IsPlayerAlive(hits[i])) {
                ExecuteHamB(Ham_TakeDamage, hits[i], fire_ent, owner, damage, TBK_NORMAL_LASER_DMG_TYPE);
            }
        }

        TBKCreateBeamBetweenTwoPoints(start, end, sprite_id, .r=colors[0], .g=colors[1], .b=colors[2], .a=colors[3], .width=width, .last_time=laser_last_time);
    }

/*  Pointing Laser  */
    /**
    *
    * 發射一支不穿透的雷射，在終黠加上擊中效果及可帶有擊飛效果
    * 
    * @param sprite_id  雷射Sprite ID
    * @param skip_ent   第一個無視實體(發射源)
    * @param start  發射位置
    * @param dest   目的地位置
    * @param owner  主人(傷害用)
    * @param fire_ent   發射實體(傷害用)
    * @param damage 傷害
    * @param colors 顏色RGBA
    * @param width  雷射闊度(0~255)
    * @param laser_last_time 雷射維持時間
    * @param slap 擊飛?
    *
    */
    PointingLaser(const sprite_id, const Float:start[3], const Float:dest[3], owner=0, Float:damage=5.0, colors[4]={255, 0, 255, 255}, width=200, Float:laser_last_time=0.1, bool:slap=true) {
 
        static hit, Float:end[3];
        FireNonPenetrateTraceline(.start=start, 
                                    .dest=dest, 
                                    .output=end, 
                                    .hit=hit, 
                                    .skip_ent=owner);

        if(IsClientValid(hit) && IsPlayerAlive(hit)) {
            ExecuteHamB(Ham_TakeDamage, hit, 0, owner, damage, TBK_NORMAL_LASER_DMG_TYPE);
            if(slap) {
                user_slap(hit, 0);
            }
        }

        TBKCreateBeamBetweenTwoPoints(start, end, sprite_id, .r=colors[0], .g=colors[1], .b=colors[2], .a=colors[3], .width=width, .last_time=laser_last_time);
        TBKGunshotDecalTE(end);
    }

/*  Average Laser  */
    
    /**
    *
    * 發射多支不穿透的雷射，在終黠加上擊中效果及可帶有擊飛效果
    * (運算能力稍為向上)
    * 
    * @param sprite_id  雷射Sprite ID
    * @param skip_ent   第一個無視實體(發射源)
    * @param laser_amount 雷射數量
    * @param starts 發射位置s
    * @param dests   目的地位置s
    * @param owner  主人(傷害用)
    * @param fire_ent   發射實體(傷害用)
    * @param damage 傷害
    * @param colors 顏色RGBA
    * @param width  雷射闊度(0~255)
    * @param laser_last_time 雷射維持時間
    *
    */
    MultipleNormalLaser(const sprite_id, const skip_ent, const laser_amount, const Float:starts[][3], const Float:dests[][3], 
                        owner=0, fire_ent=0, Float:damage=5.0, colors[4]={255, 255, 0, 255}, width=200, Float:laser_last_time=0.1) {

        static i, hit, Float:end[3];
        for(i=0; i<laser_amount; i++) {
            FireNonPenetrateTraceline(.start=starts[i], 
                                        .dest=dests[i], 
                                        .output=end, 
                                        .hit=hit, 
                                        .skip_ent=skip_ent);
                                        
            if(IsClientValid(hit) && IsPlayerAlive(hit)) {
                ExecuteHamB(Ham_TakeDamage, hit, fire_ent, owner, damage, TBK_NORMAL_LASER_DMG_TYPE);
            }
            
            TBKCreateBeamBetweenTwoPoints(starts[i], end, sprite_id, .r=colors[0], .g=colors[1], .b=colors[2], .a=colors[3], .width=width, .last_time=laser_last_time);
        } 
    }



#pragma semicolon 0