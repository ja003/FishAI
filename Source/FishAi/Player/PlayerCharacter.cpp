// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "WaterBodyActor.h"
#include "WaterBodyComponent.h"
#include "Camera/CameraComponent.h"
#include "FishAi/Constants.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BallsCollider = CreateDefaultSubobject<USphereComponent>("balls");
	BallsCollider->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	BallsCollider->SetSphereRadius(15);
	BallsCollider->SetRelativeLocation(FVector(0,0,90));
	BallsCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
	BallsCollider->SetCollisionResponseToChannel(COLLISION_WATER, ECR_Overlap);

	BallsCollider->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnBallsBeginOverlap);
	BallsCollider->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnBallsEndOverlap);

	//USpringArmComponent* springArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	//springArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//UCameraComponent* camera = CreateDefaultSubobject<UCameraComponent>("");
	//camera->AttachToComponent(springArm, FAttachmentTransformRules::KeepRelativeTransform);

	Throwing = CreateDefaultSubobject<UThrowing>("Throwing");
	Throwing->SkeletalMesh = GetMesh();

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCharacter::SetSwimming(bool IsSwimming)
{
	IsInWater = IsSwimming;
	GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = IsSwimming;
	GetCharacterMovement()->SetMovementMode(IsSwimming ? MOVE_Swimming : MOVE_Walking);
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

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

