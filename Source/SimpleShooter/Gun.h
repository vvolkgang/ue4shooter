// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

class UParticleSystem;
UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGun();
	virtual void Tick(float DeltaTime) override;
	void PullTrigger();

	UPROPERTY(EditAnywhere)
		float MaxAmmo = 10.f;

	UPROPERTY(VisibleAnywhere)
		float Ammo = MaxAmmo;

	void AddAmmo(int32 AmmoToAdd);
	void HideActor();
	void ShowActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
		USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
		UParticleSystem* WorldImpactEffect;

	UPROPERTY(EditAnywhere)
		USoundBase* WorldImpactSound;

	UPROPERTY(EditAnywhere)
		USoundBase* EmptyClipSound;

	UPROPERTY(EditAnywhere)
		float MaxRange = 1000.f;

	UPROPERTY(EditAnywhere)
		float Damage = 10.f;

	bool GunTrace(OUT FHitResult& Hit, OUT FVector& ShotDirection);

	AController* GetOwnerController() const;

	bool IsClipEmpty() const;

};
