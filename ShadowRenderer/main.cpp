#include "stdafx.h"
#include "program.h"

#include "objloader.h"

int main()
{
	ObjLoader::Load( "Assets/plane.obj" );

	CFramework framework( 1920, 1080, "Shadow mapping" );
	CProgram program( &framework );

	framework.Run( program );
}