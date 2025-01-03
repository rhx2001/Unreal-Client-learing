// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Net/UnrealNetwork.h"
UMyGameInstance::UMyGameInstance(){
	bHasSwitched = false;
}


void UMyGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMyGameInstance, bGameStart);
}