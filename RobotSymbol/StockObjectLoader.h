#pragma once
#ifndef STOCK_OBJECT_LOADER
#define STOCK_OBJECT_LOADER

#include "AssetLoader.h"

class StockObjectLoader :
	public AssetLoader
{
public:
	StockObjectLoader();
	~StockObjectLoader();

	void loadAssetFile(const string& filepath) {}

	void loadAssets();
};

#endif

