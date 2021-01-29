#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include "Vertex.h"

class VBO
{
	// Data
private:
	ID3D11Buffer* _vbo;
	int _numVertices;

	// Constructors
public:
	VBO();
	virtual ~VBO();

	// Functions
public:
	void Create(Vertex vertices[], 
				int numVertices, 
				ID3D11Device* device, 
				ID3D11DeviceContext* context);
	void Draw(ID3D11DeviceContext* context);
};

