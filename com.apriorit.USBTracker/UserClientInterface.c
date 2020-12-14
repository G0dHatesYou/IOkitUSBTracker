#include <AvailabilityMacros.h>
#include <IOKit/IOKitLib.h>
#include "UserClientInterface.h"

kern_return_t MyOpenUserClient(io_connect_t connect)
{
    kern_return_t kernResult;
    
    kernResult = IOConnectCallScalarMethod(connect, kMyUserClientOpen, NULL, 0, NULL, NULL);
    
    return kernResult;
}

kern_return_t MyCloseUserClient(io_connect_t connect)
{
    kern_return_t kernResult;
    
    kernResult = IOConnectCallScalarMethod(connect, kMyUserClientClose, NULL, 0, NULL, NULL);
    
    return kernResult;
}

kern_return_t MyScalarIStructureO(io_connect_t connect, const uint32_t scalarI_1, const uint32_t scalarI_2,
                                  size_t* structOSize, MySampleStruct* structO)
{
    kern_return_t kernResult;
    
    uint64_t scalarI_64[2];
    
    scalarI_64[0] = scalarI_1;
    scalarI_64[1] = scalarI_2;
    
    kernResult = IOConnectCallMethod(connect,                        // an io_connect_t returned from IOServiceOpen().
                                     kMyScalarIStructOMethod,        // selector of the function to be called via the user client.
                                     scalarI_64,                    // array of scalar (64-bit) input values.
                                     2,                                // the number of scalar input values.
                                     NULL,                            // a pointer to the struct input parameter.
                                     0,                                // the size of the input structure parameter.
                                     NULL,                            // array of scalar (64-bit) output values.
                                     NULL,                            // pointer to the number of scalar output values.
                                     structO,                        // pointer to the struct output parameter.
                                     structOSize                    // pointer to the size of the output structure parameter.
                                     );
        
    return kernResult;
}
