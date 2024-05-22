// Fill out your copyright notice in the Description page of Project Settings.

#include "Bucket.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Math/Vector.h"
#include "Misc/OutputDeviceNull.h"
#include "UObject/Class.h"

// Sets default values
ABucket::ABucket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	StaticMeshComponent->Mobility = EComponentMobility::Movable;
	StaticMeshComponent->SetGenerateOverlapEvents(false);
	StaticMeshComponent->bUseDefaultCollision = true;
	RootComponent = StaticMeshComponent;

	CleaningCollisionMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WaterCollisionMesh"));
	CleaningCollisionMeshComponent->SetupAttachment(RootComponent);
	CleaningCollisionMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ABucket::OnBeginOverlapCleaning);
}

// Called when the game starts or when spawned
void ABucket::BeginPlay()
{
	Super::BeginPlay();

	isFull = WaterLevel >= MaxWaterLevel;
	
	waterMeshScale = CleaningCollisionMeshComponent->GetRelativeScale3D().Z;
}

// Called every frame
void ABucket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckRotation();
}

void ABucket::Spill(float angleRad)
{
	float waterSpillProportion = (MaxWaterLevel * FMath::Abs(FMath::Cos(angleRad)))/MaxWaterLevel;
	float waterPorportion = WaterLevel / MaxWaterLevel;
	float missingWaterProportion = (MaxWaterLevel - WaterLevel) / MaxWaterLevel;

	if(waterPorportion < waterSpillProportion || WaterLevel < MinWaterLevelForCleaning)
	{
		return;
	}
	
	float spilledWaterQuantity = WaterLevel * (waterPorportion - waterSpillProportion);
	WaterLevel -= spilledWaterQuantity;
	
	if (WaterLevel < MinWaterLevelForCleaning) {
		WaterLevel = 0;
	}

	OnWaterLevelChanged(WaterLevel);
}

void ABucket::CheckRotation() 
{
	FVector worldUp = FVector(0, 0, 1);
	FVector actorUp = GetActorUpVector();

	float angleRad = FMath::Acos(FVector::DotProduct(actorUp, worldUp) / (actorUp.Normalize() * worldUp.Normalize()));
	float angleDeg = angleRad * 180.f / PI;

	if (angleDeg > angleTreshhold) 
	{
		if(CanBeSpilled)
			Spill(angleRad);
	}
}

void ABucket::OnWaterLevelChanged(float waterLevel) 
{
	FVector waterScale = CleaningCollisionMeshComponent->GetRelativeScale3D();
	waterScale.Z = waterMeshScale * (waterLevel / MaxWaterLevel);
	CleaningCollisionMeshComponent->SetRelativeScale3D(waterScale);
	isFull = WaterLevel <= MaxWaterLevel;
}

void ABucket::OnBeginOverlapCleaning(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UIWashableBroom::StaticClass()))
	{
		if (DirtySaturation >= MaxDirtySaturation || WaterLevel < MinWaterLevelForCleaning || !CanGetDirty)
		{
			return;
		}

		DirtySaturation += CleaningValue;

		FOutputDeviceNull ar;
		const FString command = FString::Printf(TEXT("BPCleanBroom"));
		if (broomBPActor)
		{
			broomBPActor->CallFunctionByNameWithArguments(*command, ar, NULL, true);
		}
	}
}

void ABucket::Fill(int32 quantity)
{
	WaterLevel += quantity;
	isFull = WaterLevel <= MaxWaterLevel;
}