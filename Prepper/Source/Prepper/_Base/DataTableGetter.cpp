#include "DataTableGetter.h"

bool DataTableGetter::GetDataTable(const FString& TableName, UDataTable* &RetTable)
{
	FString ContentPath = "/Game/Data/" + TableName;
	
	RetTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(),nullptr,*ContentPath));

	if (RetTable) return true;
	return false;
}
