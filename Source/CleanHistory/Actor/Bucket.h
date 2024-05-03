// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Prop.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bucket.generated.h"

UCLASS()
class CLEANHISTORY_API ABucket : public AProp
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABucket();

protected:
	// Called when the game starts or when spawned 
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlapCleaning(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnBeginOverlapFill(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void Spill(float angleRad);

	UFUNCTION()
	void CheckRotation();

	UFUNCTION()
	void OnWaterLevelChanged(float waterLevel);

private:
	UPROPERTY(EditAnywhere)
	bool CanBeSpilled;

	UPROPERTY(EditAnywhere)
	bool CanGetDirty;

	UPROPERTY(EditAnywhere)
	float WaterLevel;

	UPROPERTY(EditAnywhere)
	float MaxWaterLevel;

	UPROPERTY(EditAnywhere)
	float MinWaterLevelForCleaning;

	UPROPERTY(EditAnywhere)
	float DirtySaturation;

	UPROPERTY(EditAnywhere)
	float MaxDirtySaturation;

	UPROPERTY(EditAnywhere)
	float CleaningValue;

	UPROPERTY(EditAnywhere)
	uint32 angleTreshhold;
	
	UPROPERTY(EditAnywhere)
	float waterMeshScale;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	TObjectPtr<class UStaticMeshComponent> CleaningCollisionMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	TObjectPtr<class UBoxComponent> FillCollisionBoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;
};
