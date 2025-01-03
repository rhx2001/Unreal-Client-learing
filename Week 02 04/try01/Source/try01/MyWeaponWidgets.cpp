// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeaponWidgets.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/SizeBoxSlot.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"

void UMyWeaponWidgets::UpdateAmmos(uint32 LoadAmmosNum, uint32 MaxAmmosNum)
{
    LoadAmmos->SetText(FText::AsNumber(LoadAmmosNum));
    MaxAmmos->SetText(FText::AsNumber(MaxAmmosNum));

}

void UMyWeaponWidgets::DynamicCrosshair()
{
    UCanvasPanelSlot* TCrosshairSlot = Cast<UCanvasPanelSlot>(TCrosshair->Slot);
    UCanvasPanelSlot* BCrosshairSlot = Cast<UCanvasPanelSlot>(BCrosshair->Slot);
    UCanvasPanelSlot* LCrosshairSlot = Cast<UCanvasPanelSlot>(LCrosshair->Slot);
    UCanvasPanelSlot* RCrosshairSlot = Cast<UCanvasPanelSlot>(RCrosshair->Slot);
    TCrosshairSlot->SetSize(FVector2D(CrosshairThickness, CrosshairLength));
    BCrosshairSlot->SetSize(FVector2D(CrosshairThickness, CrosshairLength));
    LCrosshairSlot->SetSize(FVector2D(CrosshairLength, CrosshairThickness));
    RCrosshairSlot->SetSize(FVector2D(CrosshairLength, CrosshairThickness));

    TCrosshairSlot->SetPosition(FVector2D(-CrosshairThickness/2, -CrosshairLength - CrosshairLength / 2 - CrosshairSpread));
    BCrosshairSlot->SetPosition(FVector2D(-CrosshairThickness / 2, CrosshairLength/2 + CrosshairSpread));
    LCrosshairSlot->SetPosition(FVector2D(-CrosshairLength - CrosshairLength/2 - CrosshairSpread, -CrosshairThickness / 2));
    RCrosshairSlot->SetPosition(FVector2D(CrosshairLength/2 + CrosshairSpread, -CrosshairThickness/2));
}

// 子弹击中时调用的方法
void UMyWeaponWidgets::AmmoOnHit() {
    if (OnHitCanvas)
    {
        // 显示 Canvas
        ShowAmmoOnHit();

        // 设置定时器，0.2 秒后调用 HideAmmoOnHit
        GetWorld()->GetTimerManager().SetTimer(AmmoOnHitTimerHandle, this, &UMyWeaponWidgets::HideAmmoOnHit, 0.2f, false);
    }
}

// 显示 Canvas 的实现
void UMyWeaponWidgets::ShowAmmoOnHit() {
    if (OnHitCanvas)
    {
        OnHitCanvas->SetVisibility(ESlateVisibility::Visible);
    }
}

// 隐藏 Canvas 的实现
void UMyWeaponWidgets::HideAmmoOnHit() {
    if (OnHitCanvas)
    {
        OnHitCanvas->SetVisibility(ESlateVisibility::Hidden);
    }
}