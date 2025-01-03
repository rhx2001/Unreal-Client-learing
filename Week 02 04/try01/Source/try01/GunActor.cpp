// Fill out your copyright notice in the Description page of Project Settings.


#include "GunActor.h"
#include "Net/UnrealNetwork.h"
#include "MyWeaponWidgets.h"


// Sets default values
AGunActor::AGunActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/FirstPerson/Blueprints/Weapons")); // 替换为实际路径
	if (WidgetClass.Succeeded())
	{
		MyUserAmmoClass = WidgetClass.Class;
	}

}

// Called when the game starts or when spawned
void AGunActor::BeginPlay()
{
	Super::BeginPlay();
	if (MyUserAmmoClass)
	{
		// 创建 UMG Widget 实例
		MyUserAmmoInstance = CreateWidget<UUserWidget>(GetWorld(), MyUserAmmoClass);
	}
}

// Called every frame
void AGunActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AActor* Player = GetOwner();
	if (Player)
	{
		UMyWeaponWidgets* Crosshair = Cast<UMyWeaponWidgets>(MyUserAmmoInstance);
		Crosshair->CrosshairSpread = FMath::Clamp(GetOwner()->GetVelocity().Size(),0.0f, 10.0f);
		Crosshair->DynamicCrosshair();
	}
}


void AGunActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGunActor, LoadAmmos);
	DOREPLIFETIME(AGunActor, AmmosNum);
}

void AGunActor::OnRep_HandleAmmos()
{
	if (MyUserAmmoInstance)
	{
		Cast<UMyWeaponWidgets>(MyUserAmmoInstance)->UpdateAmmos(LoadAmmos, AmmosNum);
		UE_LOG(LogTemp, Warning, TEXT("Ammos:%d pack:%d"), LoadAmmos,AmmosNum);
	}
}