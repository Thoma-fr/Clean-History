#pragma once

#include "ScoringSubSystem.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h" 
#include "Kismet/GameplayStatics.h" 

void UScoringSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	lastType = EScoringTypeEnum::DEFAULT;
}

void UScoringSubSystem::Deinitialize()
{
	Super::Deinitialize();
}

void UScoringSubSystem::Tick(float DeltaSeconds)
{
	if (scoringDataAsset == nullptr)
		return;

	comboTime += DeltaSeconds;
	if (comboTime >= scoringDataAsset->comboDelay)
	{
		comboTime = 0;
		multiplier = 1;
	}
}

void UScoringSubSystem::Score(EScoringTypeEnum scoreType, FVector WorldLocation)
{
	switch (scoreType)
	{
	case EScoringTypeEnum::DESTRUCTION:
		scoreDestruction += scoringDataAsset->scoreTypeValues[scoreType] * multiplier;
		break;

	case EScoringTypeEnum::CUT:
		scoreCut += scoringDataAsset->scoreTypeValues[scoreType] * multiplier;
		break;

	case EScoringTypeEnum::BLOOD:
		scoreBlood += scoringDataAsset->scoreTypeValues[scoreType] * multiplier;
		break;

	case EScoringTypeEnum::BURN:
		scoreBurn += scoringDataAsset->scoreTypeValues[scoreType] * multiplier;
		break;

	case EScoringTypeEnum::DEFAULT:
		return;

	default:
		return;
	}

	DisplayScoreFeedback(scoringDataAsset->scoreTypeValues[scoreType]*multiplier, WorldLocation);

	if (lastType != scoreType)
	{
		comboTime = 0;
		if (multiplier <= 1)
		{
			multiplier = scoringDataAsset->scoreTypeMultiplier[scoreType];
		}
		else
		{
			multiplier += scoringDataAsset->scoreTypeMultiplier[scoreType];
		}
	}
}

void UScoringSubSystem::DisplayScoreFeedback(float Score, FVector WorldLocation)
{ 
	if (scoringDataAsset->ScoreFeedbackWidgetClass)
	{ 
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(scoringDataAsset->ScoreFeedbackWidgetClass, WorldLocation, FRotator::ZeroRotator);
		SpawnedActor->SetLifeSpan(scoringDataAsset->ScoreFeedbackTime);

		if (SpawnedActor)
		{ 
			UWidgetComponent* WidgetComp = SpawnedActor->FindComponentByClass<UWidgetComponent>();

			if (WidgetComp)
			{
				UUserWidget* ScoreFeedbackWidget = WidgetComp->GetUserWidgetObject();
				if (ScoreFeedbackWidget)
				{
					UTextBlock* ScoreText = (UTextBlock*)ScoreFeedbackWidget->GetWidgetFromName(TEXT("ScoreText"));
					if (ScoreText)
					{
						ScoreText->SetText(FText::AsNumber(Score));
					}

					if (multiplier > 1)
					{
						UTextBlock* MultiplierText = (UTextBlock*)ScoreFeedbackWidget->GetWidgetFromName(TEXT("MultiplierText"));
						if (MultiplierText)
						{
							MultiplierText->SetText(FText::FromString(FString::Printf(TEXT("x%.1f"), multiplier)));
						}
					}
				}
			}
		} 
	} 
}

int UScoringSubSystem::GetFinalScore()
{
	return scoreDestruction + scoreBlood + scoreCut + scoreBurn;
}

int UScoringSubSystem::GetScore(EScoringTypeEnum scoreType)
{
	switch (scoreType)
	{
	case EScoringTypeEnum::DESTRUCTION :
		return scoreDestruction;
		break;

	case EScoringTypeEnum::CUT :
		return scoreCut;
		break;
		
	case EScoringTypeEnum::BLOOD :
		return scoreBlood;
		break; 

	case EScoringTypeEnum::BURN :
		return scoreBurn;
		break;

	case EScoringTypeEnum::DEFAULT :
		return -1;
		break;

	default:
		return -1;
		break;
	}
}

TStatId UScoringSubSystem::GetStatId() const
{
	return TStatId();
}
