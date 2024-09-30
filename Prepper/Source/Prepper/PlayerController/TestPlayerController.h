// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/IHttpRequest.h"
#include "TestPlayerController.generated.h"


// Reference: https://drqwrites.medium.com/accessing-the-openai-api-using-c-a3e527b6584b
class FHttpModule;
/**
 * 
 */
UCLASS()
class PREPPER_API ATestPlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	FHttpModule* Http;
protected:
	virtual void BeginPlay() override;
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void SendOpenAICompletionRequest(const FString& Prompt, const FString& Model = "gpt-3.5-turbo");
};
