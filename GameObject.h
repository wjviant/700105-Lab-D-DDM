#pragma once
#include <vector>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <DirectXMath.h>
#include "GL\GLM\GLM.hpp"
#include "GL\GLM\GTC\matrix_transform.hpp"
#include "GL\GLM\GTC\type_ptr.hpp"
#include "Vector4.h"
#include "Colour.h"
#include "Mesh.h"
#include "Vertex.h"
#include "MathsHelper.h"
#include "UniformBuffer.h"

enum TypeOfGameObject
{
	NoneType,
	ShipType,
	AsteroidType,
	BulletType,
	ExplosionType
};

class GameObject
{
	// Constants
public:
	static const float			MAX_SPEED;
	static const float			ACCELERATION;
	static const float			BULLET_SPEED;
	static const float			MAX_LIFE;	// How long the bullet lasts (in seconds)
	static const float			MAX_DRIFT;
	static const float			LIFETIME;	// In seconds
	static const float			MAX_SIZE;	// Max scale of the explosion

	// Data
protected:
	float						_angle;		// Angle of object in degrees
	float						_scale;		// Scale of the object (1 = normal)
	Colour						_colour;	// Colour of object
	Vector4						_position;	// Position of object's origin
	Vector4						_velocity;	// Velocity of object
	bool						_alive;
	float						_life;
	TypeOfGameObject			_type;

	bool						_locked;	// True once we've made the VBO; can no longer add vertices etc unless reset
	Mesh*						_mesh;
	std::vector<Vertex>			_vertices;

	// Constructors
public:
	GameObject(TypeOfGameObject objType = TypeOfGameObject::NoneType, Mesh* mesh = NULL);
	virtual ~GameObject();

	GameObject(const GameObject&);
	GameObject& operator=(const GameObject&);

	// Gets/sets
public:
	float GetAngle()		const	{ return _angle; }
	void SetAngle(float v)			{ _angle = v;  }

	float GetScale()		const	{ return _scale; }
	void SetScale(float v)			{ _scale = v; }

	Colour GetColour()		const	{ return _colour; }
	void SetColour(Colour c)		{ _colour = c; }

	Vector4 GetPosition()	const	{ return _position; }
	void SetPosition(Vector4 v)		{ _position = v; }

	Vector4 GetVelocity()	const	{ return _velocity; }
	void SetVelocity(Vector4 v)		{ _velocity = v; }

	bool IsAlive()			const	{ return _alive; }
	void SetAlive(bool v)			{ _alive = v; }

	Mesh* GetMesh()			const	{ return _mesh; }
	void SetMesh(Mesh* m)			{ _mesh = m; }


	// Functions
public:
	bool CollideWith(const GameObject* other, float collisionRange);
	void Update(double deltaTime);
	void Draw(glm::mat4 MVM, ID3D11DeviceContext* context, ID3D11Buffer* uniformBuffer);

	// Draws the mesh
private:
	void Draw(ID3D11DeviceContext* context)		{ if (_mesh) _mesh->Draw(context); }
};

