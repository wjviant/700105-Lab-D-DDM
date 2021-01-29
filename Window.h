#pragma once
/***************************************************/
// DX code based on http://www.directxtutorial.com //
/***************************************************/

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

// Other useful libraries 
#include <stdlib.h>
#include <time.h>
#include <vector>

// GLM
#include "GL\GLM\GLM.hpp"
#include "GL\GLM\GTC\matrix_transform.hpp"
#include "GL\GLM\GTC\type_ptr.hpp"

// Program classes
#include "MathsHelper.h"
#include "GameObject.h"
#include "ScoreDisplay.h"
#include "Ship.h"
#include "Bullet.h"
#include "Explosion.h"
#include "Asteroid.h"


enum GameState
{
	Playing,
	Paused,
	Dead,
	Resetting
};

// This class represents our OpenGL window
class Window
{
	// Constants and statics
public:
	static const int			NUM_ASTEROIDS = 5;
	static const int			NUM_BULLETS = 5;
	static const int			SCREEN_WIDTH = 512;
	static const int			SCREEN_HEIGHT = 512;

	static Window*				TheWindow;

	// Private member variables
private:

	// DirectX declarations
	IDXGISwapChain*				_swapchain;             // the pointer to the swap chain interface
	ID3D11Device*				_device;                // the pointer to our Direct3D device interface
	ID3D11DeviceContext*		_context;				// the pointer to our Direct3D device context
	ID3D11RenderTargetView*		_backbuffer;			// the pointer to our back buffer
	ID3D11InputLayout*			_layout;				// the pointer to the input layout
	ID3D11VertexShader*			_vertexShader;			// the pointer to the vertex shader
	ID3D11PixelShader*			_pixelShader;			// the pointer to the pixel shader
	ID3D11Buffer*				_uniformBuffer;			// Stores the MVM
	ID3D11Buffer*				_uniformColourBuffer;	// Stores the global colour

	double						_oldTime;

	// Matrices
	glm::mat4					_IM;  // Identity matrix
	glm::mat4					_MVM; // ModelView matrix
	glm::mat4					_MVMT; // ModelView matrix transposed

	ScoreDisplay*				_scoreDisplay;

	Ship*						_ship;
	Explosion*					_explosion;
	std::vector<Bullet*>		_bullets;
	std::vector<Asteroid*>		_asteroids;
	std::vector<GameObject*>	_renderList;

	// Meshes
	Mesh*						_shipMesh;
	Mesh*						_asteroidMesh;
	Mesh*						_bulletMesh;
	Mesh*						_explosionMesh;
	//Mesh*						_numberMeshes[10];

	int							_score;
	float						_scoreCountdown;
	float						_fader;
	float						_faderInc;
	bool						_godMode;
	GameState					_gameState;

	bool						_keyStates[256];
	bool						_running;

	HWND						_hWnd;

	// Functions
public:
	// Constructor
	Window();

	// Destructor
	virtual ~Window();

	// Main window loop
	int Start(HINSTANCE hInstance, int nCmdShow);

	// Windows Message loop
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// DirectX functions
	void InitD3D();				// sets up and initializes Direct3D
	void RenderFrame();			// renders a single frame
	void CleanD3D();			// closes Direct3D and releases memory
	void InitShaders();			// loads and prepares the shaders

	// Sets up the game...
	void SetupGame();			

	// Game loop
	void Game(double deltaTime);

	// Draw the score
	void DrawScore();

	// Reset game
	void Reset();

	// Keyboard input
	void OnKeyboard(int key, bool down);

};
