#include "Layer.h"

Landmark::Vk::Layer::Layer(VkLayerProperties _prop)
:Properties(_prop),
Name(_prop.layerName),
Description(_prop.description),
Version(_prop.specVersion),
ImplementationVersion(_prop.implementationVersion)
{

}
