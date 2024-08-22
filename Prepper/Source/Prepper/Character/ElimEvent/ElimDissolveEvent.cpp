// Fill out your copyright notice in the Description page of Project Settings.


#include "ElimDissolveEvent.h"

#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"

void UElimDissolveEvent::StartElim()
{
	if (DissolveMaterialInstance)
	{
		DynamicDissolveMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);

		TargetCharacter->GetMesh()->SetMaterial(0, DynamicDissolveMaterialInstance);

		TArray<USceneComponent*> AttachedComponents;
		TargetCharacter->GetMesh()->GetChildrenComponents(true,  AttachedComponents);

		// Loop through all found Static Mesh components
		for (USceneComponent* SceneComponent : AttachedComponents)
		{
			if(UStaticMeshComponent* SMComp = Cast<UStaticMeshComponent>(SceneComponent))
			{
				SMComp->SetMaterial(0, DynamicDissolveMaterialInstance);
			}
		}
			
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("DissolveValue"), 0.55f);
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("GlowValue"), 200.f);
	}
	
	DissolveTrack.BindDynamic(this, &UElimDissolveEvent::UpdateDissolveMaterial);
	
	if (DissolveCurve && DissolveTimeline)
	{
		DissolveTimeline->AddInterpFloat(DissolveCurve, DissolveTrack);
		DissolveTimeline->Play();
	}
	
}

void UElimDissolveEvent::SetTarget(ACharacter* Target)
{
	TargetCharacter = Target;
	DissolveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DissolveTimelineComponent"));
}

void UElimDissolveEvent::UpdateDissolveMaterial(const float DissolveValue)
{
	if (DynamicDissolveMaterialInstance)
	{
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("DissolveValue"), DissolveValue);
	}
}
