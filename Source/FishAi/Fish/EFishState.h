#pragma once

UENUM(BlueprintType)
enum class EFishState : uint8 
{
	Idle     UMETA(DisplayName = "Idle"),
	Bait     UMETA(DisplayName = "Bait"),
	Danger   UMETA(DisplayName = "Danger"),	
};