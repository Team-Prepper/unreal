// Fill out your copyright notice in the Description page of Project Settings.


#include "ElimDissolveComponent.h"

#include "AsyncTreeDifferences.h"
#include "GameFramework/Character.h"


// Sets default values
UElimDissolveComponent::UElimDissolveComponent()
{
	DissolveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DissolveTimelineComponent"));
}

void UElimDissolveComponent::StartElim()
{
	// Start Dissolve Effect
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
	
	DissolveTrack.BindDynamic(this, &UElimDissolveComponent::UpdateDissolveMaterial);
	
	if (DissolveCurve && DissolveTimeline)
	{
		DissolveTimeline->AddInterpFloat(DissolveCurve, DissolveTrack);
		DissolveTimeline->Play();
	}
	
}

void UElimDissolveComponent::SetTarget(ACharacter* Target)
{
	TargetCharacter = Target;
}

void UElimDissolveComponent::UpdateDissolveMaterial(const float DissolveValue)
{
	if (DynamicDissolveMaterialInstance)
	{
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("DissolveValue"), DissolveValue);
	}
}
