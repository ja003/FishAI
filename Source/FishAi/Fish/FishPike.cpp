#include "FishPike.h"

#include "EFishState.h"
#include "NavigationSystem.h"
#include "Components/TextRenderComponent.h"
#include "FishAi/Constants.h"
#include "FishAi/Managers/WaterManager.h"
#include "FishAi/Data/DataManager.h"
#include "FishAi/Data/Fish/PikeData.h"
#include "FishAi/Managers/NoiseReporter.h"
#include "Kismet/GameplayStatics.h"

AFishPike::AFishPike()
{
	StateText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("StateText"));
	StateText->AttachToComponent(bodyMesh, FAttachmentTransformRules::KeepRelativeTransform);
	StateText->SetVisibility(false);
}

void AFishPike::OnEdibleFishPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus)
{
	Super::OnEdibleFishPerceptionUpdated(Actor, Stimulus);

	if(IsReadyForHunt())
	{
		SetState(EFishState::Hunt);
		blackboard->SetValueAsObject(FishBB_Prey, Actor);

		GetWorld()->GetTimerManager().ClearTimer(EndHuntHandle);
		GetWorld()->GetTimerManager().SetTimer(EndHuntHandle, this, &AFishPike::EndHunt, Cast<UPikeData>(Data)->HuntDuration, false);
	}
}

void AFishPike::OnBaitPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus)
{
	Super::OnBaitPerceptionUpdated(Actor, Stimulus);

	// pike perceives a bait as a rock
	RunawayFrom(Stimulus.StimulusLocation, Data->RockRunawayDistance, EFishState::Rock);
}

void AFishPike::SetState(EFishState NewState)
{
	if (GetState() == EFishState::Hunt)
	{
		UE_LOG(LogTemp, Log, TEXT("xxx end hunt"));
		lastHuntTime = FDateTime::Now().ToUnixTimestamp();
	}
	
	Super::SetState(NewState);
	StateText->SetVisibility(NewState == EFishState::Hunt);
}

void AFishPike::BeginPlay()
{
	Super::BeginPlay();
	
	//dont hunt right away
	lastHuntTime = FDateTime::Now().ToUnixTimestamp();
	lastHuntTime = -99;

	GetWorld()->GetTimerManager().SetTimer(RoarHandle, this, &AFishPike::Roar, Cast<UPikeData>(Data)->RoarCoolDown, true);
}

void AFishPike::Init(AWaterManager* InWater)
{
	Super::Init(InWater);

	currentPatrolPathIndex = FMath::RandRange(0, Water->PatrolPath.Num() - 1);
}

bool AFishPike::IsReadyForHunt()
{
	return FDateTime::Now().ToUnixTimestamp() - lastHuntTime > Cast<UPikeData>(Data)->HuntCooldown;
}

void AFishPike::OnMouthBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult)
{
	// 2 fishes can be spawned at the same location - we dont want
	// them to overlap right away
	if (GetGameTimeSinceCreation() < 1)
	{
		UE_LOG(LogTemp, Log, TEXT("xxx 2 fishes spawned too close to each other"));
		return;	
	}
	
	Super::OnMouthBeginOverlap(PrimitiveComponent, Actor, PrimitiveComponent1, I, Arg, HitResult);

	if(AFishBase* fish = Cast<AFishBase>(Actor))
	{
		if (fish->FishType != EFish::Pike)
		{
			fish->OnEatenByFish();
			SetState(EFishState::Idle);
		}
	}
}

void AFishPike::EndHunt()
{
	SetState(EFishState::Idle);
}

void AFishPike::Roar()
{
	NoiseReporter->ReportNoise(this, GetActorLocation(), nullptr, 1, Cast<UPikeData>(Data)->RoarRange);
}

FVector AFishPike::GetNextPatrolPoint()
{
	if (Water == nullptr || Water->PatrolPath.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("xxx error: patrol point not set"));
		return GetActorLocation();
	}
	
	currentPatrolPathIndex = FMath::RandRange(0, Water->PatrolPath.Num() - 1);
	FVector patrolPoint = Water->PatrolPath[currentPatrolPathIndex];

	// BUG
	// it seems that patrol points are sometimes unreachable and sending the pike
	// there sets it into invalid state and it falls through a navmesh.
	// I try to fix it by using GetRandomReachablePoint but no luck so far..
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys)
	{
		UE_LOG(LogTemp, Error, TEXT("xxx error: UNavigationSystemV1"));
		return GetActorLocation();
	}
	
	FVector randomReachablePoint;
	for (int i = 0; i < 10; ++i)
	{
		int radius = 500;
		bool bSuccess = NavSys->K2_GetRandomReachablePointInRadius(GetWorld(), patrolPoint, randomReachablePoint, radius);
		if (!bSuccess)
		{
			UE_LOG(LogTemp, Error, TEXT("xxx error: K2_GetRandomReachablePointInRadius"));
			break;
		}
		
		if (Water->IsPointInWater(randomReachablePoint))
		{
			//DrawDebugSphere(GWorld, patrolPoint, radius, 10, FColor::Green, false, 5);
			//DrawDebugLine(GetWorld(), GetActorLocation(), randomReachablePoint, FColor::Purple, false, 5);


			return randomReachablePoint;
		}
	}
	//DrawDebugLine(GetWorld(), GetActorLocation(), patrolPoint, FColor::Purple, false, 5);

	
	//DrawDebugSphere(GWorld, patrolPoint, radius, 10, FColor::Red, false, 5);
	return patrolPoint;
	
	//DrawDebugSphere(GWorld, result, 50, 10, FColor::White, false, .1f);
	//UE_LOG(LogTemp, Log, TEXT("xxx currentPatrolPathIndex = %d"), currentPatrolPathIndex);
}
