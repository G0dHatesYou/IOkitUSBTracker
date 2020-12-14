#import <Cocoa/Cocoa.h>
#include <IOKit/IOKitLib.h>
#include <ApplicationServices/ApplicationServices.h>

@interface ViewController : NSViewController
@property (strong) IBOutlet NSTextField *devicesLabel;
@property (nonatomic) BOOL connectionClosed;

- (IBAction)closeConnectionPressed:(id)sender;
- (IBAction)startPressed:(id)sender;
- (void) runClient;
- (void) TestUserClient:(io_service_t)service;

@end

