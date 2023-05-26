#pragma once
#include <glm/glm.hpp>

class Transform
{
public:

	Transform();
	Transform(glm::vec2 position, glm::vec2 scale, float rotation);
	Transform(float xPos, float yPos, float xScale, float yScale, float rotation);

	// Transformations
	void Translate(glm::vec2 translation) { position = position + translation; UpdateModelMatrix();}
	void Translate(float xTrans, float yTrans) { Translate(glm::vec2(xTrans, yTrans)); }

	// Additive scaling (not relative to current size)
	void Scale(glm::vec2 scaling) { scale = scale + scaling; UpdateModelMatrix(); } 
	void Scale(float xScale, float yScale) { Scale(glm::vec2(xScale, yScale)); }
	// Multiplicative scaling (relative to current size)
	void ScaleFactor(glm::vec2 scaling) { scale = scale * scaling; UpdateModelMatrix();}
	void ScaleFactor(float xScale, float yScale) { ScaleFactor(glm::vec2(xScale, yScale)); }

	void Rotate(float degrees) { rotation += degrees;  }

	// Getters
	const glm::mat4& GetModelMatrix() { return modelMatrix; } // TODO: I imagine I'll b putting calculation back in here
	// Want to change renderer to take pos and scale to speed things up esp with many quads -- don't need matrix ops
	const glm::vec2& GetPosition() const { return position; }
	const glm::vec2& GetScale() const { return scale; }
	float GetRotation() const { return rotation; }

	glm::mat4 GetViewMatrix() { UpdateModelMatrix(); return glm::inverse(modelMatrix); }

	// Setters
	void SetPosition(glm::vec2 newPos) { position = newPos; UpdateModelMatrix();}
	void SetPosition(float xPos, float yPos) { position = glm::vec2(xPos, yPos); UpdateModelMatrix();}
	void SetPositionX(float xPos) { position.x = xPos; UpdateModelMatrix(); }
	void SetPositionY(float yPos) { position.y =  yPos; UpdateModelMatrix(); }

	void SetScale(glm::vec2 newScale) { scale = newScale; UpdateModelMatrix(); }
	void SetScale(float xScale, float yScale) { scale = glm::vec2(xScale, yScale); UpdateModelMatrix();}
	void SetScaleX(float xScale) { scale.x = xScale; UpdateModelMatrix();}
	void SetScaleY(float yScale) { scale.y = yScale; UpdateModelMatrix();}

	void SetRotation(float degrees) { rotation = degrees; UpdateModelMatrix();}

private:

	glm::vec2 position;
	glm::vec2 scale;
	float rotation;
	glm::mat4 modelMatrix;

	// Applies values above to model matrix
	void UpdateModelMatrix();

};

