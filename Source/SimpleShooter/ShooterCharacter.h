// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;
UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void FireGun();

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure)
	float GetSelectedWeaponAmmo() const;

	UFUNCTION(BlueprintPure)
	float GetSelectedWeaponMaxAmmo() const;

	UFUNCTION(BlueprintCallable)
	void AddHealth(float HealthToAdd);

	UFUNCTION(BlueprintPure)
	bool CanAddHealth() const;

	UFUNCTION(BlueprintCallable)
	void AddAmmo(int32 Ammo);

	UFUNCTION(BlueprintPure)
	bool CanAddAmmo() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	//void LookUp(float AxisValue);
	void LookUpRate(float AxisValue); //Used for Controllers
	void LookRightRate(float AxisValue);
	void OnUseItem();

	UPROPERTY(EditAnywhere)
	float RotationRate = 80.f;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass = nullptr;

	UPROPERTY()
	AGun* Gun = nullptr;
	AGun* BigPipiGun = nullptr;


};