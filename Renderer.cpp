#include "Renderer.h"
#include "GameObject.h"

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

Renderer::Renderer(HWND hWnd)
	: _clearColour(0.0f, 0.0f, 0.0f, 1.0f),
	  _hWnd(hWnd)
{
}

/******************************************************************************************************************/

Renderer::~Renderer()
{
}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

void Renderer::ClearScreen()
{
	// TODO:
	// Complete in task 1
}

/******************************************************************************************************************/

void Renderer::Destroy()
{
	// TODO:
	// Complete in task 1
}

/******************************************************************************************************************/

void Renderer::Initialise(int width, int height)
{
	// TODO:
	// Complete in task 1
}

/******************************************************************************************************************/

void Renderer::SwapBuffers()
{
	// TODO:
	// Complete in task 1
}

/******************************************************************************************************************/

// Load and prepare shaders
void Renderer::InitialiseShaders()
{
	// TODO:
	// Complete in task 1
}

/******************************************************************************************************************/

// TODO: COMPLETE THESE FUNCTIONS IN TASK 2 //
void Renderer::Render(GameObject* object, glm::mat4 MVM)
{
	// TODO:
	// Complete in task 2
}

/******************************************************************************************************************/

void Renderer::Draw(const Mesh* mesh, glm::mat4 MVM, const Colour& colour)
{
	// TODO:
	// Complete in task 2
}

/******************************************************************************************************************/

void Renderer::UpdateUniformBuffer(UniformBuffer& buffer)
{
	D3D11_MAPPED_SUBRESOURCE ms;
	_context->Map(_uniformBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);		// map the buffer
	memcpy(ms.pData, &buffer, sizeof(UniformBuffer));								// copy the data
	_context->Unmap(_uniformBuffer, NULL);											// unmap the buffer
	_context->VSSetConstantBuffers(0, 1, &_uniformBuffer);
	_context->PSSetConstantBuffers(0, 1, &_uniformBuffer);
}

/******************************************************************************************************************/
