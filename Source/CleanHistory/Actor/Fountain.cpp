// Fill out your copyright notice in the Description page of Project Settings.


#include "Fountain.h"
#include "Bucket.h"

// Sets default values
AFountain::AFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	StaticMeshComponent->Mobility = EComponentMobility::Static;
	StaticMeshComponent->SetGenerateOverlapEvents(false);
	StaticMeshComponent->bUseDefaultCollision = true;
	RootComponent = StaticMeshComponent;

	WaterCollision = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WaterCollisionMesh"));
	WaterCollision->SetupAttachment(RootComponent);
	WaterCollision->OnComponentBeginOverlap.AddDynamic(this, &AFountain::OnBeginOverlapFill);
}

// Called when the game starts or when spawned
void AFountain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFountain::OnBeginOverlapFill(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	bucket = Cast<ABucket>(OtherActor);

	if (bucket)
	{
		if (!bucket->isFull) 
		{
			GetWorldTimerManager().SetTimer(FillTimer, this, &AFountain::FillOverTime, 0.1f, true, 1.0f);
		}
	}
}

void AFountain::FillOverTime()
{
	bucket->ABucket::Fill(WaterFillLevelPerSecond/10);

	if (bucket->isFull)
		StopFill();
}

void AFountain::StopFill()
{
	GetWorldTimerManager().ClearTimer(FillTimer);
	bucket = nullptr;
}