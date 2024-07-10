#include "InventoryInteractableItem.h"

#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

AInventoryInteractableItem::AInventoryInteractableItem()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetVisibility(false);  // 초기에는 숨김
	
	BoxCollision->OnBeginCursorOver.AddDynamic(this, &AInventoryInteractableItem::OnBeginCursorOver);
	BoxCollision->OnEndCursorOver.AddDynamic(this, &AInventoryInteractableItem::OnEndCursorOver);
	BoxCollision->OnClicked.AddDynamic(this, &AInventoryInteractableItem::OnClicked);
}

void AInventoryInteractableItem::Interaction(APlayerCharacter* Target)
{
	if (WidgetClass)
	{
		
	}
}

void AInventoryInteractableItem::OnBeginCursorOver(UPrimitiveComponent* TouchedComponent)
{
	WidgetComponent->SetVisibility(true);
}

void AInventoryInteractableItem::OnEndCursorOver(UPrimitiveComponent* TouchedComponent)
{
	WidgetComponent->SetVisibility(false);
}

void AInventoryInteractableItem::OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (WidgetComponent && WidgetComponent->GetUserWidgetObject())
	{
		// 위젯에 정의된 버튼 클릭 이벤트를 처리합니다.
		UUserWidget* Widget = WidgetComponent->GetUserWidgetObject();
		// 필요한 경우, 위젯의 특정 함수나 속성을 호출합니다.
	}
}
