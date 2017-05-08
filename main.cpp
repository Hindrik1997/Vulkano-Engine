//
// Created by hindrik on 30-1-17.
//

#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h> 
#include "src/Core/Engine.h"

void t() 
{
	Engine e;
	e.run();
}

int main()
{
	t();
	_CrtDumpMemoryLeaks();
}