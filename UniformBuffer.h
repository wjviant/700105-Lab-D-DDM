#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <DirectXMath.h>

// Uniform buffer struct
typedef struct UniformBuffer
{
	DirectX::XMFLOAT4X4 MVM;
	DirectX::XMFLOAT4 Colour;
} UniformBuffer;
