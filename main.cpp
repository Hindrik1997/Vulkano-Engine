//
// Created by hindrik on 30-1-17.
//

#ifdef WIN32
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>
#endif
#include "src/Core/Engine.h"

void t() 
{
	Engine e;
	e.run();
}

int main()
{
	t();
    #ifdef WIN32
        _CrtDumpMemoryLeaks();
    #endif
}