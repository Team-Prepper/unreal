#pragma once

#include "GUIFullScreen.h"
#include "../Singleton.h"

class UIManager : public Singleton<UIManager>
{
private:
	TMap<FString, UUserWidget*> UIDataMap;
	TArray<IGUIFullScreen*> FullScreenStack;
	IGUIFullScreen* NowDisplay;
protected:
public:
	UIManager();
	virtual ~UIManager() override;
	IGUIFullScreen* GetNowDisplay() { return NowDisplay;}
	
	void EnrollmentGUI(IGUIFullScreen* NewFullScreen);
	void Pop();
	
	template <typename T>
	T* OpenGUI(const FString& GuiName);


};

