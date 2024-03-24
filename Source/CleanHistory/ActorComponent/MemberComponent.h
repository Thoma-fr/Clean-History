// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MemberComponent.generated.h"

class USkeletalMeshComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLEANHISTORY_API UMemberComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMemberComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere)
		TObjectPtr<USkeletalMeshComponent> ParentSkelethalMesh;
	UPROPERTY(EditAnywhere)
		TObjectPtr<USkeletalMeshComponent> MemberMesh;
	UPROPERTY(EditAnywhere)
		TObjectPtr<class UBoxComponent> CutZone;
	UPROPERTY(EditAnywhere)
		TObjectPtr<class USceneComponent> BleedPoint;
		
};
