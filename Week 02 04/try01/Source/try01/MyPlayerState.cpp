// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "try01Character.h"
#include "MyUserWidget.h"

AMyPlayerState::AMyPlayerState(){
    PlayerScore = 0;
    bIsReady = false;
    bTakeGun = false;
}

void AMyPlayerState::OnRep_UpdateGrade()
{
   
    // 获取 PlayerController
    APlayerController* PlayerController = Cast<APlayerController>(GetPlayerController());
    if (PlayerController)
    {
        // 获取 PlayerController 所控制的 Pawn
        Atry01Character* Player = Cast<Atry01Character>(PlayerController->GetPawn());
        if (Player)
        {
            // 更新 UI
            if (Player->MyUserWidgetInstance)
            {
                UMyUserWidget* Widget = Cast<UMyUserWidget>(Player->MyUserWidgetInstance);
                if (Widget)
                {
                    Widget->UpdateUi(PlayerScore);
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("MyUserWidgetInstance is not of type UMyUserWidget"));
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("MyUserWidgetInstance is null"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to cast Pawn to Atry01Character"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to get PlayerController from PlayerState"));
    }
}




void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AMyPlayerState, PlayerScore);
    DOREPLIFETIME(AMyPlayerState, bIsReady);
}