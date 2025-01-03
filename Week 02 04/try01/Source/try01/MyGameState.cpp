// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameState.h"
#include "MyPlayerState.h"
#include "Net/UnrealNetwork.h"

//初始化
AMyGameState::AMyGameState()
{
    CountdownTime = 0;
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/FirstPerson/Blueprints/SummaryWidget"));
    if (WidgetClassFinder.Succeeded())
    {
        SummaryWidgetClass = WidgetClassFinder.Class;
    }
    if (!SummaryWidget && SummaryWidgetClass)
    {
        SummaryWidget = CreateWidget<USummary>(GetWorld(), SummaryWidgetClass);
    }
}



//倒计时部分
void AMyGameState::OnRep_CountDownTime()
{
    UE_LOG(LogTemp, Log, TEXT("Countdown: %d"), CountdownTime);
}

void AMyGameState::HandleCountTime()
{
    
    if (CountdownTime > 0)
    {
        CountdownTime--;
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);

        // 倒计时结束，广播得分
        ReportPlayerScore();
    }
}

void AMyGameState::StartCountDown()
{
    if (HasAuthority())
    {
        CountdownTime = MaxCountTime;
        GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &AMyGameState::HandleCountTime, 1.0f, true);
    }
}

void AMyGameState::ReportPlayerScore_Implementation()
{

    //if (SummaryWidget)
    //{
    //   
    //    SummaryWidget->SetVisibility(ESlateVisibility::Hidden);
    //}
    if (SummaryWidget)
    {
        SummaryWidget->AddToViewport();
        for (APlayerState* PlayerState : PlayerArray)
        {
            if (AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(PlayerState))
            {
                SummaryWidget->AddPlayerScore(
                    MyPlayerState->GetPlayerName(),
                    MyPlayerState->PlayerScore
                );
                UE_LOG(LogTemp, Log, TEXT("Player: %s, Score: %d"),
                    *MyPlayerState->GetPlayerName(), MyPlayerState->PlayerScore);
            }
        }
        SummaryWidget->SetVisibility(ESlateVisibility::Visible);
    }
}


void AMyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMyGameState, CountdownTime);
}