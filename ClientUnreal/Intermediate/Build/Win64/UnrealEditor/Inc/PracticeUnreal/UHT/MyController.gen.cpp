// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Input/MyController.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeMyController() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_APlayerController();
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputAction_NoRegister();
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputMappingContext_NoRegister();
PRACTICEUNREAL_API UClass* Z_Construct_UClass_AMyController();
PRACTICEUNREAL_API UClass* Z_Construct_UClass_AMyController_NoRegister();
UPackage* Z_Construct_UPackage__Script_PracticeUnreal();
// ********** End Cross Module References **********************************************************

// ********** Begin Class AMyController ************************************************************
void AMyController::StaticRegisterNativesAMyController()
{
}
FClassRegistrationInfo Z_Registration_Info_UClass_AMyController;
UClass* AMyController::GetPrivateStaticClass()
{
	using TClass = AMyController;
	if (!Z_Registration_Info_UClass_AMyController.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("MyController"),
			Z_Registration_Info_UClass_AMyController.InnerSingleton,
			StaticRegisterNativesAMyController,
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
	return Z_Registration_Info_UClass_AMyController.InnerSingleton;
}
UClass* Z_Construct_UClass_AMyController_NoRegister()
{
	return AMyController::GetPrivateStaticClass();
}
struct Z_Construct_UClass_AMyController_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Collision Rendering Transformation" },
		{ "IncludePath", "Input/MyController.h" },
		{ "ModuleRelativePath", "Input/MyController.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CharacterMapping_MetaData[] = {
		{ "Category", "Input|Input Mapping" },
		{ "ModuleRelativePath", "Input/MyController.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UIMapping_MetaData[] = {
		{ "Category", "Input|Input Mapping" },
		{ "ModuleRelativePath", "Input/MyController.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ChatAction_MetaData[] = {
		{ "Category", "Input|Input Action" },
		{ "ModuleRelativePath", "Input/MyController.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_CharacterMapping;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_UIMapping;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ChatAction;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AMyController>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AMyController_Statics::NewProp_CharacterMapping = { "CharacterMapping", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AMyController, CharacterMapping), Z_Construct_UClass_UInputMappingContext_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CharacterMapping_MetaData), NewProp_CharacterMapping_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AMyController_Statics::NewProp_UIMapping = { "UIMapping", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AMyController, UIMapping), Z_Construct_UClass_UInputMappingContext_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UIMapping_MetaData), NewProp_UIMapping_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AMyController_Statics::NewProp_ChatAction = { "ChatAction", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AMyController, ChatAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ChatAction_MetaData), NewProp_ChatAction_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AMyController_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AMyController_Statics::NewProp_CharacterMapping,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AMyController_Statics::NewProp_UIMapping,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AMyController_Statics::NewProp_ChatAction,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AMyController_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_AMyController_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_APlayerController,
	(UObject* (*)())Z_Construct_UPackage__Script_PracticeUnreal,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AMyController_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AMyController_Statics::ClassParams = {
	&AMyController::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_AMyController_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_AMyController_Statics::PropPointers),
	0,
	0x009003A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AMyController_Statics::Class_MetaDataParams), Z_Construct_UClass_AMyController_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AMyController()
{
	if (!Z_Registration_Info_UClass_AMyController.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AMyController.OuterSingleton, Z_Construct_UClass_AMyController_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AMyController.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR(AMyController);
AMyController::~AMyController() {}
// ********** End Class AMyController **************************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_WinsockUnrealNetworking_ClientUnreal_Source_PracticeUnreal_Input_MyController_h__Script_PracticeUnreal_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AMyController, AMyController::StaticClass, TEXT("AMyController"), &Z_Registration_Info_UClass_AMyController, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AMyController), 315694037U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_WinsockUnrealNetworking_ClientUnreal_Source_PracticeUnreal_Input_MyController_h__Script_PracticeUnreal_51741574(TEXT("/Script/PracticeUnreal"),
	Z_CompiledInDeferFile_FID_WinsockUnrealNetworking_ClientUnreal_Source_PracticeUnreal_Input_MyController_h__Script_PracticeUnreal_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_WinsockUnrealNetworking_ClientUnreal_Source_PracticeUnreal_Input_MyController_h__Script_PracticeUnreal_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
