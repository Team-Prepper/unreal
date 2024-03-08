// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Prepper/Enums/TurningInPlace.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTurningInPlace() {}
// Cross Module References
	PREPPER_API UEnum* Z_Construct_UEnum_Prepper_ETurningInPlace();
	UPackage* Z_Construct_UPackage__Script_Prepper();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_ETurningInPlace;
	static UEnum* ETurningInPlace_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_ETurningInPlace.OuterSingleton)
		{
			Z_Registration_Info_UEnum_ETurningInPlace.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_Prepper_ETurningInPlace, (UObject*)Z_Construct_UPackage__Script_Prepper(), TEXT("ETurningInPlace"));
		}
		return Z_Registration_Info_UEnum_ETurningInPlace.OuterSingleton;
	}
	template<> PREPPER_API UEnum* StaticEnum<ETurningInPlace>()
	{
		return ETurningInPlace_StaticEnum();
	}
	struct Z_Construct_UEnum_Prepper_ETurningInPlace_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_Prepper_ETurningInPlace_Statics::Enumerators[] = {
		{ "ETurningInPlace::ETIP_Left", (int64)ETurningInPlace::ETIP_Left },
		{ "ETurningInPlace::ETIP_Right", (int64)ETurningInPlace::ETIP_Right },
		{ "ETurningInPlace::ETIP_NotTurning", (int64)ETurningInPlace::ETIP_NotTurning },
		{ "ETurningInPlace::ETIP_MAX", (int64)ETurningInPlace::ETIP_MAX },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_Prepper_ETurningInPlace_Statics::Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ETIP_Left.DisplayName", "Turning Left" },
		{ "ETIP_Left.Name", "ETurningInPlace::ETIP_Left" },
		{ "ETIP_MAX.DisplayName", "Default Max" },
		{ "ETIP_MAX.Name", "ETurningInPlace::ETIP_MAX" },
		{ "ETIP_NotTurning.DisplayName", "Not Turning" },
		{ "ETIP_NotTurning.Name", "ETurningInPlace::ETIP_NotTurning" },
		{ "ETIP_Right.DisplayName", "Turning Right" },
		{ "ETIP_Right.Name", "ETurningInPlace::ETIP_Right" },
		{ "ModuleRelativePath", "Enums/TurningInPlace.h" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_Prepper_ETurningInPlace_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_Prepper,
		nullptr,
		"ETurningInPlace",
		"ETurningInPlace",
		Z_Construct_UEnum_Prepper_ETurningInPlace_Statics::Enumerators,
		RF_Public|RF_Transient|RF_MarkAsNative,
		UE_ARRAY_COUNT(Z_Construct_UEnum_Prepper_ETurningInPlace_Statics::Enumerators),
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_Prepper_ETurningInPlace_Statics::Enum_MetaDataParams), Z_Construct_UEnum_Prepper_ETurningInPlace_Statics::Enum_MetaDataParams)
	};
	UEnum* Z_Construct_UEnum_Prepper_ETurningInPlace()
	{
		if (!Z_Registration_Info_UEnum_ETurningInPlace.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_ETurningInPlace.InnerSingleton, Z_Construct_UEnum_Prepper_ETurningInPlace_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_ETurningInPlace.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_Prepper_Source_Prepper_Enums_TurningInPlace_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Prepper_Source_Prepper_Enums_TurningInPlace_h_Statics::EnumInfo[] = {
		{ ETurningInPlace_StaticEnum, TEXT("ETurningInPlace"), &Z_Registration_Info_UEnum_ETurningInPlace, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 3080949757U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Prepper_Source_Prepper_Enums_TurningInPlace_h_3290326042(TEXT("/Script/Prepper"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_Prepper_Source_Prepper_Enums_TurningInPlace_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Prepper_Source_Prepper_Enums_TurningInPlace_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
