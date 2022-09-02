#version 430

out vec4 color;
in vec2 varyingVert;

void main()
{
	float radius = sqrt(0.05);
	float height = sqrt(0.5 - radius * radius) / (sqrt(5) / 2.0);

	if(varyingVert.y < 2.0 * varyingVert.x &&
		varyingVert.y < -2.0 * varyingVert.x + 2.0 && varyingVert.y > height ||
		(varyingVert.x - 0.5) * (varyingVert.x - 0.5) + 
		(varyingVert.y - 0.5) * (varyingVert.y - 0.5) < radius * radius)	
		color = vec4(0.0, 0.3, 0.6, 0.9);
	else
		color = vec4(0.0, 0.0, 0.0, 0.0);
}
