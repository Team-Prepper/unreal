// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SunShine.generated.h"

UCLASS()
class PREPPER_API ASunShine : public AActor
{
	GENERATED_BODY()
	
public:	
	ASunShine();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Directional Light reference
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DayNight")
	class UDirectionalLightComponent* DirectionalLightComponent;

	// Length of a full day in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight")
	float DayLength;

private:
	// Function to update light rotation
	void UpdateLightRotation(float DeltaTime);

};
