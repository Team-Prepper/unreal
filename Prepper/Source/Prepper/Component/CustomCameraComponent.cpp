// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCameraComponent.h"

// Sets default values for this component's properties
UCustomCameraComponent::UCustomCameraComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UCustomCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	GoalFOV = FieldOfView;
	
}

// Called every frame
void UCustomCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	SetFieldOfView(FMath::FInterpTo(FieldOfView, GoalFOV, DeltaTime, InterpSpeed));
}

void UCustomCameraComponent::InterpFOV(float GoalFOVSet, float InterpSpeedSet)
{
	GoalFOV = GoalFOVSet;
	InterpSpeed = InterpSpeedSet;
}

