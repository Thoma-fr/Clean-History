// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Prop.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

#include "CorpseMembers.generated.h"

class ABloodManager;
class USkeletalMesh;
UCLASS(Blueprintable)
class CLEANHISTORY_API ACorpseMembers : public AProp
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACorpseMembers();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DetacheAllChildOnDIe;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> ParentSkelethalMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> MemberMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBoxComponent> CutZone;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<class USceneComponent> BleedPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABloodManager> bloodManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MemberLife=100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsHidden = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CountAsCorpse = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool MustEject = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Debug = false;
	UPROPERTY()
	TObjectPtr<ABloodManager> myBloodManager;
	//UPROPERTY()
	UPROPERTY(EditAnywhere)
	FName SocketName;

	UPROPERTY()
	FVector lastPos;

	UPROPERTY()
	bool hasDetached;
	UFUNCTION(BlueprintCallable)
	void Die() override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USoundBase* HitSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* DismenberSound;;
	UPROPERTY(BlueprintReadWrite)
	bool WantPhysic;
	/*UFUNCTION()
		void ChangeParent(USkeletalMesh* parent);*/
	UFUNCTION()
		void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintCallable)
		void Eject();

private:
	UPROPERTY()
	TArray<TObjectPtr<USkeletalMeshComponent>> ChildsMesh;

	UPROPERTY()
	TArray<FVector> ChildsMeshLastPos;
	UPROPERTY()
	bool Burned;
	UFUNCTION(BlueprintCallable)
	void Detache();
	UFUNCTION(BlueprintCallable)
	void DetacheAll();
};
