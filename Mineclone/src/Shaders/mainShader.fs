#version 330 core

struct VertexData{
	vec3 position;
	int faceNormalIndex;

	vec2 size;
	vec2 padding2;
};

flat in VertexData vertexData;

float[6] lightMultiplier = float[6](0.8, 0.5, 0.4, 0.7, 0.9, 0.3);

void main()
{
    gl_FragColor = lightMultiplier[vertexData.faceNormalIndex] * vec4(0.8, 0.32, 0.1, 1.0);
}

