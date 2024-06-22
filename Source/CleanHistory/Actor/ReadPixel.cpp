// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadPixel.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AReadPixel::AReadPixel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AReadPixel::BeginPlay()
{
	Super::BeginPlay();

	
}

void AReadPixel::TestPrintPixel(UTextureRenderTarget2D* RenderTarget)
{


	FString text = "";

	if (RenderTarget)
	{
		text = RenderTarget->GetName();
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *text);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, text);
}

float AReadPixel::GetBloodQuantityFromRenderTarget(UTextureRenderTarget2D* uRenderTarget)
{
	TArray<FColor> SurfData;
	FRenderTarget* RenderTarget = uRenderTarget->GameThread_GetRenderTargetResource();
	RenderTarget->ReadPixels(SurfData);
	float bloodQuantity = 0.0f;

	FString text = "";

	int numberOperation = 0;

	for (FColor& color : SurfData)
	{
		bloodQuantity += color.R;
	}

	text = FString::Printf(TEXT("maxInt : %d"), numberOperation);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, text);
	
	//bloodQuantity = (static_cast<float>(bloodQuantity) / SurfData.Num()) / 255.f;

	// Divide by the number of pixel to get the % 
	bloodQuantity = ((bloodQuantity) / SurfData.Num());
	// Get a value between 0-1 instead of 0-255
	bloodQuantity /= 255.f;

	return bloodQuantity;
}