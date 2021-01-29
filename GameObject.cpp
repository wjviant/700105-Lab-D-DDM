#include "GameObject.h"

const float GameObject::MAX_DRIFT = 0.25f;

const float GameObject::BULLET_SPEED = 1.0f;
const float GameObject::MAX_LIFE = 1.0f;

const float GameObject::LIFETIME = 1.0f;
const float GameObject::MAX_SIZE = 1.0f;

const float GameObject::ACCELERATION = 1.0f;
const float GameObject::MAX_SPEED = 0.5f;

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

GameObject::GameObject(TypeOfGameObject objType, Mesh* mesh)
	:	_angle(0),
		_scale(1),
		_position(0,0),
		_velocity(0,0),
		_colour(1,1,1,1),
		_alive(true),
		_life(0),
		_type(objType),
		_locked(false),
		_mesh(mesh)
{
	
}

/******************************************************************************************************************/

GameObject::~GameObject()
{
}

/******************************************************************************************************************/

GameObject::GameObject(const GameObject& other)
{
	_angle = other.GetAngle();
	_scale = other.GetScale();
	_position = other.GetPosition();
	_velocity = other.GetVelocity();
	_colour = other.GetColour();
}

/******************************************************************************************************************/

GameObject& GameObject::operator=(const GameObject& other)
{
	_angle = other.GetAngle();
	_scale = other.GetScale();
	_position = other.GetPosition();
	_velocity = other.GetVelocity();
	_colour = other.GetColour();
	return *this;
}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

void GameObject::Draw(glm::mat4 MVM, ID3D11DeviceContext* context, ID3D11Buffer* uniformBuffer)
{
	if (_alive)
	{
		MVM = glm::translate(MVM, glm::vec3(_position.x(), _position.y(), 0));
		MVM = glm::rotate(MVM, -_angle, glm::vec3(0, 0, 1)); // Rotates anti-clockwise
		MVM = glm::scale(MVM, glm::vec3(_scale, _scale, 1));
		MVM = glm::transpose(MVM);

		UniformBuffer uniforms;
		memcpy(&uniforms.MVM, &MVM[0][0], sizeof(DirectX::XMFLOAT4X4));
		_colour.copyToArray((float*)&uniforms.Colour);

		// Need to update uniform buffer here
		D3D11_MAPPED_SUBRESOURCE ms;
		context->Map(uniformBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);			// map the buffer
		memcpy(ms.pData, &uniforms, sizeof(UniformBuffer));								// copy the data
		context->Unmap(uniformBuffer, NULL);											// unmap the buffer
		context->VSSetConstantBuffers(0, 1, &uniformBuffer);
		context->PSSetConstantBuffers(0, 1, &uniformBuffer);

		Draw(context);
	}
}

/******************************************************************************************************************/

void GameObject::Update(double deltaTime)
{
	Vector4 vel = _velocity;
	vel *= deltaTime;
	_position += vel;

	// Wrap around
	if (_position.x() < -1) _position.x(_position.x() + 2);
	else if (_position.x() > 1) _position.x(_position.x() - 2);
	if (_position.y() < -1) _position.y(_position.y() + 2);
	else if (_position.y() > 1) _position.y(_position.y() - 2);
}

/******************************************************************************************************************/

bool GameObject::CollideWith(const GameObject* other, float collisionRange)
{
	Vector4 diff = other->GetPosition();
	diff -= this->GetPosition();

	return (diff.length() < collisionRange);
}

/******************************************************************************************************************/
