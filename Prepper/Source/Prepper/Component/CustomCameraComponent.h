// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "Prepper/Character/Component/Combat/AimingEffect.h"
#include "CustomCameraComponent.generated.h"


UCLASS( HideCategories=(Mobility, Rendering, LOD), Blueprintable, ClassGroup=Camera, meta=(BlueprintSpawnableComponent) )
class PREPPER_API UCustomCameraComponent : public UCameraComponent, public IAimingEffect
{
	GENERATED_BODY()
private:
	float GoalFOV;
	float InterpSpeed;
public:	
	// Sets default values for this component's properties
	UCustomCameraComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void InterpFOV(float GoalFOVSet, float InterpSpeedSet) override;
	virtual float GetDefaultFieldOfView() { return FieldOfView; }
	
	virtual void ShowSniperScopeWidget(bool bIsAiming) override
	{
		_ShowSniperScopeWidget(bIsAiming);
	};
	
	UFUNCTION(BlueprintImplementableEvent)
	void _ShowSniperScopeWidget(bool bIsAiming);
};
