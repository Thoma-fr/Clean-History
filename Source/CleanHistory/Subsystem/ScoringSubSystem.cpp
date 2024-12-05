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


void UScoringSubSystem::SetupAfterInit() 
{
	scoringFeedbackAsset->feedBackFromTreshhold.GetKeys(keys);
	keys.Sort();
}

void UScoringSubSystem::Tick(float DeltaSeconds)
{
	if (scoringDataAsset == nullptr)
		return;

	comboTime += DeltaSeconds;
	specialFeedbackTime += DeltaSeconds;

	if (comboTime >= scoringDataAsset->comboDelay)
	{
		comboTime = 0;
		multiplier = 1;
		lastType = EScoringTypeEnum::DEFAULT;
	}

	if (specialFeedbackTime >= scoringFeedbackAsset->specialFeedBackDelay && comboCpt > keys[0])
	{
		specialFeedbackTime = 0;
		
		float lastKey = -1;
		for (float key : keys)
		{
			if (comboCpt > key)
				lastKey = key;
		}

		if (lastKey > 0)
		{
			DisplaySpecialFeedback(*scoringFeedbackAsset->feedBackFromTreshhold.Find(lastKey), LastComboLoc);
		}
		
		comboCpt = 0;
	}
}

void UScoringSubSystem::Score(EScoringTypeEnum scoreType, FVector WorldLocation)
{
	ScoreWithValue(scoreType, WorldLocation, scoringDataAsset->scoreTypeValues[scoreType], scoringDataAsset->scoreTypeMultiplier[scoreType], false);
}

void UScoringSubSystem::ScoreFromProp(EScoringTypeEnum scoreType, FVector WorldLocation, AProp* prop)
{
	ScoreWithValueAndText(scoreType, WorldLocation, prop->scoreEventValue, prop->scoreEventMultiplier, prop->IsEvent, prop->scoreSpecialText);
}

void UScoringSubSystem::ScoreWithValue(EScoringTypeEnum scoreType, FVector WorldLocation, float ScoreValue, float ScoreMultiplier, bool isEvent)
{
	ScoreWithValueAndText(scoreType, WorldLocation, ScoreValue, ScoreMultiplier, isEvent, "");
}

void UScoringSubSystem::ScoreWithValueAndText(EScoringTypeEnum scoreType, FVector WorldLocation, float ScoreValue, float ScoreMultiplier, bool isEvent, FString specialText)
{
	ScoreValue = ScoreValue >= 0 ? ScoreValue : scoringDataAsset->scoreTypeValues[scoreType];
	ScoreMultiplier = ScoreMultiplier >= 0 ? ScoreMultiplier : scoringDataAsset->scoreTypeMultiplier[scoreType];
	float finalScoreAdde = isEvent ? ScoreValue : ScoreValue * multiplier;

	switch (scoreType)
	{
	case EScoringTypeEnum::DESTRUCTION:
		comboCpt++;
		LastComboLoc = WorldLocation;
		specialFeedbackTime = 0;
		scoreDestruction += finalScoreAdde;
		if (isEvent)
		{
			DisplayScoreFeedbackWithoutMultiplier(finalScoreAdde, WorldLocation);
		}
		else
		{
			DisplayScoreFeedback(finalScoreAdde, WorldLocation);
		}
		break;

	case EScoringTypeEnum::CUT:
		comboCpt++;
		LastComboLoc = WorldLocation;
		specialFeedbackTime = 0;
		scoreCut += finalScoreAdde;
		if (isEvent)
		{
			DisplayScoreFeedbackWithoutMultiplier(finalScoreAdde, WorldLocation);
		}
		else
		{
			DisplayScoreFeedback(finalScoreAdde, WorldLocation);
		}
		break;

	case EScoringTypeEnum::BLOOD:
		scoreBlood += ScoreValue;
		DisplayScoreFeedbackWithoutMultiplier(ScoreValue, WorldLocation);
		break;

	case EScoringTypeEnum::BURN:
		comboCpt++;
		LastComboLoc = WorldLocation;
		specialFeedbackTime = 0;
		scoreBurn += ScoreValue * multiplier;
		if (isEvent)
		{
			DisplayScoreFeedbackWithoutMultiplier(finalScoreAdde, WorldLocation);
		}
		else
		{
			DisplayScoreFeedback(finalScoreAdde, WorldLocation);
		}
		break;

	case EScoringTypeEnum::DEFAULT:
		return;

	default:
		return;
	}

	if (isEvent)
		DisplaySpecialFeedback(specialText, WorldLocation);

	if (lastType != scoreType || isEvent)
	{
		lastType = scoreType;
		comboTime = 0;
		multiplier += ScoreMultiplier;
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

					UTextBlock* MultiplierText = (UTextBlock*)ScoreFeedbackWidget->GetWidgetFromName(TEXT("MultiplierText"));
					
					if (MultiplierText)
					{
						if (multiplier > 1)
						{
							MultiplierText->SetText(FText::FromString(FString("x") + FString::SanitizeFloat(multiplier)));
						}
						else
						{
							MultiplierText->SetText(FText::FromString(FString("")));
						}
					}
				}
			}
		} 
	} 
}

void UScoringSubSystem::DisplayScoreFeedbackWithoutMultiplier(float Score, FVector WorldLocation)
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

					UTextBlock* MultiplierText = (UTextBlock*)ScoreFeedbackWidget->GetWidgetFromName(TEXT("MultiplierText"));

					if (MultiplierText)
					{
						MultiplierText->SetText(FText::FromString(FString("")));
					}
				}
			}
		}
	}
}

void UScoringSubSystem::DisplaySpecialFeedback(FString specialText, FVector WorldLocation)
{
	if (scoringFeedbackAsset->ScoreSpecialFeedbackWidgetClass)
	{
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(scoringFeedbackAsset->ScoreSpecialFeedbackWidgetClass, WorldLocation, FRotator::ZeroRotator);
		SpawnedActor->SetLifeSpan(scoringDataAsset->ScoreFeedbackTime);

		if (SpawnedActor)
		{
			UWidgetComponent* WidgetComp = SpawnedActor->FindComponentByClass<UWidgetComponent>();

			if (WidgetComp)
			{
				UUserWidget* ScoreFeedbackWidget = WidgetComp->GetUserWidgetObject();
				if (ScoreFeedbackWidget)
				{
					UTextBlock* ScoreText = (UTextBlock*)ScoreFeedbackWidget->GetWidgetFromName(TEXT("ScoreEventFeedback"));
					if (ScoreText)
					{
						ScoreText->SetText(FText::FromString(specialText));
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
