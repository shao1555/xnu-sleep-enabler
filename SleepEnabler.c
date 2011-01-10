#include <libkern/libkern.h>
#include <mach/mach_types.h>

#include "pmCPU.h"

// Meklort's code added 02-22-10
//  (02-28-10) added some #defs and changed a bit of logic

// shao's code added Jan 11, 2011
//  added some #defs for 10.6.6 release

#define MAJOR_VERSION_10_6				10
#define PM_DISPATCH_VERSION_10_6_0		17
#define PM_DISPATCH_VERSION_10_6_1		17
#define PM_DISPATCH_VERSION_10_6_2		18
#define PM_DISPATCH_VERSION_10_6_3		19
#define PM_DISPATCH_VERSION_10_6_4		20
#define PM_DISPATCH_VERSION_10_6_5		21
#define PM_DISPATCH_VERSION_10_6_6		21
#define PM_DISPATCH_VERSION_10_6_PLUS	version_minor + 16

#include <pexpert/pexpert.h>

extern const int version_major;
extern const int version_minor;
extern const int version_revision;


kern_return_t enable_exitHaltToOff(x86_lcpu_t *lcpu)
{
	return KERN_SUCCESS;
}

kern_return_t SleepEnabler_start (kmod_info_t * ki, void * d) {
	unsigned int dispatchVersion = PM_DISPATCH_VERSION_10_6_PLUS;		// auto inc with kernel version  
	static pmDispatch_t dispatchTable;
	static pmCallBacks_t callbacks;
	int pmInc = 1; // default is enabled
	
	memset(&dispatchTable, 0, sizeof(dispatchTable));
	memset(&callbacks, 0, sizeof(callbacks));
	
	// Determien kernel version, and an appropriate PM_DISPATCH_VERSION
	if (!PE_parse_boot_argn("pmVersion", &dispatchVersion, sizeof (dispatchVersion)))
	{
		if(version_major == MAJOR_VERSION_10_6)	// 10.6
		{
			switch(version_minor)
			{
				case 0:		// 10.0.x kernel (10.6.0 / 10.6.1)
					dispatchVersion = PM_DISPATCH_VERSION_10_6_0;
					break;

				case 2:		// 10.2.x kernel (10.6.2) = 18
					dispatchVersion = PM_DISPATCH_VERSION_10_6_2;
					break;

				case 3:		// 10.3.x kernel (10.6.3) = 19
					dispatchVersion = PM_DISPATCH_VERSION_10_6_3;
					break;

				case 4:		// 10.4.x kernel (10.6.4) = 20
					dispatchVersion = PM_DISPATCH_VERSION_10_6_4;
					break;

				case 5:		// 10.5.x kernel (10.6.5) = 21
					dispatchVersion = PM_DISPATCH_VERSION_10_6_5;
					break;

				case 6:		// 10.6.x kernel (10.6.6) = 21
					dispatchVersion = PM_DISPATCH_VERSION_10_6_6;
					break;
					
				default:
					if (PE_parse_boot_argn("pmIncrement", &pmInc, sizeof (pmInc)))
					{
						if(pmInc == 0) 
						{
							dispatchVersion = PM_DISPATCH_VERSION_10_6_6;	// lask known value
						}
					}
					break;
			}
		}
		
	}
	
	if(dispatchVersion >= 16)
	{
		dispatchTable.exitHaltToOff = &enable_exitHaltToOff;
	}
	else if(dispatchVersion == 0) 
	{
		printf("[SleepEnabler] Disabled due to pmVersion=0\n");
		return KERN_FAILURE;
	}
	else
	{
		printf("[SleepEnabler] Testing version for pre-9.8.0 kernel, without any actual functionality\n");
		
	}
	
	printf("[SleepEnabler] Registering PowerManagement dispatch table...\n");
	printf("[SleepEnabler] To disable (if the kernel panics), please specify the boot argument pmVersion=0\n");
	
	pmKextRegister(dispatchVersion, &dispatchTable, &callbacks);
	
	printf("[SleepEnabler] Calling pmInitComplete()...\n");
	callbacks.initComplete();
	
    return KERN_SUCCESS;
}


kern_return_t SleepEnabler_stop (kmod_info_t * ki, void * d) {
	// unloading the kext would cause the system to crash immediately
    return KERN_FAILURE;
}
