#include "ScoreDisplay.h"

/******************************************************************************************************************/

ScoreDisplay::ScoreDisplay(ID3D11Device* device,
						   ID3D11DeviceContext* context)
{
	// Setup letter meshes
	_numbers[0].AddVertex({ -0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numbers[0].AddVertex({ 0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numbers[0].AddVertex({ 0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numbers[0].AddVertex({ -0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numbers[0].AddVertex({ -0.04f, -0.09f, 0, 1, 1, 1, 1 });

	_numbers[1].AddVertex({ 0.0f, -0.09f, 0, 1, 1, 1, 1 });
	_numbers[1].AddVertex({ 0.0f, 0.09f, 0, 1, 1, 1, 1 });

	_numbers[2].AddVertex({ 0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numbers[2].AddVertex({ -0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numbers[2].AddVertex({ -0.04f, 0.00f, 0, 1, 1, 1, 1 });
	_numbers[2].AddVertex({ 0.04f, 0.00f, 0, 1, 1, 1, 1 });
	_numbers[2].AddVertex({ 0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numbers[2].AddVertex({ -0.04f, 0.09f, 0, 1, 1, 1, 1 });

	_numbers[3].AddVertex({ -0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numbers[3].AddVertex({ 0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numbers[3].AddVertex({ 0.04f, 0.00f, 0, 1, 1, 1, 1 });
	_numbers[3].AddVertex({ -0.04f, 0.00f, 0, 1, 1, 1, 1 });
	_numbers[3].AddVertex({ 0.04f, 0.00f, 0, 1, 1, 1, 1 });
	_numbers[3].AddVertex({ 0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numbers[3].AddVertex({ -0.04f, 0.09f, 0, 1, 1, 1, 1 });

	_numbers[4].AddVertex({ -0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numbers[4].AddVertex({ -0.04f, 0.00f, 0, 1, 1, 1, 1 });
	_numbers[4].AddVertex({ 0.04f, 0.00f, 0, 1, 1, 1, 1 });
	_numbers[4].AddVertex({ 0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numbers[4].AddVertex({ 0.04f, -0.09f, 0, 1, 1, 1, 1 });

	_numbers[5].AddVertex({ -0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numbers[5].AddVertex({ 0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numbers[5].AddVertex({ 0.04f, 0.00f, 0, 1, 1, 1, 1 });
	_numbers[5].AddVertex({ -0.04f, 0.00f, 0, 1, 1, 1, 1 });
	_numbers[5].AddVertex({ -0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numbers[5].AddVertex({ 0.04f, 0.09f, 0, 1, 1, 1, 1 });

	_numbers[6].AddVertex({ -0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numbers[6].AddVertex({ -0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numbers[6].AddVertex({ 0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numbers[6].AddVertex({ 0.04f, 0.00f, 0, 1, 1, 1, 1 });
	_numbers[6].AddVertex({ -0.04f, 0.00f, 0, 1, 1, 1, 1 });

	_numbers[7].AddVertex({ -0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numbers[7].AddVertex({ 0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numbers[7].AddVertex({ 0.04f, -0.09f, 0, 1, 1, 1, 1 });

	_numbers[8].AddVertex({ 0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numbers[8].AddVertex({ -0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numbers[8].AddVertex({ -0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numbers[8].AddVertex({ 0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numbers[8].AddVertex({ 0.04f, -0.00f, 0, 1, 1, 1, 1 });
	_numbers[8].AddVertex({ -0.04f, -0.00f, 0, 1, 1, 1, 1 });
	_numbers[8].AddVertex({ 0.04f, -0.00f, 0, 1, 1, 1, 1 });
	_numbers[8].AddVertex({ 0.04f, -0.09f, 0, 1, 1, 1, 1 });

	_numbers[9].AddVertex({ 0.04f, -0.09f, 0, 1, 1, 1, 1 });
	_numbers[9].AddVertex({ 0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numbers[9].AddVertex({ -0.04f, 0.09f, 0, 1, 1, 1, 1 });
	_numbers[9].AddVertex({ -0.04f, 0.00f, 0, 1, 1, 1, 1 });
	_numbers[9].AddVertex({ 0.04f, 0.00f, 0, 1, 1, 1, 1 });

	for (int i = 0; i < 10; i++)
	{
		_numbers[i].CreateVBO(device, context);
	}

}

/******************************************************************************************************************/

ScoreDisplay::~ScoreDisplay()
{
}

/******************************************************************************************************************/

void ScoreDisplay::Draw(int score, glm::mat4 MVM, ID3D11DeviceContext* context, ID3D11Buffer* uniformBuffer)
{
	int number = 0;
	float offset = -0.1f;
	int numberOfDigits = 0;
	int tscore = score;
	while (tscore >= 10)
	{
		tscore /= 10;
		numberOfDigits++;
	}

	MVM = glm::translate(MVM, glm::vec3(-offset * numberOfDigits, 0, 0));
	glm::mat4 tempMVM;

	UniformBuffer uniforms;
	uniforms.Colour = { 1.0f, 1.0f, 0.0f, 1.0f };

	while (score >= 10)
	{
		tempMVM = glm::transpose(MVM);
		memcpy(&uniforms.MVM, &tempMVM[0][0], sizeof(DirectX::XMFLOAT4X4));

		// Need to update uniform buffer here
		D3D11_MAPPED_SUBRESOURCE ms;
		context->Map(uniformBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);			// map the buffer
		memcpy(ms.pData, &uniforms, sizeof(UniformBuffer));								// copy the data
		context->Unmap(uniformBuffer, NULL);											// unmap the buffer
		context->VSSetConstantBuffers(0, 1, &uniformBuffer);
		context->PSSetConstantBuffers(0, 1, &uniformBuffer);

		number = score % 10;
		_numbers[number].Draw(context);
		score /= 10;
		MVM = glm::translate(MVM, glm::vec3(offset, 0, 0));
	}

	tempMVM = glm::transpose(MVM);
	memcpy(&uniforms.MVM, &tempMVM[0][0], sizeof(DirectX::XMFLOAT4X4));

	// Need to update uniform buffer here
	D3D11_MAPPED_SUBRESOURCE ms;
	context->Map(uniformBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);			// map the buffer
	memcpy(ms.pData, &uniforms, sizeof(UniformBuffer));								// copy the data
	context->Unmap(uniformBuffer, NULL);											// unmap the buffer
	context->VSSetConstantBuffers(0, 1, &uniformBuffer);
	context->PSSetConstantBuffers(0, 1, &uniformBuffer);

	_numbers[score].Draw(context);
}

/******************************************************************************************************************/
