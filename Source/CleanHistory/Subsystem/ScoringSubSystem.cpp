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
	comboTime += DeltaSeconds;
	if (comboTime >= comboDelay)
	{
		comboTime = 0;
		multiplier = 1;
	}
}

void UScoringSubSystem::Score(EScoringTypeEnum scoreType, FVector WorldLocation)
{
	if (lastType != scoreType)
	{
		comboTime = 0;
		multiplier += scoreTypeMultiplier[scoreType];
	}

	switch (scoreType)
	{
	case EScoringTypeEnum::DESTRUCTION:
		scoreDestruction += scoreTypeValues[scoreType] * multiplier;
		break;

	case EScoringTypeEnum::CUT:
		scoreCut += scoreTypeValues[scoreType] * multiplier;
		break;

	case EScoringTypeEnum::BLOOD:
		scoreBlood += scoreTypeValues[scoreType] * multiplier;
		break;

	case EScoringTypeEnum::BURN:
		scoreBurn += scoreTypeValues[scoreType] * multiplier;
		break;

	case EScoringTypeEnum::DEFAULT:
		return;

	default:
		return;
	}

	DisplayScoreFeedback(scoreTypeValues[scoreType], WorldLocation);
}

void UScoringSubSystem::DisplayScoreFeedback(float Score, FVector WorldLocation)
{ 
	if (ScoreFeedbackWidgetClass) 
	{ 
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ScoreFeedbackWidgetClass, WorldLocation, FRotator::ZeroRotator);
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
							MultiplierText->SetText(FText::AsNumber(multiplier));
						}
					}
				}

				/*FTimerHandle WidgetTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(WidgetTimerHandle, FTimerDelegate::CreateLambda([ScoreFeedbackWidget]()
					{
						if (ScoreFeedbackWidget)
						{
							ScoreFeedbackWidget->RemoveFromParent();
						}
					}), 2.0f, false);*/
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

void UScoringSubSystem::SetComboDelay(float delay)
{
	comboDelay = delay;
}
