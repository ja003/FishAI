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
	//UE_LOG(LogTemp, Log, TEXT("xxx OnEdibleFishPerceptionUpdated"));

	Super::OnEdibleFishPerceptionUpdated(Actor, Stimulus);

	//UE_LOG(LogTemp, Log, TEXT("xxx IsReadyForHunt = %s"), IsReadyForHunt() ? TEXT("true"):TEXT("false"));
	
	if(IsReadyForHunt())
	{
		//UE_LOG(LogTemp, Log, TEXT("xxx go hunt"));
		SetState(EFishState::Hunt);
		blackboard->SetValueAsObject(FishBB_Prey, Actor);

		lastHuntTime = FDateTime::Now().ToUnixTimestamp();

		GetWorld()->GetTimerManager().ClearTimer(EndHuntHandle);
		GetWorld()->GetTimerManager().SetTimer(EndHuntHandle, this, &AFishPike::EndHunt, Cast<UPikeData>(Data)->HuntDuration, false);
	}
}

void AFishPike::SetState(EFishState NewState)
{
	Super::SetState(NewState);
	//UE_LOG(LogTemp, Log, TEXT("xxx hunt = %s"), NewState == EFishState::Hunt ? TEXT("true"):TEXT("false"));
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

void AFishPike::OnComponentHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, FVector Normal, const FHitResult& HitResult)
{
	Super::OnComponentHit(PrimitiveComponent, Actor, PrimitiveComponent1, Normal, HitResult);

	//UE_LOG(LogTemp, Log, TEXT("xxx OnComponentHit"));

	if(Cast<AFishBase>(Actor))
	{
		Cast<AFishBase>(Actor)->OnEatenByFish();
	}
}


bool AFishPike::IsReadyForHunt()
{
	return FDateTime::Now().ToUnixTimestamp() - lastHuntTime > Cast<UPikeData>(Data)->HuntCooldown;
}

void AFishPike::OnMouthBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult)
{
	Super::OnMouthBeginOverlap(PrimitiveComponent, Actor, PrimitiveComponent1, I, Arg, HitResult);

	//UE_LOG(LogTemp, Log, TEXT("xxx AFishPike::OnMouthBeginOverlap"));

	if(AFishBase* fish = Cast<AFishBase>(Actor))
	{
		if (fish->Type != EFish::Pike)
			fish->OnEatenByFish();
	}
}

void AFishPike::EndHunt()
{
	//UE_LOG(LogTemp, Log, TEXT("xxx end hunt"));
	SetState(EFishState::Idle);
}

void AFishPike::Roar()
{
	NoiseReporter->ReportNoise(this, GetActorLocation(), nullptr, 1, Cast<UPikeData>(Data)->RoarRange);
	//UE_LOG(LogTemp, Log, TEXT("xxx roar"));
}

FVector AFishPike::GetNextPatrolPoint()
{
	if (Water == nullptr || Water->PatrolPath.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("xxx error: patrol point not set"));
		return GetActorLocation();
	}
	
	// currentPatrolPathIndex++;
	// FVector result = Water->PatrolPath[(currentPatrolPathIndex - 1) % Water->PatrolPath.Num()];
	currentPatrolPathIndex = FMath::RandRange(0, Water->PatrolPath.Num() - 1);
	FVector patrolPoint = Water->PatrolPath[currentPatrolPathIndex];

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys)
	{
		UE_LOG(LogTemp, Log, TEXT("xxx error: UNavigationSystemV1"));
		return GetActorLocation();
	}
	
	FVector randomReachablePoint;
	for (int i = 0; i < 10; ++i)
	{
		int radius = 500;
		bool bSuccess = NavSys->K2_GetRandomReachablePointInRadius(GetWorld(), patrolPoint, randomReachablePoint, radius);
		if (!bSuccess)
		{
			UE_LOG(LogTemp, Log, TEXT("xxx error: K2_GetRandomReachablePointInRadius"));
			break;
		}
		
		if (Water->IsPointInWater(randomReachablePoint))
		{
			//DrawDebugSphere(GWorld, patrolPoint, radius, 10, FColor::Green, false, 5);
			DrawDebugLine(GetWorld(), GetActorLocation(), randomReachablePoint, FColor::Purple, false, 5);


			return randomReachablePoint;
		}
	}
	DrawDebugLine(GetWorld(), GetActorLocation(), patrolPoint, FColor::Purple, false, 5);

	
	//DrawDebugSphere(GWorld, patrolPoint, radius, 10, FColor::Red, false, 5);
	return patrolPoint;
	
	//DrawDebugSphere(GWorld, result, 50, 10, FColor::White, false, .1f);
	//UE_LOG(LogTemp, Log, TEXT("xxx currentPatrolPathIndex = %d"), currentPatrolPathIndex);
}
