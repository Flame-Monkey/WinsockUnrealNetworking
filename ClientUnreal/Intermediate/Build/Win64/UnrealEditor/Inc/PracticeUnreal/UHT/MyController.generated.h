// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Input/MyController.h"

#ifdef PRACTICEUNREAL_MyController_generated_h
#error "MyController.generated.h already included, missing '#pragma once' in MyController.h"
#endif
#define PRACTICEUNREAL_MyController_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class AMyController ************************************************************
PRACTICEUNREAL_API UClass* Z_Construct_UClass_AMyController_NoRegister();

#define FID_WinsockUnrealNetworking_ClientUnreal_Source_PracticeUnreal_Input_MyController_h_23_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAMyController(); \
	friend struct Z_Construct_UClass_AMyController_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend PRACTICEUNREAL_API UClass* Z_Construct_UClass_AMyController_NoRegister(); \
public: \
	DECLARE_CLASS2(AMyController, APlayerController, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/PracticeUnreal"), Z_Construct_UClass_AMyController_NoRegister) \
	DECLARE_SERIALIZER(AMyController)


#define FID_WinsockUnrealNetworking_ClientUnreal_Source_PracticeUnreal_Input_MyController_h_23_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	AMyController(AMyController&&) = delete; \
	AMyController(const AMyController&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AMyController); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AMyController); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AMyController) \
	NO_API virtual ~AMyController();


#define FID_WinsockUnrealNetworking_ClientUnreal_Source_PracticeUnreal_Input_MyController_h_20_PROLOG
#define FID_WinsockUnrealNetworking_ClientUnreal_Source_PracticeUnreal_Input_MyController_h_23_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_WinsockUnrealNetworking_ClientUnreal_Source_PracticeUnreal_Input_MyController_h_23_INCLASS_NO_PURE_DECLS \
	FID_WinsockUnrealNetworking_ClientUnreal_Source_PracticeUnreal_Input_MyController_h_23_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class AMyController;

// ********** End Class AMyController **************************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_WinsockUnrealNetworking_ClientUnreal_Source_PracticeUnreal_Input_MyController_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
