#include <IOKit/IOKitLib.h>
#include <ApplicationServices/ApplicationServices.h>

#import "UserClientInterface.h"
#import "ViewController.h"

#define kMyPathToSystemLog "/var/log/system.log"

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    [_devicesLabel setStringValue:@"1. -Oops...nothing found"];
}

- (void)setRepresentedObject:(id)representedObject
{
    [super setRepresentedObject:representedObject];
}

kern_return_t MyUserClientOpenExample(io_service_t service, io_connect_t *connect, ViewController* controller)
{
    kern_return_t kernResult = IOServiceOpen(service, mach_task_self(), 0, connect);
    
    if (kernResult != KERN_SUCCESS)
    {
        NSString* data =  [NSString stringWithFormat : @"IOServiceOpen returned 0x%08x\n", kernResult];
    }
    else
    {
        kernResult = MyOpenUserClient(*connect);
            
        if (kernResult == KERN_SUCCESS)
        {
            printf("MyOpenUserClient was successful.\n\n");
        }
        else
        {
            fprintf(stderr, "MyOpenUserClient returned 0x%08x.\n\n", kernResult);
            NSString* data =  [NSString stringWithFormat : @"MyOpenUserClient returned 0x%08x.\n\n", kernResult];
        }
    }
        
    return kernResult;
}

void MyUserClientCloseExample(io_connect_t connect)
{
    kern_return_t kernResult = MyCloseUserClient(connect);
        
    if (kernResult == KERN_SUCCESS)
    {
        printf("MyCloseUserClient was successful.\n\n");
    }
    else
    {
        fprintf(stderr, "MyCloseUserClient returned 0x%08x.\n\n", kernResult);
    }
    
    kernResult = IOServiceClose(connect);
    
    if (kernResult == KERN_SUCCESS)
    {
        printf("IOServiceClose was successful.\n\n");
    }
    else
    {
        fprintf(stderr, "IOServiceClose returned 0x%08x\n\n", kernResult);
    }
}

void MyScalarIStructureOExample(io_connect_t connect, ViewController* controller)
{
    MySampleStruct sampleStruct;
    uint32_t sampleNumber1 = 154;    // This number is random.
    uint32_t sampleNumber2 = 863;    // This number is random.
    size_t structSize = sizeof(MySampleStruct);
    
    kern_return_t kernResult =
        MyScalarIStructureO(connect, sampleNumber1, sampleNumber2, &structSize, &sampleStruct);
        
    if (kernResult == KERN_SUCCESS)
    {
        printf("MyScalarIStructureO was successful.\n");
        printf("field1 = %lld, field2 = %lld, structSize = %lu\n\n", sampleStruct.field1, sampleStruct.field2, structSize);
        NSString* data =  [NSString stringWithFormat : @"USBDevice with VendorID : %llu, and ProductID : %llu\n", sampleStruct.field1, sampleStruct.field2];
        [[controller devicesLabel] setStringValue:[NSString stringWithFormat:@"%@\n%@", [[controller devicesLabel] stringValue], data]];

    }
    else
    {
        fprintf(stderr, "MyScalarIStructureO returned 0x%08x.\n\n", kernResult);
        NSString* data =  [NSString stringWithFormat : @ "MyScalarIStructureO returned 0x%08x.\n\n", kernResult];
        [[controller devicesLabel] setStringValue:[NSString stringWithFormat:@"%@\n USBDevice Disconnected", [[controller devicesLabel] stringValue]]];
    }
}

void MyLaunchConsoleApp()
{
    CFURLRef pathRef;

    pathRef = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, CFSTR(kMyPathToSystemLog), kCFURLPOSIXPathStyle, false);
    
    if (pathRef)
    {
        LSOpenCFURLRef(pathRef, NULL);
        CFRelease(pathRef);
    }
}

-(void) TestUserClient:(io_service_t) service
{
    kern_return_t kernResult;
    io_connect_t connect;

    kernResult = MyUserClientOpenExample(service, &connect, self);

    if (connect != IO_OBJECT_NULL)
    {
        MyScalarIStructureOExample(connect, self);
        
        MyUserClientCloseExample(connect);
    }
}

- (IBAction)closeConnectionPressed:(id)sender
{
    _connectionClosed = YES;
}

- (IBAction)startPressed:(id)sender
{
    [self runClient];
}

- (void) runClient
{
    kern_return_t kernResult;
    io_service_t service;
    io_iterator_t iterator;
    bool driverFound = false;
    
    MyLaunchConsoleApp();

    kernResult = IOServiceGetMatchingServices(kIOMasterPortDefault, IOServiceMatching(kSimpleDriverClassName), &iterator);
    
    if (kernResult != KERN_SUCCESS)
    {
        fprintf(stderr, "IOServiceGetMatchingServices returned 0x%08x\n\n", kernResult);
        return;
    }
        
    while ((service = IOIteratorNext(iterator)) != IO_OBJECT_NULL)
    {
        driverFound = true;
        printf("Found a device of class "kSimpleDriverClassName".\n\n");
        [self TestUserClient: service];
    }
    
    IOObjectRelease(iterator);
    
    if (driverFound == false)
    {
        fprintf(stderr, "No matching drivers found.\n");
    }
}

@end
