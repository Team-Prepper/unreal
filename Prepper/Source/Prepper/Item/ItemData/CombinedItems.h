#pragma once

#include "CoreMinimal.h"
#include "CombinedItems.generated.h"

USTRUCT(BlueprintType)
struct FCombinedItems
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combine")
	FString CombinedItem1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combine")
	FString CombinedItem2;

	FCombinedItems()
		: CombinedItem1(TEXT("DefaultString1")), CombinedItem2(TEXT("DefaultString2"))
	{
	}

	// 오버로드된 생성자 선언
	FCombinedItems(const FString& InString1, const FString& InString2)
		: CombinedItem1(InString1), CombinedItem2(InString2)
	{
	}
};