// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fountain.generated.h"

UCLASS()
class CLEANHISTORY_API AFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFountain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlapFill(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
private: 
	UPROPERTY()
	class ABucket* bucket;

	UPROPERTY()
	FTimerHandle FillTimer;

	UPROPERTY(EditAnywhere)
	float WaterFillLevelPerSecond;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	TObjectPtr<class UStaticMeshComponent> WaterCollision;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;

	UFUNCTION()
	void FillOverTime();

	UFUNCTION()
	void StopFill();
};
