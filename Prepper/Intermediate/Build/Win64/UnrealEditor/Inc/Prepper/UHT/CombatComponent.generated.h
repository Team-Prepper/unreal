// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Component/CombatComponent.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
struct FVector_NetQuantize;
#ifdef PREPPER_CombatComponent_generated_h
#error "CombatComponent.generated.h already included, missing '#pragma once' in CombatComponent.h"
#endif
#define PREPPER_CombatComponent_generated_h

#define FID_Prepper_Source_Prepper_Component_CombatComponent_h_10_SPARSE_DATA
#define FID_Prepper_Source_Prepper_Component_CombatComponent_h_10_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_Prepper_Source_Prepper_Component_CombatComponent_h_10_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_Prepper_Source_Prepper_Component_CombatComponent_h_10_RPC_WRAPPERS_NO_PURE_DECLS \
	virtual void MulticastFire_Implementation(FVector_NetQuantize const& TraceHitTarget); \
	virtual void ServerFire_Implementation(FVector_NetQuantize const& TraceHitTarget); \
	virtual void ServerSetAiming_Implementation(bool bIsAiming); \
 \
	DECLARE_FUNCTION(execMulticastFire); \
	DECLARE_FUNCTION(execServerFire); \
	DECLARE_FUNCTION(execOnRep_EquippedWeapon); \
	DECLARE_FUNCTION(execServerSetAiming);


#define FID_Prepper_Source_Prepper_Component_CombatComponent_h_10_ACCESSORS
#define FID_Prepper_Source_Prepper_Component_CombatComponent_h_10_CALLBACK_WRAPPERS
#define FID_Prepper_Source_Prepper_Component_CombatComponent_h_10_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUCombatComponent(); \
	friend struct Z_Construct_UClass_UCombatComponent_Statics; \
public: \
	DECLARE_CLASS(UCombatComponent, UActorComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/Prepper"), NO_API) \
	DECLARE_SERIALIZER(UCombatComponent) \
	enum class ENetFields_Private : uint16 \
	{ \
		NETFIELD_REP_START=(uint16)((int32)Super::ENetFields_Private::NETFIELD_REP_END + (int32)1), \
		EquippedWeapon=NETFIELD_REP_START, \
		bAiming, \
		NETFIELD_REP_END=bAiming	}; \
	NO_API virtual void ValidateGeneratedRepEnums(const TArray<struct FRepRecord>& ClassReps) const override;


#define FID_Prepper_Source_Prepper_Component_CombatComponent_h_10_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UCombatComponent(UCombatComponent&&); \
	NO_API UCombatComponent(const UCombatComponent&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UCombatComponent); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UCombatComponent); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UCombatComponent) \
	NO_API virtual ~UCombatComponent();


#define FID_Prepper_Source_Prepper_Component_CombatComponent_h_7_PROLOG
#define FID_Prepper_Source_Prepper_Component_CombatComponent_h_10_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Prepper_Source_Prepper_Component_CombatComponent_h_10_SPARSE_DATA \
	FID_Prepper_Source_Prepper_Component_CombatComponent_h_10_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_Prepper_Source_Prepper_Component_CombatComponent_h_10_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_Prepper_Source_Prepper_Component_CombatComponent_h_10_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Prepper_Source_Prepper_Component_CombatComponent_h_10_ACCESSORS \
	FID_Prepper_Source_Prepper_Component_CombatComponent_h_10_CALLBACK_WRAPPERS \
	FID_Prepper_Source_Prepper_Component_CombatComponent_h_10_INCLASS_NO_PURE_DECLS \
	FID_Prepper_Source_Prepper_Component_CombatComponent_h_10_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> PREPPER_API UClass* StaticClass<class UCombatComponent>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Prepper_Source_Prepper_Component_CombatComponent_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
