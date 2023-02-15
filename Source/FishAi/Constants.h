#pragma once

const FName FishBB_State = "State";
const FName FishBB_Bait = "Bait";
const FName FishBB_Rock = "Rock";
const FName FishBB_Prey = "Prey";
const FName FishBB_Target = "Target";

const FName Tag_Ground = "Ground";
const FName Tag_Water = "Water";
const FName Tag_Rock = "Rock";
const FName Tag_Bait = "Bait";
const FName Tag_Grenade = "Grenade";

#define COLLISION_FISH ECC_GameTraceChannel1
#define COLLISION_THROWABLE_OBJECT ECC_GameTraceChannel2
#define COLLISION_WATER ECC_GameTraceChannel3
#define COLLISION_GROUND ECC_GameTraceChannel4
#define COLLISION_EXPLOSION ECC_GameTraceChannel5
