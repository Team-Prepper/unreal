#pragma once
#include "Prepper/_Base/Util/GaugeFloat.h"

class Status
{
public:
	Status(const FGaugeFloat& Hungry, const FGaugeFloat& Thirsty, const FGaugeFloat& Infected);
	FGaugeFloat Hungry;
	FGaugeFloat Thirsty;
	FGaugeFloat Infected;
};
