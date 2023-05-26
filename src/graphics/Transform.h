#pragma once
#include <glm/glm.hpp>

class Transform
{
public:

	Transform();
	Transform(glm::vec2 position, glm::vec2 scale, float rotation);
	Transform(float xPos, float yPos, float xScale, float yScale, float rotation);

	// Transformations
	void Translate(glm::vec2 translation) { position = position + translation; }
	void Translate(float xTrans, float yTrans) { Translate(glm::vec2(xTrans, yTrans)); }

	// Additive scaling (not relative to current size)
	void Scale(glm::vec2 scaling) { scale = scale + scaling; } 
	void Scale(float xScale, float yScale) { Scale(glm::vec2(xScale, yScale)); }
	// Multiplicative scaling (relative to current size)
	void ScaleFactor(glm::vec2 scaling) { scale = scale * scaling; }
	void ScaleFactor(float xScale, float yScale) { ScaleFactor(glm::vec2(xScale, yScale)); }

	void Rotate(float degrees) { rotation += degrees;  }

	// Getters
	const glm::mat4& GetModelMatrix() { UpdateModelMatrix(); return modelMatrix; } 
	const glm::vec2& GetPosition() const { return position; }
	const glm::vec2& GetScale() const { return scale; }
	float GetRotation() const { return rotation; }

	glm::mat4 GetViewMatrix() { UpdateModelMatrix(); return glm::inverse(modelMatrix); }

	// Setters
	void SetPosition(glm::vec2 newPos) { position = newPos; }
	void SetPosition(float xPos, float yPos) { position = glm::vec2(xPos, yPos); }
	void SetPositionX(float xPos) { position.x = xPos; UpdateModelMatrix(); }
	void SetPositionY(float yPos) { position.y =  yPos; UpdateModelMatrix(); }

	void SetScale(glm::vec2 newScale) { scale = newScale; UpdateModelMatrix(); }
	void SetScale(float xScale, float yScale) { scale = glm::vec2(xScale, yScale); }
	void SetScaleX(float xScale) { scale.x = xScale; }
	void SetScaleY(float yScale) { scale.y = yScale;}

	void SetRotation(float degrees) { rotation = degrees;}

	// Data can be public now that matrix is calculated on get
	glm::vec2 position;
	glm::vec2 scale;
	float rotation;

private:

	glm::mat4 modelMatrix;

	// Applies values above to model matrix
	void UpdateModelMatrix();

};

