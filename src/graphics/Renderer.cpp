#include "Renderer.h"

Renderer::Renderer():
	camera(nullptr), shader(nullptr)
{}

Renderer::Renderer(Camera* camera, Shader* shader) : 
	camera(camera), shader(shader), quadVertices(nullptr), numIndices(0), quadBufferPointer(nullptr), numBoundTextures(0){
	Initialize();
}

void Renderer::Initialize() {
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	// Create VAO
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	
	// Heap allocate vertex buffer (cpu side)
	quadVertices = new QuadVertex[MAX_VERTICES];
	// Set up VBO
	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	// Allocates memory for buffer (gpu side) but passes nothing
	glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertex) * MAX_VERTICES, nullptr, GL_DYNAMIC_DRAW);

	// Enable vertex attributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	// Specify vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, position));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, color));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, texCoord));
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, texSlot));
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, tileCount));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, texCoordBounds));

	// Initialize index buffer with quad indicies up to max indicies 
	unsigned int* quadIndices = new unsigned int[MAX_INDICES];
	unsigned int offset = 0;
	for (int i = 0; i < MAX_INDICES; i+=6) {
		quadIndices[i + 0] = 0 + offset;
		quadIndices[i + 1] = 1 + offset;
		quadIndices[i + 2] = 2 + offset;

		quadIndices[i + 3] = 2 + offset;
		quadIndices[i + 4] = 3 + offset;
		quadIndices[i + 5] = 0 + offset;

		offset += 4;
	}

	// Init IBO and pass index buffer to gpu
	glGenBuffers(1, &quadIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * MAX_INDICES, quadIndices, GL_STATIC_DRAW);
	// On the GPU so dont need anymore
	delete[] quadIndices;

	// Default quad vertices
	quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
	quadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
	quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	// Default quad vertices
	quadVertexPosSimple[0] = { -0.5f, -0.5f };
	quadVertexPosSimple[1] = {  0.5f, -0.5f };
	quadVertexPosSimple[2] = {  0.5f,  0.5f };
	quadVertexPosSimple[3] = { -0.5f,  0.5f };

	// Populate sampler2D array
	int textureIDs[MAX_TEXTURES] = {};
	for (int i = 0; i < MAX_TEXTURES; i++) {
		textureIDs[i] = i;
	}
	// Set uniform
	shader->SetInts("textures", MAX_TEXTURES, textureIDs);

	// Clears memory for bound texture array
	for (int i = 0; i < MAX_TEXTURES; i++) {
		boundTextures[i] = nullptr;
	}

	// Setting up line rendering
	lineShader = ResourceManager::LoadShader("res/shaders/line.vert", "res/shaders/line.frag", "line");
	glGenVertexArrays(1, &lineVAO);
	glBindVertexArray(lineVAO);

	// Allocates vertex buffer
	glGenBuffers(1, &lineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

}


void Renderer::Start() {
	//GLClearError();
	numIndices = 0;
	// Reset pointer to beginning of buffer
	quadBufferPointer = quadVertices;
}


void Renderer::DrawQuad(glm::vec4 color, glm::vec2 position, glm::vec2 scale) {
	// Another draw call needed
	if (numIndices > MAX_INDICES - 6) {
		End();
		Start();
	}

	float texSlot = -1.0f;
	glm::vec2 offset = scale / 2.0f;
	// Bottom left vertex
	quadBufferPointer->position = glm::vec3(position.x - offset.x, position.y - offset.y, 0.0f);
	quadBufferPointer->color = color;
	quadBufferPointer->texCoord = { 0.0f, 0.0f };
	quadBufferPointer->texSlot = texSlot;
	quadBufferPointer++;

	// Bottom right vertex
	quadBufferPointer->position = glm::vec3(position.x + offset.x, position.y - offset.y, 0.0f);
	quadBufferPointer->color = color;
	quadBufferPointer->texCoord = { 1.0f, 0.0f };
	quadBufferPointer->texSlot = texSlot;
	quadBufferPointer++;

	// Top right vertex
	quadBufferPointer->position = glm::vec3(position.x + offset.x, position.y + offset.y, 0.0f);
	quadBufferPointer->color = color;
	quadBufferPointer->texCoord = { 1.0f, 1.0f };
	quadBufferPointer->texSlot = texSlot;
	quadBufferPointer++;

	// Top left vertex
	quadBufferPointer->position = glm::vec3(position.x - offset.x, position.y + offset.y, 0.0f);
	quadBufferPointer->color = color;
	quadBufferPointer->texCoord = { 0.0f, 1.0f };
	quadBufferPointer->texSlot = texSlot;
	quadBufferPointer++;

	numIndices += 6;
}

void Renderer::DrawQuad(glm::vec4 color, const glm::mat4& modelMatrix) {
	// Another draw call needed
	if (numIndices > MAX_INDICES - 6) {
		End();
		Start();
	}

	float texSlot = -1.0f;

	// Bottom left vertex
	quadBufferPointer->position = modelMatrix * quadVertexPositions[0];
	quadBufferPointer->color = color;
	quadBufferPointer->texCoord = { 0.0f, 0.0f };
	quadBufferPointer->texSlot = texSlot;
	quadBufferPointer++;

	// Bottom right vertex
	quadBufferPointer->position = modelMatrix * quadVertexPositions[1];
	quadBufferPointer->color = color;
	quadBufferPointer->texCoord = { 1.0f, 0.0f };
	quadBufferPointer->texSlot = texSlot;
	quadBufferPointer++;

	// Top right vertex
	quadBufferPointer->position = modelMatrix * quadVertexPositions[2];
	quadBufferPointer->color = color;
	quadBufferPointer->texCoord = { 1.0f, 1.0f };
	quadBufferPointer->texSlot = texSlot;
	quadBufferPointer++;

	// Top left vertex
	quadBufferPointer->position = modelMatrix * quadVertexPositions[3];
	quadBufferPointer->color = color;
	quadBufferPointer->texCoord = { 0.0f, 1.0f };
	quadBufferPointer->texSlot = texSlot;
	quadBufferPointer++;

	numIndices += 6;
}


void Renderer::DrawQuad(Texture* texture, const glm::mat4& modelMatrix)  {
	// Default constructor makes tex coords the whole texture
	DrawQuad(texture, defaultSubTexture, modelMatrix);
}

void Renderer::DrawQuad(Texture* texture, glm::vec2 position, glm::vec2 scale) {
	DrawQuad(texture, defaultSubTexture, position, scale);
}

// PREFFERED FUNCTION
void Renderer::DrawQuad(Texture* texture, const SubTexture& subTexture, glm::vec2 position, glm::vec2 scale, bool tiled) {
	// Another draw call needed
	if (numIndices > MAX_INDICES - 6) {
		End();
		Start();
	}

	// Represents texture the draw call is using
	float texSlot = -1.0f;

	// Working with not previously bound texture;
	if (texture->GetBoundSlot() < 0) {
		// New draw call if texture limit reached
		if (numBoundTextures >= MAX_TEXTURES) {
			End();
			Start();
		}

		// Bind the new texture and keep track of it in boundTextures
		texture->Bind(numBoundTextures);
		if (numBoundTextures < MAX_TEXTURES) boundTextures[numBoundTextures] = texture; // If statement to get rid of buffer overrun warning
		numBoundTextures++;
	}

	texSlot = (float)texture->GetBoundSlot();
	glm::vec2 offset = scale / 2.0f;
	
	glm::vec2 tileCount(1.0f,1.0f);
	glm::vec4 texCoordBounds(0, 0, 1, 1);
	if (tiled) {
		tileCount = glm::vec2(scale / texture->SubWorldSize(subTexture));
		texCoordBounds = glm::vec4(subTexture.xCoord, subTexture.yCoord,
			subTexture.xCoord + subTexture.coordWidth, subTexture.yCoord + subTexture.coordHeight);
	}

	// Bottom left vertex
	quadBufferPointer->position = glm::vec3(position.x - offset.x, position.y - offset.y, 0.0f);
	quadBufferPointer->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	quadBufferPointer->texCoord = { subTexture.xCoord, subTexture.yCoord };
	quadBufferPointer->texSlot = texSlot;
	quadBufferPointer->tileCount = tileCount;
	quadBufferPointer->texCoordBounds = texCoordBounds;
	quadBufferPointer++;

	// Bottom right vertex
	quadBufferPointer->position = glm::vec3(position.x + offset.x, position.y - offset.y, 0.0f);
	quadBufferPointer->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	quadBufferPointer->texCoord = { subTexture.xCoord + subTexture.coordWidth, subTexture.yCoord };
	quadBufferPointer->texSlot = texSlot;
	quadBufferPointer->tileCount = tileCount;
	quadBufferPointer->texCoordBounds = texCoordBounds;
	quadBufferPointer++;

	// Top right vertex
	quadBufferPointer->position = glm::vec3(position.x + offset.x, position.y + offset.y, 0.0f);
	quadBufferPointer->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	quadBufferPointer->texCoord = { subTexture.xCoord + subTexture.coordWidth, subTexture.yCoord + subTexture.coordHeight };
	quadBufferPointer->texSlot = texSlot;
	quadBufferPointer->tileCount = tileCount;
	quadBufferPointer->texCoordBounds = texCoordBounds;
	quadBufferPointer++;

	// Top left vertex
	quadBufferPointer->position = glm::vec3(position.x - offset.x, position.y + offset.y, 0.0f);
	quadBufferPointer->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	quadBufferPointer->texCoord = { subTexture.xCoord, subTexture.yCoord + subTexture.coordHeight };
	quadBufferPointer->texSlot = texSlot;
	quadBufferPointer->tileCount = tileCount;
	quadBufferPointer->texCoordBounds = texCoordBounds;
	quadBufferPointer++;

	numIndices += 6;
}


void Renderer::DrawQuad(Texture* texture, const SubTexture &subTexture, const glm::mat4& modelMatrix) {
	// Another draw call needed
	if (numIndices > MAX_INDICES - 6) {
		End();
		Start();
	}

	// Represents texture the draw call is using
	float texSlot = -1.0f;

	// Working with not previously bound texture;
	if (texture->GetBoundSlot() < 0) {
		// New draw call if texture limit reached
		if (numBoundTextures >= MAX_TEXTURES) {
			End();
			Start();
		}

		// Bind the new texture and keep track of it in boundTextures
		texture->Bind(numBoundTextures);
		if (numBoundTextures < MAX_TEXTURES) boundTextures[numBoundTextures] = texture; // If statement to get rid of buffer overrun warning
		numBoundTextures++;
	}

	texSlot = (float)texture->GetBoundSlot();

	// Bottom left vertex
	quadBufferPointer->position = modelMatrix * quadVertexPositions[0];
	quadBufferPointer->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	quadBufferPointer->texCoord = { subTexture.xCoord, subTexture.yCoord };
	quadBufferPointer->texSlot = texSlot;
	quadBufferPointer++;

	// Bottom right vertex
	quadBufferPointer->position = modelMatrix * quadVertexPositions[1];
	quadBufferPointer->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	quadBufferPointer->texCoord = { subTexture.xCoord + subTexture.coordWidth, subTexture.yCoord };
	quadBufferPointer->texSlot = texSlot;
	quadBufferPointer++;

	// Top right vertex
	quadBufferPointer->position = modelMatrix * quadVertexPositions[2];
	quadBufferPointer->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	quadBufferPointer->texCoord = { subTexture.xCoord + subTexture.coordWidth, subTexture.yCoord + subTexture.coordHeight };
	quadBufferPointer->texSlot = texSlot;
	quadBufferPointer++;

	// Top left vertex
	quadBufferPointer->position = modelMatrix * quadVertexPositions[3];
	quadBufferPointer->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	quadBufferPointer->texCoord = { subTexture.xCoord, subTexture.yCoord + subTexture.coordHeight };
	quadBufferPointer->texSlot = texSlot;
	quadBufferPointer++;

	numIndices += 6;
}



void Renderer::DrawLine(glm::vec2 startPos, glm::vec2 endPos, float width, glm::vec4 color) {
	// Set up vertices
	float lineVerts[] = { 
		startPos.x, startPos.y, 
		endPos.x, endPos.y 
	};
	// Activate shader
	lineShader->Use();
	lineShader->SetVec4("color", color);
	lineShader->SetMat4("view", camera->GetViewMatrix());
	lineShader->SetMat4("projection", camera->GetProjectionMatrix());
	// Bind buffers
	glBindVertexArray(lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	// Insert data into buffer
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 4, lineVerts);
	// Set line width and draw
	glLineWidth(width);
	glDrawArrays(GL_LINES, 0, 2);
}

void Renderer::End() {
	Flush();
	// Unbinds all textures
	for (unsigned int i = 0; i < numBoundTextures; i++) {
		boundTextures[i]->boundSlot = -1;
	}
	numBoundTextures = 0;
}

void Renderer::Flush() {
	shader->Use();
	shader->SetMat4("view", camera->GetViewMatrix());
	shader->SetMat4("projection", camera->GetProjectionMatrix());
	
	glBindVertexArray(quadVAO);
	// Send data into vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	GLsizeiptr size = (uint8_t*)quadBufferPointer - (uint8_t*)quadVertices;
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, quadVertices);

	// Draw call
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

}

Renderer::~Renderer() {
	delete[] quadVertices;
}

