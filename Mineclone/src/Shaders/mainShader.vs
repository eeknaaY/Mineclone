#version 460 core

struct VertexData{
	vec3 position;
	int faceNormalIndex;

	vec2 size;
	vec2 padding2;
};

flat out VertexData vertexData; 

layout(std430, binding = 0) readonly buffer vertexPullBuffer 
{
	VertexData AllVertexData[];
};

vec3 rotateVector(vec3 original, vec3 rotation){
	vec3 rotationRadian = radians(rotation);

	mat3 rotationMatrix = mat3(cos(rotationRadian.x) * cos(rotationRadian.y), cos(rotationRadian.x) * sin(rotationRadian.y) * sin(rotationRadian.z) - sin(rotationRadian.x) * cos(rotationRadian.z), cos(rotationRadian.x) * sin(rotationRadian.y) * cos(rotationRadian.z) + sin(rotationRadian.x) * sin(rotationRadian.z),
								sin(rotationRadian.x) * cos(rotationRadian.y), sin(rotationRadian.x) * sin(rotationRadian.y) * sin(rotationRadian.z) + cos(rotationRadian.x) * cos(rotationRadian.z), sin(rotationRadian.x) * sin(rotationRadian.y) * cos(rotationRadian.z) - cos(rotationRadian.x) * sin(rotationRadian.z),
								-sin(rotationRadian.y), cos(rotationRadian.y) * sin(rotationRadian.z), cos(rotationRadian.y) * cos(rotationRadian.z));
	
	
	return rotationMatrix * original;
}

vec3 facePositions[4] = vec3[4]
(
	vec3(0, 0, 0),
	vec3(1, 0, 0),
	vec3(1, 1, 0),
	vec3(0, 1, 0)
);

vec3 rotationDegrees[6] = vec3[6](
	// Rz, Ry, Rx
	vec3(0, 0, 0), // North
	vec3(0, 180, 0), // South
	vec3(0, -90, 0), // West
	vec3(0, 90, 0), // East
	vec3(90, 0, 90), // Up
	vec3(90, 0, -90) // DOwn
);

vec3 faceNormals[6] = vec3[6](
	vec3(0, 0, 1), // North Default, 0deg
	vec3(0, 0, -1), // South 180 Deg Y-Axis
	vec3(1, 0, 0), // West -90 Deg Y-Axis
	vec3(-1, 0, 0), // East 90 Deg Y-Axis
	vec3(0, 1, 0), // Up 90 Deg X-Axis
	vec3(0, -1, 0) // Down -90 Deg X-Axis
);

int indices[6] = {0, 1, 2, 0, 2, 3};

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 TransformMatrix;

void main()
{   
	int vertexIndex = gl_VertexID / 6;
  	int faceIndex = gl_VertexID % 6;

	VertexData currentVertexData = AllVertexData[vertexIndex];
	vertexData = currentVertexData;

	vec3 addedPosition = facePositions[indices[faceIndex]];;
	addedPosition = vec3(addedPosition.x * currentVertexData.size.x, addedPosition.y * currentVertexData.size.y, 0);
	currentVertexData.position += rotateVector(addedPosition, rotationDegrees[currentVertexData.faceNormalIndex]);

	gl_Position = ProjectionMatrix * ViewMatrix * TransformMatrix * vec4(currentVertexData.position, 1.0);
}