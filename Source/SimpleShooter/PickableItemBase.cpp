#include "PickableItemBase.h"

#include "ShooterCharacter.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APickableItemBase::APickableItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	//SphereComp->bGenerateOverlapEvents = true;
	SetActorEnableCollision(true);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &APickableItemBase::OnBeginOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &APickableItemBase::OnEndOverlap);

	RootComponent = SphereComp;

	BaseMesh = CreateDefaultSubobject< UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void APickableItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickableItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickableItemBase::PickItem_Implementation(AActor* Pawn)
{
	GEngine->AddOnScreenDebugMessage(5, 5.f, FColor::Red, TEXT("Item picked"));
	
	if (bIsSingleUse)
	{
		Destroy();
	}
}

void APickableItemBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(5, 5.f, FColor::Red, TEXT("Press E to Pickup"));

	if(bIsAutoPickable)
	{
		PickItem(OtherActor);
	}
	else
	{
		//TODO show UI message
	}
}

void APickableItemBase::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//GEngine->AddOnScreenDebugMessage(5, 5.f, FColor::Red, TEXT("Ended overalap"));
	GEngine->RemoveOnScreenDebugMessage(5);

}
