#pragma once

const FName FishBB_State = "State";
const FName FishBB_Bait = "Bait";
const FName FishBB_Rock = "Rock";
const FName FishBB_Prey = "Prey";
const FName FishBB_Target = "Target";

const FName Tag_Ground = "Ground";
const FName Tag_Water = "Water";
const FName Tag_Rock = "Rock";

#define COLLISION_FISH ECC_GameTraceChannel1
#define COLLISION_THROWABLE_OBJECT ECC_GameTraceChannel2
#define COLLISION_WATER ECC_GameTraceChannel3
//#define COLLISION_PLAYER ECC_GameTraceChannel4
