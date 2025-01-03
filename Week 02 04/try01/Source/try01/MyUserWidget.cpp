// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"
#include "Components/TextBlock.h"

void UMyUserWidget::UpdateUi(uint32 PlayerScore)
{
    if (ScoreTextBlock)
    {
        ScoreTextBlock->SetText(FText::AsNumber(PlayerScore));
    }

}

