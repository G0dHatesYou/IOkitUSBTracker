#ifndef UserClientInterface_h
#define UserClientInterface_h

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define kSimpleDriverClassName "com_apriorit_driver"

typedef struct MySampleStruct {
    uint64_t field1;
    uint64_t field2;
} MySampleStruct;

enum {
    kMyUserClientOpen,
    kMyUserClientClose,
    kMyScalarIStructOMethod,
    kNumberOfMethods // Must be last
};

kern_return_t MyOpenUserClient(io_connect_t connect);
kern_return_t MyCloseUserClient(io_connect_t connect);
kern_return_t MyScalarIStructureO(io_connect_t connect, const uint32_t scalarI_1, const uint32_t scalarI_2,
                                  size_t* structOSize, MySampleStruct* structO);

#endif /* UserClientInterface_h */
