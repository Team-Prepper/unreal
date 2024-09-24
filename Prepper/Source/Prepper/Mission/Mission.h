#pragma once

#include "CoreMinimal.h"
#include "Prepper/GameMode/PrepperGameMode.h"
#include "UObject/Interface.h"
#include "Mission.generated.h"

UINTERFACE(MinimalAPI)
class UMission : public UInterface
{
	GENERATED_BODY()
};

class PREPPER_API IMission
{
	GENERATED_BODY()
public:
	virtual void SetTargetMode(APrepperGameMode* Target) PURE_VIRTUAL();
	
	virtual bool CheckClear() PURE_VIRTUAL( IMission::CheckClear, return false; );
	virtual float GetClearRate() PURE_VIRTUAL( IMission:ClearRate, return 0.f; );
	
	virtual FString GetMissionName() PURE_VIRTUAL(IMission:GetMissionName, return FString("NONE"); );
	virtual FString GetMissionContent() PURE_VIRTUAL(IMission:GetMissionContent, return FString("NONE"); );
};