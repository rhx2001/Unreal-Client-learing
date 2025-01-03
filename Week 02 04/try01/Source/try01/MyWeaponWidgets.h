// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyWeaponWidgets.generated.h"

/**
 * 
 */
class UTextBlock;
class UBorder;
class UCanvasPanel;
UCLASS()
class TRY01_API UMyWeaponWidgets : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(BluePrintReadOnly, meta = (BindWidget))
	UTextBlock* MaxAmmos;

	UPROPERTY(BluePrintReadOnly, meta = (BindWidget))
	UTextBlock* LoadAmmos;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UBorder* TCrosshair;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UBorder* LCrosshair;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UBorder* RCrosshair;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UBorder* BCrosshair;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* OnHitCanvas;

	UPROPERTY(EditAnywhere)
	float CrosshairLength=12.0;
	UPROPERTY(EditAnywhere)
	float CrosshairThickness=4.0;
	UPROPERTY(BlueprintReadOnly)
	float CrosshairSpread=0.0;




public:
	void UpdateAmmos(uint32 LoadAmmosNum, uint32 MaxAmmosNum);
	void DynamicCrosshair();
	void AmmoOnHit();
	void ShowAmmoOnHit();
	void HideAmmoOnHit(); // 用于计时结束后隐藏

private:
	FTimerHandle AmmoOnHitTimerHandle;
};
