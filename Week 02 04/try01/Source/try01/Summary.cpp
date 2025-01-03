// Fill out your copyright notice in the Description page of Project Settings.


#include "Summary.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void USummary::AddPlayerScore(const FString& PlayerName, int32 PlayerScore)
{
    UTextBlock* PlayerText = NewObject<UTextBlock>(this);
    PlayerText->SetText(FText::FromString(FString::Printf(TEXT("%s: %d"), *PlayerName, PlayerScore)));
    if (ScoreContentBox) // ¼ÙÉè VerticalBox °ó¶¨µ½ ScoreListBox
    {
        ScoreContentBox->AddChild(PlayerText);
    }       
}



