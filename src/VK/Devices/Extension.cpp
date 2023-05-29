#include "Extension.h"

Extension::Extension(VkExtensionProperties _prop):
	Properties(_prop),
	Name(Properties.extensionName),
	Version(Properties.specVersion)
{
		
}
