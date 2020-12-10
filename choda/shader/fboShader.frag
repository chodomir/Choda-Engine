#version 330 core

in vec2 texCoord;

uniform sampler2D screenTexture;

float offset = 1.0 / 300.0;


void main() {
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset), // top left
		vec2(0.0f, offset), // top center
		vec2(offset, offset), // top right
		vec2(-offset, 0.0f), // center left
		vec2(0.0f, 0.0f), // center
		vec2(offset, 0.0f), // center right
		vec2(-offset, -offset), // bottom left
		vec2(0.0f, -offset), // bottom center
		vec2(offset, -offset) // bottom right
	);

	float kernel[9] = float[](
		1.0, 2.0, 1.0,
		2.0, 4.0, 2.0,
		1.0, 2.0, 1.0
	);

	vec3 sampleTex[9];
	for (int i = 0; i < 9; i++) 
		sampleTex[i] = vec3(texture(screenTexture, texCoord.st + offsets[i]));

	vec3 color = vec3(0.0f);
	for (int i = 0; i < 9; i++)
		color += sampleTex[i] * kernel[i] / 16;
		
	gl_FragColor = vec4(color, 1.0f);
}