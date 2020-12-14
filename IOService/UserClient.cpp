#include <IOKit/IOLib.h>
#include <IOKit/IOKitKeys.h>
#include <libkern/OSByteOrder.h>
#include "UserClient.h"

#define super IOUserClient

OSDefineMetaClassAndStructors(com_apriorit_driver_UserClient, IOUserClient)

const IOExternalMethodDispatch SimpleUserClientClassName::sMethods[kNumberOfMethods] = {
    {   // kMyUserClientOpen
        (IOExternalMethodAction) &SimpleUserClientClassName::sOpenUserClient,    // Method pointer.
        0,                                                                        // No scalar input values.
        0,                                                                        // No struct input value.
        0,                                                                        // No scalar output values.
        0                                                                        // No struct output value.
    },
    {   // kMyUserClientClose
        (IOExternalMethodAction) &SimpleUserClientClassName::sCloseUserClient,    // Method pointer.
        0,                                                                        // No scalar input values.
        0,                                                                        // No struct input value.
        0,                                                                        // No scalar output values.
        0                                                                        // No struct output value.
    },
    {   // kMyScalarIStructOMethod
        (IOExternalMethodAction) &SimpleUserClientClassName::sScalarIStructO,    // Method pointer.
        2,                                                                        // Two scalar input values.
        0,                                                                        // No struct input value.
        0,                                                                        // No scalar output values.
        sizeof(MySampleStruct)                                                    // The size of the output struct.
    }
    
};

IOReturn SimpleUserClientClassName::externalMethod(uint32_t selector, IOExternalMethodArguments* arguments,
                                                   IOExternalMethodDispatch* dispatch, OSObject* target, void* reference)

{
    IOLog("%s[%p]::%s(%d, %p, %p, %p, %p)\n", getName(), this, __FUNCTION__,
          selector, arguments, dispatch, target, reference);
        
    if (selector < (uint32_t) kNumberOfMethods)
    {
        dispatch = (IOExternalMethodDispatch *) &sMethods[selector];
        
        if (!target)
        {
            if (selector == kMyScalarIStructOMethod)
            {
                target = this;
            }
            else
            {
                target = this;
            }
        }
    }
        
    return super::externalMethod(selector, arguments, dispatch, target, reference);
}

bool SimpleUserClientClassName::initWithTask(task_t owningTask, void* securityToken, UInt32 type, OSDictionary* properties)
{
    bool success;
    
    success = super::initWithTask(owningTask, securityToken, type, properties);
    
    if (success)
    {
        
        fCrossEndian = false;
        
        if (properties != NULL && properties->getObject(kIOUserClientCrossEndianKey))
        {
            if (setProperty(kIOUserClientCrossEndianCompatibleKey, kOSBooleanTrue))
            {
                fCrossEndian = true;
                IOLog("%s[%p]::%s(): fCrossEndian = true\n", getName(), this, __FUNCTION__);
            }
        }
    }
    
    fTask = owningTask;
    fProvider = NULL;
        
    return success;
}

bool SimpleUserClientClassName::start(IOService* provider)
{
    bool success;
    
    IOLog("%s[%p]::%s(%p)\n", getName(), this, __FUNCTION__, provider);

    fProvider = OSDynamicCast(MyIOKitClass, provider);
    success = (fProvider != NULL);
    
    if (success)
    {
        success = super::start(provider);
    }
    
    return success;
}

void SimpleUserClientClassName::stop(IOService* provider)
{
    IOLog("%s[%p]::%s(%p)\n", getName(), this, __FUNCTION__, provider);
    
    super::stop(provider);
}

IOReturn SimpleUserClientClassName::clientClose(void)
{
    IOLog("%s[%p]::%s()\n", getName(), this, __FUNCTION__);
    
    (void) closeUserClient();
    
    bool success = terminate();
    if (!success)
    {
        IOLog("%s[%p]::%s(): terminate() failed.\n", getName(), this, __FUNCTION__);
    }
    
    return kIOReturnSuccess;
}

IOReturn SimpleUserClientClassName::clientDied(void)
{
    IOReturn result = kIOReturnSuccess;

    IOLog("%s[%p]::%s()\n", getName(), this, __FUNCTION__);

    result = super::clientDied();

    return result;
}

bool SimpleUserClientClassName::willTerminate(IOService* provider, IOOptionBits options)
{
    IOLog("%s[%p]::%s(%p, %u)\n", getName(), this, __FUNCTION__, provider, (unsigned)options);
    
    return super::willTerminate(provider, options);
}

bool SimpleUserClientClassName::didTerminate(IOService* provider, IOOptionBits options, bool* defer)
{
    IOLog("%s[%p]::%s(%p, %u, %p)\n", getName(), this, __FUNCTION__, provider, (unsigned)options, defer);

    closeUserClient();
    *defer = false;
    
    return super::didTerminate(provider, options, defer);
}

bool SimpleUserClientClassName::terminate(IOOptionBits options)
{
    bool success;
    
    IOLog("%s[%p]::%s(%u)\n", getName(), this, __FUNCTION__, (unsigned)options);

    success = super::terminate(options);
    
    return success;
}

bool SimpleUserClientClassName::finalize(IOOptionBits options)
{
    bool success;
    
    IOLog("%s[%p]::%s(%u)\n", getName(), this, __FUNCTION__, (unsigned)options);
    
    success = super::finalize(options);
    
    return success;
}

IOReturn SimpleUserClientClassName::sOpenUserClient(SimpleUserClientClassName* target, void* reference, IOExternalMethodArguments* arguments)
{
    return target->openUserClient();
}

IOReturn SimpleUserClientClassName::openUserClient(void)
{
    IOReturn result = kIOReturnSuccess;
    
    IOLog("%s[%p]::%s()\n", getName(), this, __FUNCTION__);
    
    if (fProvider == NULL || isInactive())
    {
        result = kIOReturnNotAttached;
    }
    else if (!fProvider->open(this))
    {
        result = kIOReturnExclusiveAccess;
    }
        
    return result;
}

IOReturn SimpleUserClientClassName::sCloseUserClient(SimpleUserClientClassName* target, void* reference, IOExternalMethodArguments* arguments)
{
    return target->closeUserClient();
}

IOReturn SimpleUserClientClassName::closeUserClient(void)
{
    IOReturn result = kIOReturnSuccess;
    
    IOLog("%s[%p]::%s()\n", getName(), this, __FUNCTION__);
            
    if (fProvider == NULL)
    {
        result = kIOReturnNotAttached;
        IOLog("%s[%p]::%s(): returning kIOReturnNotAttached.\n", getName(), this, __FUNCTION__);
    }
    else if (fProvider->isOpen(this))
    {
        fProvider->close(this);
    }
    else
    {
        result = kIOReturnNotOpen;
        IOLog("%s[%p]::%s(): returning kIOReturnNotOpen.\n", getName(), this, __FUNCTION__);
    }
    
    return result;
}

IOReturn SimpleUserClientClassName::sScalarIStructO(SimpleUserClientClassName* target, void* reference, IOExternalMethodArguments* arguments)
{
    return target->ScalarIStructO((uint32_t) arguments->scalarInput[0],
                                  (uint32_t) arguments->scalarInput[1],
                                  (MySampleStruct*) arguments->structureOutput,
                                  (uint32_t*) &arguments->structureOutputSize);
}

IOReturn SimpleUserClientClassName::ScalarIStructO(uint32_t inNumber1, uint32_t inNumber2,
                                                   MySampleStruct* outStruct, uint32_t* outStructSize)
{
    IOReturn result;

    if (fProvider == NULL || isInactive())
    {
        result = kIOReturnNotAttached;
    }
    else if (!fProvider->isOpen(this))
    {
        result = kIOReturnNotOpen;
    }
    else
    {
        result = fProvider->ScalarIStructO(inNumber1, inNumber2, outStruct, outStructSize);

        if (fCrossEndian)
        {
            outStruct->field1 = OSSwapInt64(outStruct->field1);
            outStruct->field2 = OSSwapInt64(outStruct->field2);
        }
    }

    return result;
}
