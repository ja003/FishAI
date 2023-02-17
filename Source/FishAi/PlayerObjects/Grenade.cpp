// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"

#include "Explosion.h"
#include "FishAi/Constants.h"
#include "FishAi/Fish/FishBase.h"

AGrenade::AGrenade()
{
	Tags.Add(Tag_Grenade);
}

void AGrenade::OnEnteredWater()
{
	Super::OnEnteredWater();

	FTimerHandle UnusedHandle;
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &AGrenade::Explode, .2f, false);
	
}

void AGrenade::Explode()
{
	//UE_LOG(LogTemp, Log, TEXT("xxx explode"));


	Radius = 500.0f;
	// create a collision sphere
	FCollisionShape MyColSphere = FCollisionShape::MakeSphere(Radius);

	// draw collision sphere
	DrawDebugSphere(GetWorld(), GetActorLocation(), MyColSphere.GetSphereRadius(), 50, FColor::Purple, false, 2);
	
	// create tarray for hit results
	TArray<FHitResult> OutHits;
	
	// check if something got hit in the sweep
	if (GetWorld()->SweepMultiByChannel(OutHits, GetActorLocation(), GetActorLocation(), FQuat::Identity, COLLISION_EXPLOSION, MyColSphere))
	{
		// loop through TArray
		for (auto& Hit : OutHits)
		{
			if (AFishBase* fish = Cast<AFishBase>(Hit.GetActor()))
			{
				FVector dir = fish->GetActorLocation() - GetActorLocation();
				dir.Normalize();
				dir.Z += 3;

				ExplosionStrength = 100000;
				FVector impulse = dir * ExplosionStrength;
				
				fish->OnKilledByGrenade(impulse);
			}
			
			if (GEngine && Hit.GetActor() != nullptr) 
			{
				// screen log information on what was hit
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Result: %s"), *Hit.GetActor()->GetName()));
				// uncommnet to see more info on sweeped actor
				// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("All Hit Information: %s"), *Hit.ToString()));

				//Hit.GetActor()->Destroy();
			}						
		}
	}

	GetWorld()->SpawnActor<AExplosion>(ExplosionBP, GetActorTransform());	

	Destroy();
}
