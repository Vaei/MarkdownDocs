// Copyright (c) Jared Taylor

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "MarkdownNoteFactory.generated.h"

/**
 * Factory for creating UMarkdownNote assets
 */
UCLASS(hidecategories=Object)
class UMarkdownNoteFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	UMarkdownNoteFactory();
	
	// Begin UFactory Interface
	virtual UObject* FactoryCreateNew(UClass* Class,UObject* InParent,FName Name,EObjectFlags Flags,UObject* Context,FFeedbackContext* Warn) override;
	// Begin UFactory Interface	
};
