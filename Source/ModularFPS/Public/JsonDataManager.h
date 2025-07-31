// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyData.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "UObject/NoExportTypes.h"
#include "JsonDataManager.generated.h"

/**
 * Handles fetching and parsing JSON data from a web source.
 */
UCLASS()
class MODULARFPS_API UJsonDataManager : public UObject
{
    GENERATED_BODY()

public:
    // Initiates a data fetch from the remote source
    void FetchData();

private:
    // Callback function to process the HTTP response
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};