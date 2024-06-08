#pragma once

class DataTableGetter
{
public:
	static bool GetDataTable(const FString& TableName, UDataTable* &RetTable);
};
