/* add your code here */
#include "myIOService.hpp"
#import <os/log.h>
#define super IOService
OSDefineMetaClassAndStructors(MyIOKitClass, IOService)

bool checkIsDeviceMassStorage(IOService *pProvider)
{
    
//    OSDictionary *usbDictionary = IOService::serviceMatching("IOUSBMassStorageClass");
//    return pProvider->matchPropertyTable(usbDictionary);
    return true;
}

bool MyIOKitClass::init()
{
    os_log(OS_LOG_DEFAULT,"IOKitTest::init\n");
    if (!IOService::init())
    {
        return false;
    }
    
    return true;
}

void MyIOKitClass::free()
{
    os_log(OS_LOG_DEFAULT,"IOKitTest::free\n");
    IOService::free();
}

IOService *MyIOKitClass::probe(IOService *pProvider, SInt32 *pi32Score)
{
    os_log(OS_LOG_DEFAULT,"IOKitTest::probe\n");

    bool isUsbMassStorage = checkIsDeviceMassStorage(pProvider);
    if(isUsbMassStorage)
    {
        os_log(OS_LOG_DEFAULT,"IOKitTest::probe with usb\n");
        *pi32Score = INT32_MAX;
        return this;
    }
    return nullptr;
}

void MyIOKitClass::stop(IOService *provider)
{
    os_log(OS_LOG_DEFAULT,"IOKitTest::stop\n");
    IOService::stop(provider);
}


bool MyIOKitClass::start(IOService *provider)
{
    os_log(OS_LOG_DEFAULT,"IOKitTest::start\n");
    bool res = IOService::start(provider);
   
    if (res)
    {
        OSNumber* vid = OSDynamicCast(OSNumber, provider->getProperty(kUSBVendorID));
         if (vid != nullptr)
         {
             vendorID = vid->unsigned32BitValue();
         }
        OSNumber* pid = OSDynamicCast(OSNumber, provider->getProperty(kUSBProductID));
         if (pid != nullptr)
         {
             productID = pid->unsigned32BitValue();
         }
        registerService();
     }
    
    
    return res;
}

bool MyIOKitClass::willTerminate(IOService* provider, IOOptionBits options)
{
    return super::willTerminate(provider, options);
}

bool MyIOKitClass::didTerminate(IOService* provider, IOOptionBits options, bool* defer)
{
    return super::didTerminate(provider, options, defer);
}

bool MyIOKitClass::terminate(IOOptionBits options)
{
    bool success;
    
    success = super::terminate(options);
    
    return success;
}

bool MyIOKitClass::finalize(IOOptionBits options)
{
    bool success;
        
    success = super::finalize(options);
    
    return success;
}

IOReturn MyIOKitClass::ScalarIStructO(uint32_t inNumber1, uint32_t inNumber2, MySampleStruct* outStruct, uint32_t* outStructSize)
{
    IOLog("%s[%p]::%s(inNumber1 = %d, inNumber2 = %d)\n", getName(), this, __FUNCTION__, inNumber1, inNumber2);
    
    outStruct->field1 = vendorID;
    outStruct->field2 = productID;
    
    *outStructSize = sizeof(MySampleStruct);
    
    return kIOReturnSuccess;
}
