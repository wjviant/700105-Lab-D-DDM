#include "VBO.h"

/******************************************************************************************************************/

VBO::VBO()
	:	_vbo(NULL), 
		_numVertices(0)
{
}

/******************************************************************************************************************/

VBO::~VBO()
{
	if (_vbo)
		_vbo->Release();
}

/******************************************************************************************************************/

void VBO::Create(Vertex vertices[], int numVertices, ID3D11Device* device, ID3D11DeviceContext* context)
{
	_numVertices = numVertices;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(Vertex) * numVertices;   // size is the VERTEX struct * num vertices
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	device->CreateBuffer(&bd, NULL, &_vbo);        // create the buffer

	// copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	context->Map(_vbo, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);		// map the buffer
	memcpy(ms.pData, vertices, sizeof(Vertex) * numVertices);			// copy the data
	context->Unmap(_vbo, NULL);											// unmap the buffer
}

/******************************************************************************************************************/

void VBO::Draw(ID3D11DeviceContext* context)
{
	// select which vertex buffer to display
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &_vbo, &stride, &offset);

	// select which primtive type we are using
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// draw the vertex buffer to the back buffer
	context->Draw(_numVertices, 0);

}

/******************************************************************************************************************/
