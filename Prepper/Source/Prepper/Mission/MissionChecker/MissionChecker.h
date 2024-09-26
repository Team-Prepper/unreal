#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

class AStoryGameMode;

class PREPPER_API IMissionChecker
{
public:
	virtual ~IMissionChecker() = default;
	virtual void SetTargetMode(AStoryGameMode* Target) = 0;
	
	virtual bool CheckClear() = 0;
	virtual float GetClearRate() = 0;
	
	virtual FString GetMissionName() = 0;
	virtual FString GetMissionContent() = 0;
};