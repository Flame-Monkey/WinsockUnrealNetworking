// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Common/MyGameModeBase.h"

#ifdef PRACTICEUNREAL_MyGameModeBase_generated_h
#error "MyGameModeBase.generated.h already included, missing '#pragma once' in MyGameModeBase.h"
#endif
#define PRACTICEUNREAL_MyGameModeBase_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class AMyGameMoveBase **********************************************************
PRACTICEUNREAL_API UClass* Z_Construct_UClass_AMyGameMoveBase_NoRegister();

#define FID_WinsockUnrealNetworking_ClientUnreal_Source_PracticeUnreal_Common_MyGameModeBase_h_15_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAMyGameMoveBase(); \
	friend struct Z_Construct_UClass_AMyGameMoveBase_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend PRACTICEUNREAL_API UClass* Z_Construct_UClass_AMyGameMoveBase_NoRegister(); \
public: \
	DECLARE_CLASS2(AMyGameMoveBase, AGameModeBase, COMPILED_IN_FLAGS(0 | CLASS_Transient | CLASS_Config), CASTCLASS_None, TEXT("/Script/PracticeUnreal"), Z_Construct_UClass_AMyGameMoveBase_NoRegister) \
	DECLARE_SERIALIZER(AMyGameMoveBase)


#define FID_WinsockUnrealNetworking_ClientUnreal_Source_PracticeUnreal_Common_MyGameModeBase_h_15_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AMyGameMoveBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	AMyGameMoveBase(AMyGameMoveBase&&) = delete; \
	AMyGameMoveBase(const AMyGameMoveBase&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AMyGameMoveBase); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AMyGameMoveBase); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AMyGameMoveBase) \
	NO_API virtual ~AMyGameMoveBase();


#define FID_WinsockUnrealNetworking_ClientUnreal_Source_PracticeUnreal_Common_MyGameModeBase_h_12_PROLOG
#define FID_WinsockUnrealNetworking_ClientUnreal_Source_PracticeUnreal_Common_MyGameModeBase_h_15_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_WinsockUnrealNetworking_ClientUnreal_Source_PracticeUnreal_Common_MyGameModeBase_h_15_INCLASS_NO_PURE_DECLS \
	FID_WinsockUnrealNetworking_ClientUnreal_Source_PracticeUnreal_Common_MyGameModeBase_h_15_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class AMyGameMoveBase;

// ********** End Class AMyGameMoveBase ************************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_WinsockUnrealNetworking_ClientUnreal_Source_PracticeUnreal_Common_MyGameModeBase_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
