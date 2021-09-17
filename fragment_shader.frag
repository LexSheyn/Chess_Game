#version 330 compatibility

in vec4 vert_pos;

uniform sampler2D texture;
uniform bool hasTexture;
uniform vec2 lightPos;

void main()
{
	// Ambient light
	vec4 ambient = vec4(0.1, 0.1, 0.3, 1.0);
	
	// Convert light to view coords
	vec2 lightPosTemp = (gl_ModelViewProjectionMatrix * vec4(lightPos, 0, 1)).xy;
	
	// Calculate the vector from light to pixel (Make circular)
	vec2 lightToFrag = lightPosTemp - vert_pos.xy;
	lightToFrag.x = lightToFrag.x * 2;
	lightToFrag.y = lightToFrag.y * 1.2; // / 1.7;

	// Length of the vector (distance)
	float vecLength = clamp(length(lightToFrag) * 2, 0, 1);

    // Lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    // Multiply it by the color and lighting
	if(hasTexture == true)
	{
		gl_FragColor = gl_Color * pixel * (clamp(ambient + vec4(1-vecLength, 1-vecLength, 1-vecLength, 1), 0, 1));
	}
	else
	{
		gl_FragColor = gl_Color;
	}
}