#pragma once

UENUM(BlueprintType)
enum class EFishState : uint8 
{
	Idle     UMETA(DisplayName = "Idle"),
	Bait     UMETA(DisplayName = "Bait"),
	Danger   UMETA(DisplayName = "Danger"),	
	Hunt   UMETA(DisplayName = "Hunt"),
	Rock   UMETA(DisplayName = "Rock"),	
};