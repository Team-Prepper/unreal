#pragma once

#include "GUIFullScreen.h"
#include "../USingleton.h"

class UIManager : public USingleton<UIManager>
{
private:
	TMap<FString, TSubclassOf<UUserWidget>> UIDataMap;
	TArray<IGUIFullScreen*> FullScreenStack;
	IGUIFullScreen* NowDisplay;
public:
	UIManager();
	
	IGUIFullScreen* GetNowDisplay() { return NowDisplay;}
	
	void EnrollmentGUI(IGUIFullScreen* NewFullScreen);
	void Pop();
	
	template <typename T>
	T* OpenGUI(APlayerController* Controller, const FString& GuiName);

};

