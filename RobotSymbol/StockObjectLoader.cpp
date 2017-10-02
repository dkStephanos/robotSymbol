#include "StockObjectLoader.h"
#include "OGL3DObject.h"
#include "ObjectGenerator.h"
#include "GameObjectManager.h"
#include "Cuboid.h"
#include "Turret.h"
#include "Axis.h"
#include "Arm.h"
#include "Sword.h"
#include "Sphere.h"
#include "OGL2DTexture.h"
#include "BMPTextureLoader.h"
#include "OGLTexturedFlatSurface.h"
#include "OGL2DTextDisplay.h"
#include "TextureManager.h"

#include <gl\glew.h>
#include <cstdlib>
#include <ctime>

StockObjectLoader::StockObjectLoader()
{
	srand((unsigned int)time(NULL));
}

StockObjectLoader::~StockObjectLoader()
{
}

void StockObjectLoader::loadAssets()
{
	
}
