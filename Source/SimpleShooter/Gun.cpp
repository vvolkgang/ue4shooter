// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

#define OUT
// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

void AGun::PullTrigger()
{
	if(IsClipEmpty())
	{
		UGameplayStatics::SpawnSoundAttached(EmptyClipSound, Mesh, TEXT("MuzzleFlashSocket"));
		return;
	}

	--Ammo;
	
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));
	
	FHitResult Hit;
	FVector ShotDirection;
	const bool bSuccess = GunTrace(OUT Hit, OUT ShotDirection);

	if(bSuccess && WorldImpactEffect != nullptr)
	{
		//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WorldImpactEffect, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WorldImpactSound, Hit.Location);

		AActor* HitActor = Hit.GetActor();
		if(HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);

			auto OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}	
}

void AGun::AddAmmo(int32 AmmoToAdd)
{
	Ammo = FMath::Min(MaxAmmo, Ammo + AmmoToAdd);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(OUT FHitResult& Hit, OUT FVector& ShotDirection)
{
	auto OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
		return false;

	FVector OwnerLocation;
	FRotator OwnerRotation;
	OwnerController->GetPlayerViewPoint(OUT OwnerLocation, OUT OwnerRotation);
	ShotDirection = -OwnerRotation.Vector();
	
	FVector End = OwnerLocation + OwnerRotation.Vector() * MaxRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(Hit, OwnerLocation, End, ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
		return nullptr;

	return OwnerPawn->GetController();
}

bool AGun::IsClipEmpty() const
{
	return Ammo <= 0;
}

