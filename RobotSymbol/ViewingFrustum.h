#pragma once
#ifndef VIEWING_FRUSTUM
#define VIEWING_FRUSTUM

class ViewingFrustum
{
protected:
	float fieldOfView;
	float aspectRatio;
	float zNear;
	float zFar;
public:
	ViewingFrustum();
	virtual ~ViewingFrustum();

	virtual void set(
		float fieldOfView, 
		float aspectRatio, 
		float zNear, 
		float zFar);

	void setAspectRatio(float aspectRatio) { this->aspectRatio = aspectRatio; }
};

#endif

