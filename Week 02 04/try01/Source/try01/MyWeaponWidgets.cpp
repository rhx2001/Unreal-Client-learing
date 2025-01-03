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

// �ӵ�����ʱ���õķ���
void UMyWeaponWidgets::AmmoOnHit() {
    if (OnHitCanvas)
    {
        // ��ʾ Canvas
        ShowAmmoOnHit();

        // ���ö�ʱ����0.2 ������ HideAmmoOnHit
        GetWorld()->GetTimerManager().SetTimer(AmmoOnHitTimerHandle, this, &UMyWeaponWidgets::HideAmmoOnHit, 0.2f, false);
    }
}

// ��ʾ Canvas ��ʵ��
void UMyWeaponWidgets::ShowAmmoOnHit() {
    if (OnHitCanvas)
    {
        OnHitCanvas->SetVisibility(ESlateVisibility::Visible);
    }
}

// ���� Canvas ��ʵ��
void UMyWeaponWidgets::HideAmmoOnHit() {
    if (OnHitCanvas)
    {
        OnHitCanvas->SetVisibility(ESlateVisibility::Hidden);
    }
}