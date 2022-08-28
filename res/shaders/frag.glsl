#version 430

layout(binding = 0) uniform sampler2D textureImage;

out vec4 color;

in vec2 tc;

uniform float uTexFrac = 1.0;
uniform float uTexSize = 256.0;
uniform vec2 uTexOffset = vec2(0.0, 0.0);

void main()
{
	vec2 coords = vec2(
		(floor((tc.x * uTexFrac + uTexOffset.x) * uTexSize) + 0.5) / uTexSize,
		(floor((tc.y * uTexFrac + uTexOffset.y) * uTexSize) + 0.5) / uTexSize
	);	
	color = texture(textureImage, coords);
}
