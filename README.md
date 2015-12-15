In order to integrate the handler you must have mdump_helper.h included in your file and 
mdump_helper.cpp must be present. To integrate the handler once that is done, only three 
lines of code are needed in your project to get the handler to work.

debug_info debugger_;                                              // 1
__try {                                                            // 2
  //body of code you wish to test
} __except(debugger_.minidump_setup(GetExceptionInformation())) {} // 3

Once that is complete the handler will create two dumps for every crash, one normal 
mini dump, and one mini dump with full read and write memory.