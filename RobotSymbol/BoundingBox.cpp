#include "BoundingBox.h"

BoundingBox::BoundingBox()
{
	this->use = false;
	this->set(0, 0, 0);
}

BoundingBox::~BoundingBox(void)
{
}

void BoundingBox::set(float width, float depth, float height)
{
	this->width = width;
	this->depth = depth;
	this->height = height;

	float halfWidth = this->width / 2;
	float halfDepth = this->depth / 2;
	float halfHeight = this->height / 2;

	this->A = glm::vec3(-halfWidth,  halfHeight, halfDepth);
	this->B = glm::vec3(-halfWidth, -halfHeight, halfDepth);
	this->C = glm::vec3( halfWidth, -halfHeight, halfDepth);
	this->D = glm::vec3( halfWidth,  halfHeight, halfDepth);

	this->E = glm::vec3( halfWidth,  halfHeight, -halfDepth);
	this->F = glm::vec3( halfWidth, -halfHeight, -halfDepth);
	this->G = glm::vec3(-halfWidth, -halfHeight, -halfDepth);
	this->H = glm::vec3(-halfWidth,  halfHeight, -halfDepth);
}

void BoundingBox::applyTransform()
{
	this->set(this->width, this->depth, this->height);
	this->A = glm::vec3(this->referenceFrame.orientation * glm::vec4(this->A, 1));
	this->B = glm::vec3(this->referenceFrame.orientation * glm::vec4(this->B, 1));
	this->C = glm::vec3(this->referenceFrame.orientation * glm::vec4(this->C, 1));
	this->D = glm::vec3(this->referenceFrame.orientation * glm::vec4(this->D, 1));
	this->E = glm::vec3(this->referenceFrame.orientation * glm::vec4(this->E, 1));
	this->F = glm::vec3(this->referenceFrame.orientation * glm::vec4(this->F, 1));
	this->G = glm::vec3(this->referenceFrame.orientation * glm::vec4(this->G, 1));
	this->H = glm::vec3(this->referenceFrame.orientation * glm::vec4(this->H, 1));
}

glm::vec3 BoundingBox::getCenter() const
{
	glm::vec3 center;
	center.x = (this->A.x + this->F.x) / 2;
	center.y = (this->A.y + this->F.y) / 2;
	center.z = (this->A.z + this->F.z) / 2;
	return center;
}

glm::vec3 BoundingBox::getFrontNormal() const
{
	glm::vec3 BC = this->C - this->B;
	glm::vec3 BA = this->A - this->B;
	return glm::normalize(glm::cross(BC, BA));
}

glm::vec3 BoundingBox::getUpNormal() const
{
	glm::vec3 AD = this->D - this->A;
	glm::vec3 AH = this->H - this->A;
	return glm::normalize(glm::cross(AD, AH));
}

glm::vec3 BoundingBox::getRightNormal() const
{
	glm::vec3 CF = this->F - this->C;
	glm::vec3 CD = this->D - this->C;
	return glm::normalize(glm::cross(CF, CD));
}

float BoundingBox::getMinProjection(const glm::vec3& pAxis) const
{
	float min1 = 0, min2 = 0;
	min1 = getProjection(this->A, pAxis);
	min2 = getProjection(this->B, pAxis);
	if (min2 < min1) min1 = min2;

	min2 = getProjection(this->C, pAxis);
	if (min2 < min1) min1 = min2;

	min2 = getProjection(this->D, pAxis);
	if (min2 < min1) min1 = min2;

	min2 = getProjection(this->E, pAxis);
	if (min2 < min1) min1 = min2;

	min2 = getProjection(this->F, pAxis);
	if (min2 < min1) min1 = min2;

	min2 = getProjection(this->G, pAxis);
	if (min2 < min1) min1 = min2;

	min2 = getProjection(this->H, pAxis);
	if (min2 < min1) min1 = min2;

	return min1;
}

float BoundingBox::getMaxProjection(const glm::vec3& pAxis) const
{
	float max1 = 0, max2 = 0;
	max1 = getProjection(this->A, pAxis);
	max2 = getProjection(this->B, pAxis);
	if (max2 > max1) max1 = max2;

	max2 = getProjection(this->C, pAxis);
	if (max2 > max1) max1 = max2;

	max2 = getProjection(this->D, pAxis);
	if (max2 > max1) max1 = max2;

	max2 = getProjection(this->E, pAxis);
	if (max2 > max1) max1 = max2;

	max2 = getProjection(this->F, pAxis);
	if (max2 > max1) max1 = max2;

	max2 = getProjection(this->G, pAxis);
	if (max2 > max1) max1 = max2;

	max2 = getProjection(this->H, pAxis);
	if (max2 > max1) max1 = max2;

	return max1;
}

float BoundingBox::getProjection(const glm::vec3& point, const glm::vec3& pAxis) const
{
	glm::vec3 pUnit = glm::normalize(pAxis);
	return glm::dot(point, pAxis);
}

bool BoundingBox::overlapsWith(const BoundingBox& other)
{
	if (!this->use || !other.use) return false;

	glm::vec3 yAxis(0, 1, 0);
	glm::vec3 xAxis(1, 0, 0);
	glm::vec3 zAxis(0, 0, 1);

	float minY = this->getMinProjection(yAxis);
	float maxY = this->getMaxProjection(yAxis);
	float oMinY = other.getMinProjection(yAxis);
	float oMaxY = other.getMaxProjection(yAxis);
	if (minY > oMaxY) return false;
	if (oMinY > maxY) return false;

	float minX = this->getMinProjection(xAxis);
	float maxX = this->getMaxProjection(xAxis);
	float oMinX = other.getMinProjection(xAxis);
	float oMaxX = other.getMaxProjection(xAxis);
	if (minX > oMaxX) return false;
	if (oMinX > maxX) return false;

	float minZ = this->getMinProjection(zAxis);
	float maxZ = this->getMaxProjection(zAxis);
	float oMinZ = other.getMinProjection(zAxis);
	float oMaxZ = other.getMaxProjection(zAxis);
	if (minZ > oMaxZ) return false;
	if (oMinZ > maxZ) return false;

	return true;

}


