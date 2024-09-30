
#include "TestPlayerController.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

void ATestPlayerController::BeginPlay()
{
    Super::BeginPlay();

    Http = &FHttpModule::Get();

    SendOpenAICompletionRequest("Nice To Meet You");
    
}

void ATestPlayerController::SendOpenAICompletionRequest(const FString& Prompt, const FString& Model)
{
    const FString ApiKey = ""; // OpenAI API Key 입력
    const FString Url = "https://api.openai.com/v1/chat/completions";

    if (!Http) return;
    // HTTP 요청 생성
    
    const TSharedRef<IHttpRequest> Request = Http->CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");
    Request->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *ApiKey));

    // 요청 본문 데이터 생성
    TSharedPtr<FJsonObject> RequestData = MakeShareable(new FJsonObject);
    RequestData->SetStringField("model", Model);
    
    TArray<TSharedPtr<FJsonValue>> MessagesArray;
    TSharedPtr<FJsonObject> MessageObject = MakeShareable(new FJsonObject);
    MessageObject->SetStringField("role", "user");
    MessageObject->SetStringField("content", Prompt);
    MessagesArray.Add(MakeShareable(new FJsonValueObject(MessageObject)));
    
    RequestData->SetArrayField("messages", MessagesArray);
    RequestData->SetNumberField("temperature", 0);

    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(RequestData.ToSharedRef(), Writer);

    Request->SetContentAsString(RequestBody);
    
    // 응답 콜백 설정
    Request->OnProcessRequestComplete().BindUObject(this, &ATestPlayerController::OnResponseReceived);

    // 요청 전송
    Request->ProcessRequest();
}

// 콜백 함수
void ATestPlayerController::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("HTTP request failed"));
        return;
    }

    FString ResponseString = Response->GetContentAsString();
    
    UE_LOG(LogTemp, Log, TEXT("Completion: %s"), *ResponseString);
    
    return;
    
    // JSON 파싱
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
    
    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        FString Completion = JsonObject->GetArrayField("choices")[0]->AsObject()->GetObjectField("message")->GetStringField("content");
        UE_LOG(LogTemp, Log, TEXT("Completion: %s"), *Completion);
    }
}