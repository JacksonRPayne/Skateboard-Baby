#pragma once
#include "system/ResourceManager.h"
#include "scene/Camera.h"
#include <stdio.h>
// 10,000 seems to be the performance sweet spot
#define MAX_QUADS 10000
#define MAX_VERTICES MAX_QUADS*4
#define MAX_INDICES MAX_QUADS*6
#define MAX_TEXTURES 2

// The format of each vertex in vertex buffer
struct QuadVertex {
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 texCoord;
	float texSlot;
};

class Renderer
{
public:
	// For throwaways
	Renderer();
	Renderer(Camera* camera, Shader* shader);
	// Placement new only
	Renderer(const Renderer& other) = delete;
	Renderer operator=(const Renderer& other) = delete;
	Renderer(const Renderer&& other) = delete;
	Renderer operator=(const Renderer&& other) = delete;
	~Renderer();

	// Starts the rendering for the frame
	void Start();
	// Draws a colored quad (no texture)
	void DrawQuad(glm::vec4 color, const glm::mat4& modelMatrix);
	// Renders quad with texture
	void DrawQuad(Texture* texture, const glm::mat4& modelMatrix);
	// Renders quad with subtexture
	// This is the most preffered method to render a quad
	void DrawQuad(Texture* texture, const SubTexture& subTexture, glm::vec2 position, glm::vec2 scale);
	// Use this for rotated quads ONLY: the matrix mu
	void DrawQuad(Texture* texture, const SubTexture &subTexture, const glm::mat4& modelMatrix);
	// Draws a line for debugging ONLY. Not written batched, so performance is slow but it's only for debugging
	// NEVER under ANY CIRCUMSTANCES call this in a release version or for non debugging purposes
	void DrawLine(glm::vec2 startPos, glm::vec2 endPos, float width = 2.0f,  glm::vec4 color = {0.0f, 1.0f, 0.0f, 1.0f});
	// Ends the rendering for the frame (maybe flushes)
	void End();

	void ChangeShader(Shader* newShader) { shader = newShader; }

	// Camera is public so it can be set up by scene if needed
	Camera* camera;

private:

	void Initialize();
	// Renders buffer
	void Flush();
	// Calculates value given to shader
	float CalculateTexelsPerPixel();

	unsigned int quadVAO;
	unsigned int quadVBO;
	unsigned int quadIBO;

	// Stores the VBO buffer and a pointer to iterate through it
	QuadVertex* quadVertices;
	QuadVertex* quadBufferPointer;
	// Stores default quad vertices to be translated by matrix
	glm::vec4 quadVertexPositions[4];
	glm::vec2 quadVertexPosSimple[4];

	unsigned int lineVAO;
	unsigned int lineVBO;

	unsigned int numIndices;
	// Stores bound textures in this draw call
	Texture* boundTextures[MAX_TEXTURES];
	unsigned int numBoundTextures;

	Shader* shader;
	Shader* lineShader;
	// So that subtexture isn't recreated every time quad is rendererd without one
	SubTexture defaultSubTexture = SubTexture();

};

