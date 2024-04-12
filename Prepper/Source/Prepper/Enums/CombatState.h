#pragma once

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),
	ECS_Melee UMETA(DisplayName = "Melee"),

	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};