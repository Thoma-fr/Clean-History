// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TextureRenderTarget2D.h"

#include "ReadPixel.generated.h"

UCLASS()
class CLEANHISTORY_API AReadPixel : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AReadPixel();

	UFUNCTION(BlueprintCallable, Category = "BloodHandleFunctions")
	static float GetBloodQuantityFromRenderTarget(UTextureRenderTarget2D* RenderTarget);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ClassToFind;

	UFUNCTION(BlueprintCallable)
	void TestPrintPixel(UTextureRenderTarget2D* RenderTarget);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
