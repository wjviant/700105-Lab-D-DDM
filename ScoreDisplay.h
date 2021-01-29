#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <DirectXMath.h>
#include "GL\GLM\GLM.hpp"
#include "GL\GLM\GTC\matrix_transform.hpp"
#include "GL\GLM\GTC\type_ptr.hpp"
#include "Mesh.h"
#include "Vector4.h"
#include "Colour.h"
#include "MathsHelper.h"
#include "UniformBuffer.h"

class ScoreDisplay
{
	// Data
private:
	Mesh _numbers[10];

	// Structors
public:
	ScoreDisplay(ID3D11Device* device, ID3D11DeviceContext* context);
	~ScoreDisplay();

	// Functions
public:
	void Draw(int score, glm::mat4 MVM, ID3D11DeviceContext* context, ID3D11Buffer* uniformBuffer);
};

