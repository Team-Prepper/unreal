#pragma once

UENUM(BlueprintType)
enum class EStatusEffect : uint8
{
	ESE_FULL UMETA(DisplayName = "Full"),
	ESE_HUNGRY UMETA(DisplayName = "Hungry"),
	ESE_THIRSTY UMETA(DisplayName = "Thirsty"),
	ESE_SICK UMETA(DisplayName = "Sick"),
	ESE_BLEEDING UMETA(DisplayName = "Bleeding"),
	ESE_FRACTURE UMETA(DisplayName = "Fracture"),
	ESE_INFECTED UMETA(DisplayName = "Infected"),

	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};