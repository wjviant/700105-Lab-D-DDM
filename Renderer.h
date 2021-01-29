#pragma once

// include Windows and Direct3D headers
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <DirectXMath.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

// GLM
#include "GL\GLM\GLM.hpp"
#include "GL\GLM\GTC\matrix_transform.hpp"
#include "GL\GLM\GTC\type_ptr.hpp"

#include "Colour.h"
#include "UniformBuffer.h"

// Forward declaration
class GameObject;
class Mesh;


// Main Renderer class - abstracts the DirectX interface away from the game code
class Renderer
{
	// Data
protected:
	IDXGISwapChain*			_swapchain;             // the pointer to the swap chain interface
	ID3D11Device*			_device;                // the pointer to our Direct3D device interface
	ID3D11DeviceContext*	_context;				// the pointer to our Direct3D device context
	ID3D11RenderTargetView* _backbuffer;			// the pointer to our back buffer
	ID3D11InputLayout*		_layout;				// the pointer to the input layout
	ID3D11VertexShader*		_vertexShader;			// the pointer to the vertex shader
	ID3D11PixelShader*		_pixelShader;			// the pointer to the pixel shader
	ID3D11Buffer*			_uniformBuffer;			// Stores the MVM and colour

	HWND					_hWnd;					// Window handle

	Colour					_clearColour;			// Screen clear colour

	// Structors
public:
	Renderer(HWND hWnd);
	virtual ~Renderer();


	// Gets/Sets
public:
	ID3D11Device* GetDevice()					const	{ return _device; }
	void SetDevice(ID3D11Device* v)						{ _device = v; }

	ID3D11DeviceContext* GetContext()			const	{ return _context; }
	void SetContext(ID3D11DeviceContext* v)				{ _context = v; }

	ID3D11Buffer* GetUniformBuffer()			const	{ return _uniformBuffer; }
	void SetUniformBuffer(ID3D11Buffer* v)				{ _uniformBuffer = v; }

	Colour GetClearColour()						const	{ return _clearColour; }
	void SetClearColour(Colour c)						{ _clearColour = c; }

	// Functions
public:
	virtual void ClearScreen();
	virtual void Destroy();
	virtual void Initialise(int width, int height);
	virtual void InitialiseShaders();
	virtual void UpdateUniformBuffer(UniformBuffer& buffer);
	virtual void SwapBuffers();

	// TODO: COMPLETE THESE FUNCTIONS IN TASK 2 //
	virtual void Render(GameObject* object, glm::mat4 MVM);
	virtual void Draw(const Mesh* mesh, glm::mat4 MVM, const Colour& colour);
};

