#include "Sphere.h"
#include "ObjectGenerator.h"
#include "MathUtil.h"

Sphere::Sphere(const string& name, float radius, int slices, int stacks, RGBA surfaceColor) :
	OGL3DObject(name)
{
	this->radius = radius;
	this->slices = slices;
	this->stacks = stacks;
	this->surfaceColor = surfaceColor;
	this->generate();
}

Sphere::~Sphere()
{
}

void Sphere::generate()
{
	ObjectGenerator gen;
	gen.clear();
	//ElementArray arr = 
	//	ObjectGenerator::generateSphere(
	//		this->radius, this->slices, this->stacks, this->surfaceColor);
	size_t pi = 0;
	size_t ci = 0;
	size_t ni = 0;
	Vector3f A, B, C, N;
	float stepY = 180.0f / this->stacks;
	float stepX = 360.0f / this->slices;
	float phi = stepY;
	float rho = radius;
	float theta, sinTheta, cosTheta, cosPhi, sinPhi, thetaR, phiR;
	Triangle T;
	// The top pole is composed of triangles.
	//        A
	//       / \
	//      /   \
	//     B-----C
	// Generate top vertices
	for (theta = 0; theta < 360.0f; theta += stepX) {
		// First vertex is the top pole
		A.v1 = 0;
		A.v2 = radius;
		A.v3 = 0;
		T.idx1 = pi;
		gen.positions[pi++] = {A.v1, A.v2, A.v3, 1};
		gen.colors[ci++] = { 
			surfaceColor.red, 
			surfaceColor.green, 
			surfaceColor.blue, 
			surfaceColor.alpha 
		};
		N = A.getNormal();
		gen.normals[ni++] = { N.v1, N.v2, N.v3 };

		// Second vertex
		thetaR = DegToRad(theta);
		phiR = DegToRad(phi);
		sinTheta = sinf(thetaR);
		sinPhi = sinf(phiR);
		cosPhi = cosf(phiR);
		cosTheta = cosf(thetaR);
		B.v1 = rho * sinTheta * sinPhi;
		B.v2 = rho * cosPhi;
		B.v3 = rho * cosTheta * sinPhi;
		T.idx2 = pi;
		gen.positions[pi++] = { B.v1, B.v2, B.v3, 1 };
		gen.colors[ci++] = {
			surfaceColor.red,
			surfaceColor.green,
			surfaceColor.blue,
			surfaceColor.alpha
		};
		N = B.getNormal();
		gen.normals[ni++] = { N.v1, N.v2, N.v3 };

		// Third vertex
		thetaR = DegToRad(theta + stepX);
		sinTheta = sinf(thetaR);
		cosTheta = cosf(thetaR);
		C.v1 = rho * sinTheta * sinPhi;
		C.v2 = B.v2;
		C.v3 = rho * cosTheta * sinPhi;
		T.idx3 = pi;
		gen.positions[pi++] = { C.v1, C.v2, C.v3, 1 };
		gen.colors[ci++] = {
			surfaceColor.red,
			surfaceColor.green,
			surfaceColor.blue,
			surfaceColor.alpha
		};
		N = B.getNormal();
		gen.normals[ni++] = { N.v1, N.v2, N.v3 };
		gen.triangles.push_back(T);
	}
	Vector3f A2, C2, D;
	float phiUp = phi;
	float phiDown = phi + stepY;
	// Generate the middle
	const int MiddleStackCount = this->stacks - 2; // Subtract top and bottom stack
	for (int stack = 0; stack < MiddleStackCount; stack++) {
		// Generate 4 vertices for each facet
		// A---D
		// |\  |
		// | \ |
		// |  \|
		// B---C
		for (theta = 0; theta < 360.0f; theta += stepX) {
			// A
			thetaR = DegToRad(theta);
			sinTheta = sinf(thetaR);
			cosTheta = cos(thetaR);
			phiR = DegToRad(phiUp);
			sinPhi = sinf(phiR);
			cosPhi = cosf(phiR);
			A.v1 = rho * sinTheta * sinPhi;
			A.v2 = rho * cosPhi;
			A.v3 = rho * cosTheta * sinPhi;
			T.idx1 = pi;
			gen.positions[pi++] = { A.v1, A.v2, A.v3, 1 };
			gen.colors[ci++] = {
				surfaceColor.red,
				surfaceColor.green,
				surfaceColor.blue,
				surfaceColor.alpha
			};
			N = A.getNormal();
			gen.normals[ni++] = { N.v1, N.v2, N.v3 };
			// B
			phiR = DegToRad(phiDown);
			sinPhi = sinf(phiR);
			cosPhi = cosf(phiR);
			B.v1 = rho * sinTheta * sinPhi;
			B.v2 = rho * cosPhi;
			B.v3 = rho * cosTheta * sinPhi;
			T.idx2 = pi;
			gen.positions[pi++] = { B.v1, B.v2, B.v3, 1 };
			gen.colors[ci++] = {
				surfaceColor.red,
				surfaceColor.green,
				surfaceColor.blue,
				surfaceColor.alpha
			};
			N = B.getNormal();
			gen.normals[ni++] = { N.v1, N.v2, N.v3 };
			// C
			thetaR = DegToRad(theta + stepX);
			sinTheta = sinf(thetaR);
			cosTheta = cos(thetaR);
			C.v1 = rho * sinTheta * sinPhi;
			C.v2 = B.v2;
			C.v3 = rho * cosTheta * sinPhi;
			T.idx3 = pi;
			gen.positions[pi++] = { C.v1, C.v2, C.v3, 1 };
			gen.colors[ci++] = {
				surfaceColor.red,
				surfaceColor.green,
				surfaceColor.blue,
				surfaceColor.alpha
			};
			N = C.getNormal();
			gen.normals[ni++] = { N.v1, N.v2, N.v3 };
			gen.triangles.push_back(T);

			// A2
			A2.v1 = A.v1;
			A2.v2 = A.v2;
			A2.v3 = A.v3;
			T.idx1 = pi;
			gen.positions[pi++] = { A2.v1, A2.v2, A2.v3, 1 };
			gen.colors[ci++] = {
				surfaceColor.red,
				surfaceColor.green,
				surfaceColor.blue,
				surfaceColor.alpha
			};
			N = A.getNormal();
			gen.normals[ni++] = { N.v1, N.v2, N.v3 };
			// C2
			C2.v1 = C.v1;
			C2.v2 = C.v2;
			C2.v3 = C.v3;
			T.idx2 = pi;
			gen.positions[pi++] = { C2.v1, C2.v2, C2.v3, 1 };
			gen.colors[ci++] = {
				surfaceColor.red,
				surfaceColor.green,
				surfaceColor.blue,
				surfaceColor.alpha
			};
			N = C2.getNormal();
			gen.normals[ni++] = { N.v1, N.v2, N.v3 };
			// D
			phiR = DegToRad(phiUp);
			sinPhi = sinf(phiR);
			D.v1 = rho * sinTheta * sinPhi;
			D.v2 = A.v2;
			D.v3 = rho * cosTheta * sinPhi;
			T.idx3 = pi;
			gen.positions[pi++] = { D.v1, D.v2, D.v3, 1 };
			gen.colors[ci++] = {
				surfaceColor.red,
				surfaceColor.green,
				surfaceColor.blue,
				surfaceColor.alpha
			};
			N = D.getNormal();
			gen.normals[ni++] = { N.v1, N.v2, N.v3 };
			gen.triangles.push_back(T);
		}
		phiUp = phiDown;
		phiDown = phiUp + stepY;
	}
	// The bottom pole
	//  A-----C
	//   \   /
	//    \ /
	//     B
	phi = phiUp;
	for (theta = 0; theta < 360.0f; theta += stepX) {
		thetaR = DegToRad(theta);
		sinTheta = sinf(thetaR);
		cosTheta = cosf(thetaR);
		phiR = DegToRad(phi);
		sinPhi = sinf(phiR);
		cosPhi = cosf(phiR);
		A.v1 = rho * sinTheta * sinPhi;
		A.v2 = rho * cosPhi;
		A.v3 = rho * cosTheta * sinPhi;
		T.idx1 = pi;
		gen.positions[pi++] = { A.v1, A.v2, A.v3, 1 };
		gen.colors[ci++] = {
			surfaceColor.red,
			surfaceColor.green,
			surfaceColor.blue,
			surfaceColor.alpha
		};
		N = A.getNormal();
		gen.normals[ni++] = { N.v1, N.v2, N.v3 };

		B.v1 = 0;
		B.v2 = radius;
		B.v3 = 0;
		T.idx2 = pi;
		gen.positions[pi++] = { B.v1, B.v2, B.v3, 1 };
		gen.colors[ci++] = {
			surfaceColor.red,
			surfaceColor.green,
			surfaceColor.blue,
			surfaceColor.alpha
		};
		N = B.getNormal();
		gen.normals[ni++] = { N.v1, N.v2, N.v3 };

		thetaR = DegToRad(theta + stepX);
		sinTheta = sinf(thetaR);
		cosTheta = cos(thetaR);
		C.v1 = rho * sinTheta * sinPhi;
		C.v2 = A.v2;
		C.v3 = rho * cosTheta * sinPhi;
		T.idx3 = pi;
		gen.positions[pi++] = { C.v1, C.v2, C.v3, 1 };
		gen.colors[ci++] = {
			surfaceColor.red,
			surfaceColor.green,
			surfaceColor.blue,
			surfaceColor.alpha
		};
		N = C.getNormal();
		gen.normals[ni++] = { N.v1, N.v2, N.v3 };
		gen.triangles.push_back(T);
	}

	float* vertexData = gen.generateVertexData();
	short* indexData = gen.generateIndexData();
	this->createElementArrayPCN("Triangles", vertexData, indexData, GL_TRIANGLES);
}
