// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"

#include "Explosion.h"
#include "FishAi/Constants.h"
#include "FishAi/Fish/FishBase.h"

void AGrenade::OnEnteredWater()
{
	Super::OnEnteredWater();

	FTimerHandle UnusedHandle;
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &AGrenade::Explode, .2f, false);
	
}

void AGrenade::Explode()
{
	UE_LOG(LogTemp, Log, TEXT("xxx explode"));

	GetWorld()->SpawnActor<AExplosion>(ExplosionBP, GetActorTransform());


	// create a collision sphere
	FCollisionShape MyColSphere = FCollisionShape::MakeSphere(500.0f);

	// draw collision sphere
	DrawDebugSphere(GetWorld(), GetActorLocation(), MyColSphere.GetSphereRadius(), 50, FColor::Purple, true);

	
	// create tarray for hit results
	TArray<FHitResult> OutHits;
	
	// check if something got hit in the sweep
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, GetActorLocation(), GetActorLocation(), FQuat::Identity, COLLISION_EXPLOSION, MyColSphere);

	if (isHit)
	{
		// loop through TArray
		for (auto& Hit : OutHits)
		{
			if (AFishBase* fish = Cast<AFishBase>(Hit.GetActor()))
			{
				fish->OnKilledByPlayer();
			}
			
			if (GEngine && Hit.GetActor() != nullptr) 
			{
				// screen log information on what was hit
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Result: %s"), *Hit.GetActor()->GetName()));
				// uncommnet to see more info on sweeped actor
				// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("All Hit Information: %s"), *Hit.ToString()));

				Hit.GetActor()->Destroy();
			}						
		}
	}
	

	Destroy();
}
