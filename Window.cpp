#define _USE_MATH_DEFINES // Allows access to PI from math.h
#include "Window.h"
#include <math.h>

Window* Window::TheWindow = NULL;

/******************************************************************************************************************/

// Constructor - setup window and depth buffer
Window::Window()
{
	_ship = NULL;
	_score = 0;
	_fader = 0;
	_faderInc = 1;
	_godMode = false;
	_uniformBuffer = _uniformColourBuffer = NULL;

	for (int i = 0; i < 256; i++) _keyStates[i] = false;

	// Reset RNG
	srand(time(NULL));

	// Set static 
	TheWindow = this;
}

/******************************************************************************************************************/

// Destructor
Window::~Window()
{
	delete _ship;
	delete _explosion;

	for (int i = 0; i < (int)_bullets.size(); i++)
	{
		delete _bullets[i];
	}

	for (int i = 0; i < (int)_asteroids.size(); i++)
	{
		delete _asteroids[i];
	}
	_asteroids.clear();

	//for (int i = 0; i < 10; i++)
	//{
	//	delete _numbers[i];
	//}
	delete _scoreDisplay;

	// Delete meshes
	delete _shipMesh;
	delete _explosionMesh;
	delete _asteroidMesh;
	delete _bulletMesh;
	/*for (int i = 0; i < 10; i++)
	{
		delete _numberMeshes[i];
	}*/
}


/******************************************************************************************************************/

int Window::Start(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = Window::WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	_hWnd = CreateWindowEx(NULL,
		L"WindowClass",
		L"Asteroids DX",
		WS_OVERLAPPEDWINDOW,
		300,
		300,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(_hWnd, nCmdShow);

	// Initialize Direct3D
	InitD3D();

	// Setup objects
	SetupGame();

	_oldTime = clock();

	MSG msg;
	_running = true;
	while (_running)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}

		double delta_t, tempTime;

		// TODO:
		// Get delta time
		tempTime = clock();
		delta_t = (tempTime - _oldTime) / CLOCKS_PER_SEC;
		_oldTime = tempTime;

		if (_gameState != Paused)
		{
			Game(delta_t);
		}

		RenderFrame();
	}

	// Clean up DirectX
	CleanD3D();

	return msg.wParam;
}

/******************************************************************************************************************/

// This is the main message handler for the program
LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			} 
			break;
		case WM_KEYDOWN:
			{
				TheWindow->OnKeyboard(wParam, true);
			}
			break;
		case WM_KEYUP:
			{
				TheWindow->OnKeyboard(wParam, false);
			}
			break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

/******************************************************************************************************************/

// Initialise Direct3D
void Window::InitD3D()
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                   // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
	scd.BufferDesc.Width = SCREEN_WIDTH;                   // set the back buffer width
	scd.BufferDesc.Height = SCREEN_HEIGHT;                 // set the back buffer height
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
	scd.OutputWindow = _hWnd;                               // the window to be used
	scd.SampleDesc.Count = 4;                              // how many multisamples
	scd.Windowed = TRUE;                                   // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&_swapchain,
		&_device,
		NULL,
		&_context);

	// get the address of the back buffer
	ID3D11Texture2D *p_backbuffer;
	_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&p_backbuffer);

	// use the back buffer address to create the render target
	_device->CreateRenderTargetView(p_backbuffer, NULL, &_backbuffer);
	p_backbuffer->Release();

	// set the render target as the back buffer
	_context->OMSetRenderTargets(1, &_backbuffer, NULL);


	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;

	_context->RSSetViewports(1, &viewport);

	// Initialise shaders
	InitShaders();
}

/******************************************************************************************************************/

// this is the function used to render a single frame
void Window::RenderFrame(void)
{
	// clear the back buffer to a deep blue
	_context->ClearRenderTargetView(_backbuffer, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	// Update MVM
	_MVM = _IM;
	_MVM = glm::translate(_MVM, glm::vec3(0, 0, 0));
	_MVMT = glm::transpose(_MVM); // DirectX uses row-major, not column-major

	// Set global colour
	if (!_godMode) 
	{
		_ship->SetColour(Colour(0.5f, 0.5f, 1.00f, 1.00f));
	}
	else
	{
		_ship->SetColour(Colour(_fader, _fader, 1.00f, 1.00f));
	}

	// Draw things
	for (GameObject* go : _renderList)
	{
		if (go->IsAlive())
		{
			go->Draw(_MVM, _context, _uniformBuffer);
		}
	}

	//if (_ship->IsAlive())
	//{
	//	_ship->Draw(_MVM, _context, _uniformBuffer);
	//}
	//
	//for (int i = 0; i < NUM_BULLETS; i++)
	//{
	//	if (_bullets[i]->IsAlive())
	//	{
	//		_bullets[i]->Draw(_MVM, _context, _uniformBuffer);
	//	}
	//}
	//
	//for (int i = 0; i < (int)_asteroids.size(); i++)
	//{
	//	_asteroids[i]->Draw(_MVM, _context, _uniformBuffer);
	//}
	//
	//if (_explosion->IsAlive())
	//{
	//	_explosion->Draw(_MVM, _context, _uniformBuffer);
	//}
	//
	
	DrawScore();

	// switch the back buffer and the front buffer
	_swapchain->Present(0, 0);
}

/******************************************************************************************************************/

// this is the function that cleans up Direct3D and COM
void Window::CleanD3D(void)
{
	_swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

	// close and release all existing COM objects
	_layout->Release();
	_vertexShader->Release();
	_pixelShader->Release();
	_swapchain->Release();
	_backbuffer->Release();
	_device->Release();
	_context->Release();

	if (_uniformBuffer)			_uniformBuffer->Release();
	if (_uniformColourBuffer)	_uniformColourBuffer->Release();
}

/******************************************************************************************************************/

void Window::SetupGame()
{
	// Create meshes
	_shipMesh = new Mesh();
	_shipMesh->AddVertex({ 0, 0.05f, 0, 1, 1, 1, 1 });
	_shipMesh->AddVertex({ 0.03f, -0.05f, 0, 1, 1, 1, 1 });
	_shipMesh->AddVertex({ -0.03f, -0.05f, 0, 1, 1, 1, 1 });
	_shipMesh->AddVertex({ 0, 0.05f, 0, 1, 1, 1, 1 });
	_shipMesh->CreateVBO(_device, _context);

	_bulletMesh = new Mesh();
	_bulletMesh->AddVertex({ -0.01f, -0.01f, 0, 1, 1, 1, 1 });
	_bulletMesh->AddVertex({ 0.01f, -0.01f, 0, 1, 1, 1, 1 });
	_bulletMesh->AddVertex({ 0.01f, 0.01f, 0, 1, 1, 1, 1 });
	_bulletMesh->AddVertex({ -0.01f, 0.01f, 0, 1, 1, 1, 1 });
	_bulletMesh->AddVertex({ -0.01f, -0.01f, 0, 1, 1, 1, 1 });
	_bulletMesh->CreateVBO(_device, _context);


	_asteroidMesh = new Mesh();
	for (int a = 0; a <= 360; a += 45)
	{
		float x = sinDeg(a) * 0.05f;
		float y = cosDeg(a) * 0.05f;
		_asteroidMesh->AddVertex({ x, y, 0, 0.5f, 0.5f, 0.5f, 1 });
	}
	_asteroidMesh->CreateVBO(_device, _context);

	_explosionMesh = new Mesh();
	_explosionMesh->AddVertex({ 0.0f, 0.25f, 0, 1, 1, 0, 1 });
	_explosionMesh->AddVertex({ 0.05f, 0.10f, 0, 1, 0, 0, 1 });
	_explosionMesh->AddVertex({ 0.20f, 0.20f, 0, 1, 1, 0, 1 });
	_explosionMesh->AddVertex({ 0.10f, 0.05f, 0, 1, 0, 0, 1 });
	_explosionMesh->AddVertex({ 0.25f, 0.0f, 0, 1, 1, 0, 1 });
	_explosionMesh->AddVertex({ 0.10f, -0.05f, 0, 1, 0, 0, 1 });
	_explosionMesh->AddVertex({ 0.20f, -0.20f, 0, 1, 1, 0, 1 });
	_explosionMesh->AddVertex({ 0.05f, -0.10f, 0, 1, 0, 0, 1 });
	_explosionMesh->AddVertex({ 0.0f, -0.25f, 0, 1, 1, 0, 1 });
	_explosionMesh->AddVertex({ -0.05f, -0.10f, 0, 1, 0, 0, 1 });
	_explosionMesh->AddVertex({ -0.20f, -0.20f, 0, 1, 1, 0, 1 });
	_explosionMesh->AddVertex({ -0.10f, -0.05f, 0, 1, 0, 0, 1 });
	_explosionMesh->AddVertex({ -0.25f, -0.0f, 0, 1, 1, 0, 1 });
	_explosionMesh->AddVertex({ -0.10f, 0.05f, 0, 1, 0, 0, 1 });
	_explosionMesh->AddVertex({ -0.20f, 0.20f, 0, 1, 1, 0, 1 });
	_explosionMesh->AddVertex({ -0.05f, 0.10f, 0, 1, 0, 0, 1 });
	_explosionMesh->AddVertex({ 0.0f, 0.25f, 0, 1, 1, 0, 1 });
	_explosionMesh->CreateVBO(_device, _context);


	/*for (int i = 0; i < 10; i++)
	{
		_numberMeshes[i] = new Mesh();
	}

	_numberMeshes[0]->AddVertex({ -0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[0]->AddVertex({ 0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[0]->AddVertex({ 0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[0]->AddVertex({ -0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[0]->AddVertex({ -0.04f, -0.09f, 0, 1, 1, 1, 1 });

	_numberMeshes[1]->AddVertex({ 0.0f, -0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[1]->AddVertex({ 0.0f, 0.09f, 0, 1, 1, 1, 1 });

	_numberMeshes[2]->AddVertex({ 0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[2]->AddVertex({ -0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[2]->AddVertex({ -0.04f, 0.00f, 0, 1, 1, 1, 1 });
	_numberMeshes[2]->AddVertex({ 0.04f, 0.00f, 0, 1, 1, 1, 1 });
	_numberMeshes[2]->AddVertex({ 0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[2]->AddVertex({ -0.04f, 0.09f, 0, 1, 1, 1, 1 });

	_numberMeshes[3]->AddVertex({ -0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[3]->AddVertex({ 0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[3]->AddVertex({ 0.04f, 0.00f, 0, 1, 1, 1, 1 });
	_numberMeshes[3]->AddVertex({ -0.04f, 0.00f, 0, 1, 1, 1, 1 });
	_numberMeshes[3]->AddVertex({ 0.04f, 0.00f, 0, 1, 1, 1, 1 });
	_numberMeshes[3]->AddVertex({ 0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[3]->AddVertex({ -0.04f, 0.09f, 0, 1, 1, 1, 1 });

	_numberMeshes[4]->AddVertex({ -0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[4]->AddVertex({ -0.04f, 0.00f, 0, 1, 1, 1, 1 });
	_numberMeshes[4]->AddVertex({ 0.04f, 0.00f, 0, 1, 1, 1, 1 });
	_numberMeshes[4]->AddVertex({ 0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[4]->AddVertex({ 0.04f, -0.09f, 0, 1, 1, 1, 1 });

	_numberMeshes[5]->AddVertex({ -0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[5]->AddVertex({ 0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[5]->AddVertex({ 0.04f, 0.00f, 0, 1, 1, 1, 1 });
	_numberMeshes[5]->AddVertex({ -0.04f, 0.00f, 0, 1, 1, 1, 1 });
	_numberMeshes[5]->AddVertex({ -0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[5]->AddVertex({ 0.04f, 0.09f, 0, 1, 1, 1, 1 });

	_numberMeshes[6]->AddVertex({ -0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[6]->AddVertex({ -0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[6]->AddVertex({ 0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[6]->AddVertex({ 0.04f, 0.00f, 0, 1, 1, 1, 1 });
	_numberMeshes[6]->AddVertex({ -0.04f, 0.00f, 0, 1, 1, 1, 1 });

	_numberMeshes[7]->AddVertex({ -0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[7]->AddVertex({ 0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[7]->AddVertex({ 0.04f, -0.09f, 0, 1, 1, 1, 1 });

	_numberMeshes[8]->AddVertex({ 0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[8]->AddVertex({ -0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[8]->AddVertex({ -0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[8]->AddVertex({ 0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[8]->AddVertex({ 0.04f, -0.00f, 0, 1, 1, 1, 1 });
	_numberMeshes[8]->AddVertex({ -0.04f, -0.00f, 0, 1, 1, 1, 1 });
	_numberMeshes[8]->AddVertex({ 0.04f, -0.00f, 0, 1, 1, 1, 1 });
	_numberMeshes[8]->AddVertex({ 0.04f, -0.09f, 0, 1, 1, 1, 1 });

	_numberMeshes[9]->AddVertex({ 0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[9]->AddVertex({ 0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[9]->AddVertex({ -0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numberMeshes[9]->AddVertex({ -0.04f, 0.00f, 0, 1, 1, 1, 1 });
	_numberMeshes[9]->AddVertex({ 0.04f, 0.00f, 0, 1, 1, 1, 1 });
	

	for (int i = 0; i < 10; i++)
	{
		_numberMeshes[i]->CreateVBO(_device, _context);
	}*/


	//// Create numbers
	//for (int i = 0; i < 10; i++)
	//{
	//	_numbers[i] = new GameObject((TypeOfGameObject)(Number0 + i), _numberMeshes[i]);
	//	_numbers[i]->SetColour(Colour::Yellow());
	//}

	_scoreDisplay = new ScoreDisplay(_device, _context);


	////////////////
	// Setup Objects
	_ship = new Ship();
	_ship->SetMesh(_shipMesh);
	_ship->SetColour(Colour(0.5f, 0.5f, 1.0f, 1.0f));
	_renderList.push_back(_ship);
	
	// Create bullet
	for (int i = 0; i < NUM_BULLETS; i++)
	{
		Bullet* bullet = new Bullet();
		bullet->SetMesh(_bulletMesh);
		bullet->SetColour(Colour(1.0f, 0.5f, 0.5f, 1.0f));
		_bullets.push_back(bullet);
		_renderList.push_back(bullet);
	}
	
	// Create asteroids
	for (int i = 0; i < NUM_ASTEROIDS; i++)
	{
		Asteroid* asteroid = new Asteroid();
		asteroid->SetMesh(_asteroidMesh);
		asteroid->Reset();
		_asteroids.push_back(asteroid);
		_renderList.push_back(asteroid);
	}
	
	_explosion = new Explosion();
	_explosion->SetMesh(_explosionMesh);
	_renderList.push_back(_explosion);

	_gameState = Playing;
}

/******************************************************************************************************************/

// this function loads and prepares the shaders
void Window::InitShaders()
{
	// load and compile the two shaders
	ID3D10Blob *VS, *PS;
	D3DX11CompileFromFile(L"shaders.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
	D3DX11CompileFromFile(L"shaders.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

	// encapsulate both shaders into shader objects
	_device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &_vertexShader);
	_device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &_pixelShader);

	// set the shader objects
	_context->VSSetShader(_vertexShader, 0, 0);
	_context->PSSetShader(_pixelShader, 0, 0);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	_device->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &_layout);
	_context->IASetInputLayout(_layout);


	// Create uniform buffer
	UniformBuffer uniforms;
	memcpy(&uniforms.MVM, &_IM[0][0], sizeof(DirectX::XMFLOAT4X4));
	uniforms.Colour = { 1.0f, 1.0f, 1.0f, 1.0f };

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(uniforms);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &uniforms;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the buffer.
	_device->CreateBuffer(&cbDesc, &InitData, &_uniformBuffer);
}

/******************************************************************************************************************/

void Window::OnKeyboard(int key, bool down)
{
	_keyStates[key] = down;

	if (down) return; // Ignore key down events

	// Switch key presses
	switch (key)
	{
		case 27: // Esc
			_running = false;
			break;
		case 17: // Ctrl
		case 32: // Space
			// FIRE
			if (_ship->IsAlive() &&
				_gameState == Playing)
			{
				for (int i = 0; i < NUM_BULLETS; i++)
				{
					if (!_bullets[i]->IsAlive())
					{
						_bullets[i]->Reset(_ship->GetPosition(), _ship->GetAngle());
						break;
					}
				}
			}
			break;
		case 71: // G
			_godMode = !_godMode;
			break;
		case 80: // P
			if (_gameState == Playing)
			{ 
				_gameState = Paused;
			}
			else if (_gameState = Paused)
			{
				_gameState = Playing;
			}
			break;
		case 82: // R
			Reset();
			break;
	}
}

/******************************************************************************************************************/

void Window::Game(double deltaTime)
{

	// Update ship
	if (_ship->IsAlive())
	{
		// Move ship
		if (_keyStates[37])	// Left
		{
			_ship->TurnLeft(180.0f * deltaTime);
		}
		if (_keyStates[39])	// Right
		{
			_ship->TurnRight(180.0f * deltaTime);
		}
		if (_keyStates[38])	// Up
		{
			_ship->Accelerate(GameObject::ACCELERATION * deltaTime);
		}

		_ship->Update(deltaTime);
	}

	// Update bullet
	for (int i = 0; i < NUM_BULLETS; i++)
	{
		if (_bullets[i]->IsAlive())
		{
			_bullets[i]->Update(deltaTime);
		}
	}

	// Update asteroids
	for (int i = 0; i < (int)_asteroids.size(); i++)
	{
		_asteroids[i]->Update(deltaTime);

		// Collision detections
		if (!_godMode && 
			 _ship->IsAlive() && 
			 _asteroids[i]->CollideWith(_ship, 0.05f * _asteroids[i]->GetScale()))
		{
			// Boom :(
			_ship->SetAlive(false);
			_explosion->Reset(_ship->GetPosition());
			_gameState = Dead;
		}
		else 
		{
			for (int j = 0; j < NUM_BULLETS; j++)
			{
				if (_bullets[j]->IsAlive())
				{
					if (_asteroids[i]->CollideWith(_bullets[j], 0.05f * _asteroids[i]->GetScale()))
					{
						_score++;

						// Split asteroid and kill bullet
						if (_asteroids[i]->GetScale() < 1)
						{
							// Kill it

							// Have to delete from render list too... :(
							for (int k = 0; k < (int)_renderList.size(); k++)
							{
								if (_asteroids[i] == _renderList[k])
								{
									_renderList.erase(_renderList.begin() + k);
									break;
								}
							}

							// Exterminate!
							delete _asteroids[i];
							_asteroids.erase(_asteroids.begin() + i);
							i--;
							break;
						}
						else
						{
							// Split it
							Asteroid* a = new Asteroid();
							a->SetMesh(_asteroidMesh);
							a->Reset(_asteroids[i]->GetScale() / 2, _asteroids[i]->GetPosition());
							_asteroids.push_back(a);
							_renderList.push_back(a);

							_asteroids[i]->Reset(_asteroids[i]->GetScale() / 2, _asteroids[i]->GetPosition());
						}

						_bullets[j]->SetAlive(false);
					}
				}
			}
		}
	}

	// Add more asteroids if we have less than 5
	while (_asteroids.size() < 5)
	{
		Asteroid* a = new Asteroid();
		a->SetMesh(_asteroidMesh);
		a->Reset();
		_asteroids.push_back(a);
		_renderList.push_back(a);
	}

	// Update explosion
	if (_explosion->IsAlive())
	{
		_explosion->Update(deltaTime);
	}

	_fader += _faderInc * deltaTime;
	if (_fader < 0)
	{
		_faderInc = -_faderInc;
	}
	else if (_fader > 1)
	{
		_faderInc = -_faderInc;
	}

	// Change game state if necessary
	if (_gameState == Dead &&
		!_explosion->IsAlive())
	{
		_gameState = Resetting;
		_scoreCountdown = _score;
	}
	else if (_gameState == Resetting)
	{
		_scoreCountdown -= 60;
		_score = (int)_scoreCountdown;
		if (_score < 0)
		{
			Reset();
		}
	}
}

/******************************************************************************************************************/

void Window::Reset()
{
	_ship->SetPosition(Vector4());
	_ship->SetVelocity(Vector4());

	_renderList.clear();

	for (int i = 0; i < (int)_asteroids.size(); i++)
	{
		_asteroids[i]->Reset();
		if (i >= 5)
		{
			delete _asteroids[i];
			_asteroids.erase(_asteroids.begin() + i);
			i--;
		}
		else
		{
			_renderList.push_back(_asteroids[i]);
		}
	}

	for (int i = 0; i < NUM_BULLETS; i++)
	{
		_bullets[i]->SetAlive(false);
		_renderList.push_back(_bullets[i]);
	}

	_explosion->SetAlive(false);
	_renderList.push_back(_explosion);
	_ship->SetAlive(true);
	_renderList.push_back(_ship);

	_score = 0;

	_gameState = Playing;
}

/******************************************************************************************************************/

void Window::DrawScore()
{
	_MVM = glm::translate(_MVM, glm::vec3(-0.95f, 0.90f, 0));
	_scoreDisplay->Draw(_score, _MVM, _context, _uniformBuffer);
	//int number = 0;
	//float offset = -0.1f;
	//int numberOfDigits = 0;
	//int tscore = _score;
	//while (tscore >= 10)
	//{
	//	tscore /= 10;
	//	numberOfDigits++;
	//}
	//tscore = _score;

	//_MVM = _IM;
	//glm::mat4 tempMVM = glm::transpose(_IM);

	//float x = -0.95f + -offset * numberOfDigits;
	//float y = 0.9f;

	//while (tscore >= 10)
	//{
	//	number = tscore % 10;
	//	_numbers[number]->SetPosition(Vector4(x, y));
	//	_numbers[number]->Draw(tempMVM, _context, _uniformBuffer);
	//	tscore /= 10;
	//	x += offset;
	//}

	//_numbers[tscore]->SetPosition(Vector4(x, y));
	//_numbers[tscore]->Draw(tempMVM, _context, _uniformBuffer);
}

/******************************************************************************************************************/
