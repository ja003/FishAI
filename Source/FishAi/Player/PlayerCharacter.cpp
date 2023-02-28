// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Waterboarding.h"
#include "WaterBodyActor.h"
#include "WaterBodyComponent.h"
#include "Camera/CameraComponent.h"
#include "FishAi/Constants.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	BallsCollider = CreateDefaultSubobject<USphereComponent>("balls");
	BallsCollider->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	BallsCollider->SetSphereRadius(15);
	BallsCollider->SetRelativeLocation(FVector(0,0,90));
	BallsCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
	BallsCollider->SetCollisionResponseToChannel(COLLISION_WATER, ECR_Overlap);

	BallsCollider->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnBallsBeginOverlap);
	BallsCollider->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnBallsEndOverlap);

	Throwing = CreateDefaultSubobject<UThrowing>("Throwing");
	Throwing->SkeletalMesh = GetMesh();

	ThrowStartRock = CreateDefaultSubobject<USceneComponent>("ThrowStartRock");
	ThrowStartRock->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	Throwing->ThrowStartRock = ThrowStartRock;
	ThrowStartBait = CreateDefaultSubobject<USceneComponent>("ThrowStartBait");
	ThrowStartBait->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	Throwing->ThrowStartBait = ThrowStartBait;
	ThrowStartGrenade = CreateDefaultSubobject<USceneComponent>("ThrowStartGrenade");
	ThrowStartGrenade->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	Throwing->ThrowStartGrenade = ThrowStartGrenade;
	
	Inventory = CreateDefaultSubobject<UInventory>("Inventory");

	Waterboarding = CreateDefaultSubobject<UWaterboarding>("Waterboarding");
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(Waterboarding, &UWaterboarding::OnPlayerBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(Waterboarding, &UWaterboarding::OnPlayerEndOverlap);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = Data->WalkSpeed;
	GetCharacterMovement()->MaxSwimSpeed = Data->SwimSpeed;
	GetCharacterMovement()->JumpZVelocity = Data->JumpZVelocity;
}

void APlayerCharacter::SetSwimming(bool IsSwimming)
{
	IsInWater = IsSwimming;
	GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = IsSwimming;
	GetCharacterMovement()->SetMovementMode(IsSwimming ? MOVE_Swimming : MOVE_Walking);

	if (IsSwimming)
	{
		Throwing->DeselectObjects();
	}
}

void APlayerCharacter::OnBallsBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult)
{
	if(Actor->GetClass()->IsChildOf(AWaterBody::StaticClass()))
	{
		SetSwimming(true);
	}
}

void APlayerCharacter::OnBallsEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I)
{
	if(Actor->GetClass()->IsChildOf(AWaterBody::StaticClass()))
	{
		SetSwimming(false);
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// BUG: after landing the character gets into swim mode..no idea why 
	if (GetCharacterMovement()->IsSwimming() != IsInWater)
	{
		UE_LOG(LogTemp, Log, TEXT("xxx error. swim state not in sync"));
		SetSwimming(IsInWater);
	}
}

