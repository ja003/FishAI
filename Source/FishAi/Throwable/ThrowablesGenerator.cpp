// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowablesGenerator.h"

#include "Components/BoxComponent.h"
#include "FishAi/Data/Throwable/ThrowableGeneratorData.h"
#include "FishAi/Player/PlayerCharacter.h"


// Sets default values
AThrowablesGenerator::AThrowablesGenerator()
{
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;

	Collider->SetCollisionResponseToAllChannels(ECR_Ignore);
	Collider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AThrowablesGenerator::OnBeginOverlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	StateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StateMesh"));
	StateMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	StateMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Cooldown = 5;

	PrimaryActorTick.bCanEverTick = true;
}

void AThrowablesGenerator::OnBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult)
{
	if (bIsDisabled)
	{
		UE_LOG(LogTemp, Log, TEXT("xxx generator is disabled"));
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("xxx OnBeginOverlap = %s"), *Actor->GetName());

	float cooldown = GetRemainingCooldown();
	if(cooldown > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("xxx not rdy yet"));
		return;
	}
	
	if(auto player = Cast<APlayerCharacter>(Actor))
	{
		player->Inventory->AddItem(ObjectType);
		LastTimeItemAdded = FDateTime::Now().ToUnixTimestamp();
	}
}

void AThrowablesGenerator::BeginPlay()
{
	Super::BeginPlay();

	Material = StateMesh->GetMaterial(0);

	SetStateOpacity(0);

	Cooldown = Data->Cooldown;
}

void AThrowablesGenerator::SetStateOpacity(float opacity)
{
	UMaterialInstanceDynamic* MI_Throwable = UMaterialInstanceDynamic::Create(Material,this); 
	StateMesh->SetMaterial(0, MI_Throwable);
	MI_Throwable->SetScalarParameterValue(TEXT("Opacity"), opacity);
}

void AThrowablesGenerator::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetRemainingCooldown() < 0)
	{
		// rotate mesh animation
		Mesh->AddRelativeRotation(FRotator(0,1,0));
		return;
	}

	float opacity = GetRemainingCooldown() / Cooldown;
	opacity = FMath::Clamp(opacity, 0, 1);

	SetStateOpacity(opacity);
}

float AThrowablesGenerator::GetRemainingCooldown()
{
	return LastTimeItemAdded + Cooldown - FDateTime::Now().ToUnixTimestamp();
}

void AThrowablesGenerator::Disable()
{
	bIsDisabled = true;
	PrimaryActorTick.SetTickFunctionEnable(false);
	SetStateOpacity(1);
}
