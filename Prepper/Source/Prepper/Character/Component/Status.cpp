#include "Status.h"

Status::Status(const FGaugeFloat& Hung, const FGaugeFloat& Thir, const FGaugeFloat& Infe)
{
	Hungry = Hung;
	Thirsty = Thir;
	Infected = Infe;
}