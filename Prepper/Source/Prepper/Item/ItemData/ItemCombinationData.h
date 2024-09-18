// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemCombinationData.generated.h"

USTRUCT(BlueprintType)
struct PREPPER_API FItemCombinationData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelUp")
	FString InputItemCode1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelUp")
	FString InputItemCode2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelUp")
	FString OutputItemCode;
	FItemCombinationData()
		: InputItemCode1(TEXT("DefaultString1")), InputItemCode2(TEXT("DefaultString2"))
	{
	}
	
	// 오버로드된 생성자 선언
	FItemCombinationData(const FString& InString1, const FString& InString2, const FString& OutString)
		: InputItemCode1(InString1), InputItemCode2(InString2), OutputItemCode(OutString)
	{
		
	}
};

