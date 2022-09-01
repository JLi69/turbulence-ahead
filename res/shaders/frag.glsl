#version 430

layout(binding = 0) uniform sampler2D textureImage;

out vec4 color;

in vec2 tc;

uniform float uTexFrac = 1.0;
uniform vec2 uTexSize = vec2(256.0, 256.0);
uniform vec2 uTexOffset = vec2(0.0, 0.0);
uniform float uDarkness = 1.0;

void main()
{
	vec2 coords = vec2(
		(floor((tc.x * uTexFrac + uTexOffset.x) * uTexSize.x) + 0.5) / uTexSize.x,
		(floor((tc.y * uTexFrac + uTexOffset.y) * uTexSize.y) + 0.5) / uTexSize.y
	);

	if(coords.y - uTexOffset.y > uTexFrac)
	{
		coords.y = uTexOffset.y + uTexFrac - 0.5 / uTexSize.y;
	}

	color = texture(textureImage, coords);
	float alpha = color.a;
	color *= uDarkness;
	color.a = alpha;
}
