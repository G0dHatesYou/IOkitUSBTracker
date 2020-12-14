/* add your code here */
#include <libkern/c++/OSObject.h>
#include <IOKit/usb/USB.h>
#include <IOKit/IOService.h>
#include <IOKit/IOLib.h>

#define MyIOKitClass com_apriorit_driver

typedef struct MySampleStruct
{
    uint64_t field1;
    uint64_t field2;
} MySampleStruct;

enum
{
    kMyUserClientOpen,
    kMyUserClientClose,
    kMyScalarIStructOMethod,
    kNumberOfMethods // Must be last
};

class MyIOKitClass: public IOService
{
OSDeclareDefaultStructors(com_apriorit_driver)

public:
    IOService *probe(IOService *pProvider, SInt32 *pi32Score) override;
    bool init() override;
    bool start (IOService *provider) override;
    void stop (IOService *provider) override;
    
    bool willTerminate(IOService* provider, IOOptionBits options) override;
    bool didTerminate(IOService* provider, IOOptionBits options, bool* defer) override;
    
    bool terminate(IOOptionBits options = 0) override;
    bool finalize(IOOptionBits options) override;

    virtual IOReturn ScalarIStructO(uint32_t inNumber1, uint32_t inNumber2, MySampleStruct* outStruct, uint32_t* outStructSize);
    
protected:
    void free() override;

private:
    int vendorID = 0;
    int productID = 0;
    void *fSimpleMember;
};
