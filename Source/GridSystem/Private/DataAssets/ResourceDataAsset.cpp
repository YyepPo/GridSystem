#include "DataAssets/ResourceDataAsset.h"

#include "Resource/Resource.h"

void UResourceDataAsset::OnResourceHit(AResource* resource)
{
	if (!resource) { return; }
	
	const int32 randomResource = FMath::RandRange(minResource, maxResource);

	if (resourceType == ETypeResource::ETR_Wood) resource->AddWood(randomResource);
	else if (resourceType == ETypeResource::ETR_Stone) resource->AddStone(randomResource);
}
