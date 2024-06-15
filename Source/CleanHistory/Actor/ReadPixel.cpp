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

	/*TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Linetrac::StaticClass(), FoundActors);*/

	//TSubclassOf<AActor> ClassToFind; // Needs to be populated somehow (e.g. by exposing to blueprints as uproperty and setting it there

	/*TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);

	/*if (ParentSkelethalMesh == nullptr)
	{
		return;
	}#1#

	UE_LOG(LogTemp, Warning, TEXT("The parent value is:"));*/

	/*if (FoundActors)
	{
		
	}*/

	//UGameplayStatics::GetActorOfClass(GetWorld(), ClassToFind);

	//ProtoLineTr
}

void AReadPixel::TestPrintPixel(UTextureRenderTarget2D* RenderTarget)
{
	/*char* result = TCHAR_TO_ANSI(*text);
	UE_LOG(LogTemp, Warning, TEXT(text));*/// Include the necessary header files

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

	/*for (int i = 0; i < SurfData.Num(); i++)
	{
		const uint8& color = SurfData[i].R;

		bloodQuantity += color;

		/*if (color > 150)
		{
			bloodQuantity += color * 10;
			numberOperation++;
			i += 10;
		}#1#
	}*/

	/*for (FColor& color : SurfData)
	{
		

		//maxInt++;

		/*if (maxInt < 15)
		{
			text = FString::Printf(TEXT("R : %d, G : %d, B : %d, A : %d"), color.R, color.G, color.B, color.A);

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, text);

			maxInt++;
		}#1#

		if (currentIndex >= 10)
		{
			// Do something with the color
			bloodQuantity += color.R;
			numberOperation++;
			currentIndex = 0;
		}
		else
		{
			currentIndex++;
		}
	}*/

	text = FString::Printf(TEXT("maxInt : %d"), numberOperation);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, text);
	
	//bloodQuantity = (static_cast<float>(bloodQuantity) / SurfData.Num()) / 255.f;

	// Divide by the number of pixel to get the % 
	bloodQuantity = ((bloodQuantity) / SurfData.Num());
	// Get a value between 0-1 instead of 0-255
	bloodQuantity /= 255.f;

	return bloodQuantity;
}
