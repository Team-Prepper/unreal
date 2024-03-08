// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Character/PlayerCharacter.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AWeapon;
#ifdef PREPPER_PlayerCharacter_generated_h
#error "PlayerCharacter.generated.h already included, missing '#pragma once' in PlayerCharacter.h"
#endif
#define PREPPER_PlayerCharacter_generated_h

#define FID_Prepper_Source_Prepper_Character_PlayerCharacter_h_15_SPARSE_DATA
#define FID_Prepper_Source_Prepper_Character_PlayerCharacter_h_15_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_Prepper_Source_Prepper_Character_PlayerCharacter_h_15_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_Prepper_Source_Prepper_Character_PlayerCharacter_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	virtual void ServerSprintButtonReleased_Implementation(); \
	virtual void ServerSprintButtonPressed_Implementation(); \
	virtual void ServerEquipButtonPressed_Implementation(); \
 \
	DECLARE_FUNCTION(execServerSprintButtonReleased); \
	DECLARE_FUNCTION(execServerSprintButtonPressed); \
	DECLARE_FUNCTION(execServerEquipButtonPressed); \
	DECLARE_FUNCTION(execOnRep_OverlappingWeapon);


#define FID_Prepper_Source_Prepper_Character_PlayerCharacter_h_15_ACCESSORS
#define FID_Prepper_Source_Prepper_Character_PlayerCharacter_h_15_CALLBACK_WRAPPERS
#define FID_Prepper_Source_Prepper_Character_PlayerCharacter_h_15_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAPlayerCharacter(); \
	friend struct Z_Construct_UClass_APlayerCharacter_Statics; \
public: \
	DECLARE_CLASS(APlayerCharacter, ACharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/Prepper"), NO_API) \
	DECLARE_SERIALIZER(APlayerCharacter) \
	enum class ENetFields_Private : uint16 \
	{ \
		NETFIELD_REP_START=(uint16)((int32)Super::ENetFields_Private::NETFIELD_REP_END + (int32)1), \
		OverlappingWeapon=NETFIELD_REP_START, \
		NETFIELD_REP_END=OverlappingWeapon	}; \
	NO_API virtual void ValidateGeneratedRepEnums(const TArray<struct FRepRecord>& ClassReps) const override;


#define FID_Prepper_Source_Prepper_Character_PlayerCharacter_h_15_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API APlayerCharacter(APlayerCharacter&&); \
	NO_API APlayerCharacter(const APlayerCharacter&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, APlayerCharacter); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(APlayerCharacter); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(APlayerCharacter) \
	NO_API virtual ~APlayerCharacter();


#define FID_Prepper_Source_Prepper_Character_PlayerCharacter_h_12_PROLOG
#define FID_Prepper_Source_Prepper_Character_PlayerCharacter_h_15_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Prepper_Source_Prepper_Character_PlayerCharacter_h_15_SPARSE_DATA \
	FID_Prepper_Source_Prepper_Character_PlayerCharacter_h_15_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_Prepper_Source_Prepper_Character_PlayerCharacter_h_15_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_Prepper_Source_Prepper_Character_PlayerCharacter_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Prepper_Source_Prepper_Character_PlayerCharacter_h_15_ACCESSORS \
	FID_Prepper_Source_Prepper_Character_PlayerCharacter_h_15_CALLBACK_WRAPPERS \
	FID_Prepper_Source_Prepper_Character_PlayerCharacter_h_15_INCLASS_NO_PURE_DECLS \
	FID_Prepper_Source_Prepper_Character_PlayerCharacter_h_15_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> PREPPER_API UClass* StaticClass<class APlayerCharacter>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Prepper_Source_Prepper_Character_PlayerCharacter_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
