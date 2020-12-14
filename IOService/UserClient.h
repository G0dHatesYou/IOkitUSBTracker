#ifndef UserClient_h
#define UserClient_h

#define SimpleUserClientClassName com_apriorit_driver_UserClient

#include <IOKit/IOService.h>
#include <IOKit/IOUserClient.h>
#include "myIOService.hpp"

class SimpleUserClientClassName : public IOUserClient
{
    OSDeclareDefaultStructors(com_apriorit_driver_UserClient)
    
protected:
    MyIOKitClass* fProvider;
    task_t fTask;
    bool fCrossEndian;

    static const IOExternalMethodDispatch sMethods[kNumberOfMethods];
      
public:
    // IOUserClient methods
    virtual void stop(IOService* provider) override;
    virtual bool start(IOService* provider) override;
    
    virtual bool initWithTask(task_t owningTask, void* securityToken, UInt32 type, OSDictionary* properties) override;

    virtual IOReturn clientClose(void) override;
    virtual IOReturn clientDied(void) override;

    virtual bool willTerminate(IOService* provider, IOOptionBits options) override;
    virtual bool didTerminate(IOService* provider, IOOptionBits options, bool* defer) override;
    
    virtual bool terminate(IOOptionBits options = 0) override;
    virtual bool finalize(IOOptionBits options) override;
    
protected:


    virtual IOReturn externalMethod(uint32_t selector, IOExternalMethodArguments* arguments,
                                    IOExternalMethodDispatch* dispatch, OSObject* target, void* reference) override;

    // SimpleUserClient methods
    static IOReturn sOpenUserClient(SimpleUserClientClassName* target, void* reference, IOExternalMethodArguments* arguments);
    virtual IOReturn openUserClient(void);

    static IOReturn sCloseUserClient(SimpleUserClientClassName* target, void* reference, IOExternalMethodArguments* arguments);
    virtual IOReturn closeUserClient(void);

    static IOReturn sScalarIStructO(SimpleUserClientClassName* target, void* reference, IOExternalMethodArguments* arguments);
    virtual IOReturn ScalarIStructO(uint32_t inNumber1, uint32_t inNumber2, MySampleStruct* outStruct, uint32_t* outStructSize);
};

#endif /* UserClient_h */
