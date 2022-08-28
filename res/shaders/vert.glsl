#version 430

layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 texCoords;

uniform vec2 uScale = vec2(1.0, 1.0);
uniform vec2 uOffset = vec2(0.0, 0.0);
uniform vec2 uScreenDimensions = vec2(960.0, 540.0);
uniform float uRotation = 0.0f;

out vec2 tc;

void main()
{	
	mat2 rotationMat = mat2(
		cos(uRotation), -sin(uRotation),
		sin(uRotation), cos(uRotation)
	);	
	
	vec2 rotated = rotationMat * pos.xy;

	vec2 position2d = vec2(
		(rotated.x * uScale.x + uOffset.x * 2.0) / uScreenDimensions.x,
		(rotated.y * uScale.y + uOffset.y * 2.0) / uScreenDimensions.y	
	);

	gl_Position = vec4(position2d.xy, 0.0, 1.0);

	tc = texCoords;
}
