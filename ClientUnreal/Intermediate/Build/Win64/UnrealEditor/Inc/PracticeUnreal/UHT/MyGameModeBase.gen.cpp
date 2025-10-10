// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Common/MyGameModeBase.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeMyGameModeBase() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
PRACTICEUNREAL_API UClass* Z_Construct_UClass_AMyGameMoveBase();
PRACTICEUNREAL_API UClass* Z_Construct_UClass_AMyGameMoveBase_NoRegister();
UPackage* Z_Construct_UPackage__Script_PracticeUnreal();
// ********** End Cross Module References **********************************************************

// ********** Begin Class AMyGameMoveBase **********************************************************
void AMyGameMoveBase::StaticRegisterNativesAMyGameMoveBase()
{
}
FClassRegistrationInfo Z_Registration_Info_UClass_AMyGameMoveBase;
UClass* AMyGameMoveBase::GetPrivateStaticClass()
{
	using TClass = AMyGameMoveBase;
	if (!Z_Registration_Info_UClass_AMyGameMoveBase.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("MyGameMoveBase"),
			Z_Registration_Info_UClass_AMyGameMoveBase.InnerSingleton,
			StaticRegisterNativesAMyGameMoveBase,
			sizeof(TClass),
			alignof(TClass),
			TClass::StaticClassFlags,
			TClass::StaticClassCastFlags(),
			TClass::StaticConfigName(),
			(UClass::ClassConstructorType)InternalConstructor<TClass>,
			(UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<TClass>,
			UOBJECT_CPPCLASS_STATICFUNCTIONS_FORCLASS(TClass),
			&TClass::Super::StaticClass,
			&TClass::WithinClass::StaticClass
		);
	}
	return Z_Registration_Info_UClass_AMyGameMoveBase.InnerSingleton;
}
UClass* Z_Construct_UClass_AMyGameMoveBase_NoRegister()
{
	return AMyGameMoveBase::GetPrivateStaticClass();
}
struct Z_Construct_UClass_AMyGameMoveBase_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "Common/MyGameModeBase.h" },
		{ "ModuleRelativePath", "Common/MyGameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AMyGameMoveBase>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_AMyGameMoveBase_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameModeBase,
	(UObject* (*)())Z_Construct_UPackage__Script_PracticeUnreal,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AMyGameMoveBase_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AMyGameMoveBase_Statics::ClassParams = {
	&AMyGameMoveBase::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x009003ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AMyGameMoveBase_Statics::Class_MetaDataParams), Z_Construct_UClass_AMyGameMoveBase_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AMyGameMoveBase()
{
	if (!Z_Registration_Info_UClass_AMyGameMoveBase.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AMyGameMoveBase.OuterSingleton, Z_Construct_UClass_AMyGameMoveBase_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AMyGameMoveBase.OuterSingleton;
}
AMyGameMoveBase::AMyGameMoveBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(AMyGameMoveBase);
AMyGameMoveBase::~AMyGameMoveBase() {}
// ********** End Class AMyGameMoveBase ************************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_WinsockUnrealNetworking_ClientUnreal_Source_PracticeUnreal_Common_MyGameModeBase_h__Script_PracticeUnreal_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AMyGameMoveBase, AMyGameMoveBase::StaticClass, TEXT("AMyGameMoveBase"), &Z_Registration_Info_UClass_AMyGameMoveBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AMyGameMoveBase), 1703711906U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_WinsockUnrealNetworking_ClientUnreal_Source_PracticeUnreal_Common_MyGameModeBase_h__Script_PracticeUnreal_407419125(TEXT("/Script/PracticeUnreal"),
	Z_CompiledInDeferFile_FID_WinsockUnrealNetworking_ClientUnreal_Source_PracticeUnreal_Common_MyGameModeBase_h__Script_PracticeUnreal_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_WinsockUnrealNetworking_ClientUnreal_Source_PracticeUnreal_Common_MyGameModeBase_h__Script_PracticeUnreal_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
