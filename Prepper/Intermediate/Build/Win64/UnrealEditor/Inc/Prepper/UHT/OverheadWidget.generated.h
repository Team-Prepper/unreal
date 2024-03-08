// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "HUD/OverheadWidget.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class APawn;
#ifdef PREPPER_OverheadWidget_generated_h
#error "OverheadWidget.generated.h already included, missing '#pragma once' in OverheadWidget.h"
#endif
#define PREPPER_OverheadWidget_generated_h

#define FID_Prepper_Source_Prepper_HUD_OverheadWidget_h_10_SPARSE_DATA
#define FID_Prepper_Source_Prepper_HUD_OverheadWidget_h_10_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_Prepper_Source_Prepper_HUD_OverheadWidget_h_10_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_Prepper_Source_Prepper_HUD_OverheadWidget_h_10_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execShowPlayerNetRole);


#define FID_Prepper_Source_Prepper_HUD_OverheadWidget_h_10_ACCESSORS
#define FID_Prepper_Source_Prepper_HUD_OverheadWidget_h_10_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUOverheadWidget(); \
	friend struct Z_Construct_UClass_UOverheadWidget_Statics; \
public: \
	DECLARE_CLASS(UOverheadWidget, UUserWidget, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/Prepper"), NO_API) \
	DECLARE_SERIALIZER(UOverheadWidget)


#define FID_Prepper_Source_Prepper_HUD_OverheadWidget_h_10_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UOverheadWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UOverheadWidget(UOverheadWidget&&); \
	NO_API UOverheadWidget(const UOverheadWidget&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UOverheadWidget); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UOverheadWidget); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UOverheadWidget) \
	NO_API virtual ~UOverheadWidget();


#define FID_Prepper_Source_Prepper_HUD_OverheadWidget_h_7_PROLOG
#define FID_Prepper_Source_Prepper_HUD_OverheadWidget_h_10_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Prepper_Source_Prepper_HUD_OverheadWidget_h_10_SPARSE_DATA \
	FID_Prepper_Source_Prepper_HUD_OverheadWidget_h_10_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_Prepper_Source_Prepper_HUD_OverheadWidget_h_10_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_Prepper_Source_Prepper_HUD_OverheadWidget_h_10_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Prepper_Source_Prepper_HUD_OverheadWidget_h_10_ACCESSORS \
	FID_Prepper_Source_Prepper_HUD_OverheadWidget_h_10_INCLASS_NO_PURE_DECLS \
	FID_Prepper_Source_Prepper_HUD_OverheadWidget_h_10_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> PREPPER_API UClass* StaticClass<class UOverheadWidget>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Prepper_Source_Prepper_HUD_OverheadWidget_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
