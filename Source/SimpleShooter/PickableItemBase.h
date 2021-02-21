// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "ShooterCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "PickableItemBase.generated.h"

UCLASS()
class SIMPLESHOOTER_API APickableItemBase : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Pickable Details")
	bool bIsAutoPickable = true; // if true, colliding with the item will automatically pick it

	UPROPERTY(EditAnywhere, Category = "Pickable Details")
	bool bIsSingleUse = true; // if true, actor is destroyed after the first pick

	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Sets default values for this actor's properties
	APickableItemBase();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void PickItem(AActor* Pawn);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
