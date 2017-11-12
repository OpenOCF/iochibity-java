#ifndef _c_service_provider_h
#define _c_service_provider_h

#include "octypes.h"

OCEntityHandlerResult
c_ServiceProvider_react(OCEntityHandlerFlag watch_flag,
			OCEntityHandlerRequest* c_OCEntityHandlerRequest,
			void* j_ServiceProvider);

#endif
