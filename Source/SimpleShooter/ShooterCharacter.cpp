// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

#include "Gun.h"
#include "PickableItemBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SimpleShooter/SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	//GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);


	if (GunClassArray.Num() == 0)
		return;

	for (TSubclassOf<AGun> CurrGunClass : GunClassArray)
	{
		auto NewGun = GetWorld()->SpawnActor<AGun>(CurrGunClass);
		NewGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		NewGun->SetOwner(this);

		NewGun->HideActor();
		Guns.Add(NewGun);
	}

	if (Guns.Num() > 0)
	{
		Guns[0]->ShowActor();
	}
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

float AShooterCharacter::GetSelectedWeaponAmmo() const
{
	if (Guns.Num() == 0)
		return 0;

	return Guns[ActiveGunIndex]->Ammo;
}

float AShooterCharacter::GetSelectedWeaponMaxAmmo() const
{
	if (Guns.Num() == 0)
		return 0;

	return Guns[ActiveGunIndex]->MaxAmmo;
}

void AShooterCharacter::AddHealth(float HealthToAdd)
{
	Health = FMath::Min(MaxHealth, Health + HealthToAdd);
}

bool AShooterCharacter::CanAddHealth() const
{
	return Health < MaxHealth;
}

void AShooterCharacter::AddAmmo(int32 Ammo)
{
	Guns[ActiveGunIndex]->AddAmmo(Ammo);
}


bool AShooterCharacter::CanAddAmmo() const
{
	return Guns[ActiveGunIndex]->Ammo < Guns[ActiveGunIndex]->MaxAmmo;
}


// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AShooterCharacter::FireGun);
	PlayerInputComponent->BindAction(TEXT("UseItem"), IE_Pressed, this, &AShooterCharacter::OnUseItem);
	PlayerInputComponent->BindAction(TEXT("Gun1"), IE_Pressed, this, &AShooterCharacter::OnChangeToGun1);
	PlayerInputComponent->BindAction(TEXT("Gun2"), IE_Pressed, this, &AShooterCharacter::OnChangeToGun2);
	PlayerInputComponent->BindAction(TEXT("Gun3"), IE_Pressed, this, &AShooterCharacter::OnChangeToGun3);
	PlayerInputComponent->BindAction(TEXT("NextGun"), IE_Pressed, this, &AShooterCharacter::OnChangeToNextGun);
	PlayerInputComponent->BindAction(TEXT("PrevGun"), IE_Pressed, this, &AShooterCharacter::OnChangeToPrevGun);

}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Max(Health - DamageApplied, 0.f);
	//TODO check if the damage was from the front or back and use different animations based on that
	//UE_LOG(LogTemp, Warning, TEXT("%s Health: %f"), *GetOwner()->GetName(), Health);

	if (IsDead())
	{
		auto GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}

		DetachFromControllerPendingDestroy(); //disables gun + movement
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return Health;
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::FireGun()
{
	if (Guns.Num() == 0)
		return;

	Guns[ActiveGunIndex]->PullTrigger();
}

void AShooterCharacter::OnChangeToGun1()
{
	ChangeToGun(0);
}

void AShooterCharacter::OnChangeToGun2()
{
	ChangeToGun(1);
}

void AShooterCharacter::OnChangeToGun3()
{
	ChangeToGun(2);
}

void AShooterCharacter::OnChangeToNextGun()
{
	if (Guns.Num() == 0)
		return;

	auto NextIndex = ActiveGunIndex;
	NextIndex = ++NextIndex % Guns.Num();
	ChangeToGun(NextIndex);
}

void AShooterCharacter::OnChangeToPrevGun()
{
	if (Guns.Num() == 0)
		return;

	auto NextIndex = ActiveGunIndex;
	if (NextIndex == 0)
		NextIndex = Guns.Num() - 1;
	else
		--NextIndex;

	ChangeToGun(NextIndex);
}

void AShooterCharacter::ChangeToGun(int32 Index)
{
	if (Guns.Num() == 0 || Index >= Guns.Num() || ActiveGunIndex == Index)
		return;

	ActiveGunIndex = Index;
	for (int i = 0; i < Guns.Num(); ++i)
	{
		if (i == Index)
			Guns[i]->ShowActor();
		else
			Guns[i]->HideActor();
	}

	//special case for the default gun that is built in the character model
	if (Index == 0)
	{
		GetMesh()->UnHideBoneByName(TEXT("weapon_r"));
	}
	else
	{
		GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	}

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ChangingWeaponSound, GetActorLocation());

}

void AShooterCharacter::OnUseItem()
{
	TArray<AActor*> Result;
	GetOverlappingActors(Result, APickableItemBase::StaticClass());

	if (Result.Num() == 0)
		return;

	for (AActor* Actor : Result)
	{
		Cast<APickableItemBase>(Actor)->PickItem(this);
	}
}

//void AShooterCharacter::LookUp(float AxisValue)
//{
//	AddControllerPitchInput(AxisValue);
//}

