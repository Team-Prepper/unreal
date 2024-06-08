#include "UIManager.h"

#include "UIData.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

template <typename T>
T* UIManager::OpenGUI(const FString& GuiName)
{
	T* Retval = CreateWidget<T>(UIDataMap[GuiName]->GetWorld(), UIDataMap[GuiName]);
	Cast<IGUI>(Retval)->Open();
	return Retval;
}

UIManager::UIManager()
{
	NowDisplay = nullptr;

	
	static ConstructorHelpers::FObjectFinder<UDataTable>
	UIDataTable(TEXT("/Game/Data/UITable"));
	
	if (!UIDataTable.Succeeded()) return;
	
	TArray<FUIData*> UIData;
	UIDataTable.Object->GetAllRows(TEXT("GetAllRows"), UIData);
		
	for (int i = 0; i < UIData.Num(); ++i)
	{
		UIDataMap.Add(UIData[i]->UIName, UIData[i]->TargetClass);
	}

}

UIManager::~UIManager()
{
}

void UIManager::EnrollmentGUI(IGUIFullScreen* NewFullScreen)
{
	if (NowDisplay == nullptr)
	{
		NowDisplay = NewFullScreen;
		return;

	}

	NowDisplay->SetOff();
	FullScreenStack.Add(NowDisplay);
	FullScreenStack.Add(NewFullScreen);

	Pop();
}

void UIManager::Pop()
{
	if (FullScreenStack.Num() < 1)
		return;

	NowDisplay = FullScreenStack[FullScreenStack.Num() - 1];
	FullScreenStack.RemoveAt(FullScreenStack.Num() - 1);
	NowDisplay->SetOn();

}
