#type vertex
#version 330 core
		
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TextureCoord;
layout(location = 3) in float a_TextureIndex;
layout(location = 4) in float a_TilingScale;

uniform mat4 u_ViewProjection;

out vec2 v_TextureCoord;
out float v_TextureIndex;
out vec4 v_Color;
out float v_TilingScale;

void main()
{
	v_Color = a_Color;
	v_TextureCoord = a_TextureCoord;
	v_TextureIndex = a_TextureIndex;
	v_TilingScale = a_TilingScale;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}


#type fragment
#version 330 core
		
layout(location = 0) out vec4 color;

in vec2 v_TextureCoord;
in float v_TextureIndex;
in vec4 v_Color;
in float v_TilingScale;

uniform sampler2D u_Textures[32];

void main()
{
	// TODO: u_TilingScale
	vec4 textureColor = v_Color;
	switch(int(v_TextureIndex))
	{
		case 0: textureColor *= texture(u_Textures[0], v_TextureCoord * v_TilingScale); break;
		case 1: textureColor *= texture(u_Textures[1], v_TextureCoord * v_TilingScale); break;
		case 2: textureColor *= texture(u_Textures[2], v_TextureCoord * v_TilingScale); break;
		case 3: textureColor *= texture(u_Textures[3], v_TextureCoord * v_TilingScale); break;
		case 5: textureColor *= texture(u_Textures[5], v_TextureCoord * v_TilingScale); break;
		case 6: textureColor *= texture(u_Textures[6], v_TextureCoord * v_TilingScale); break;
		case 7: textureColor *= texture(u_Textures[7], v_TextureCoord * v_TilingScale); break;
		case 8: textureColor *= texture(u_Textures[8], v_TextureCoord * v_TilingScale); break;
		case 9: textureColor *= texture(u_Textures[9], v_TextureCoord * v_TilingScale); break;
		case 10: textureColor *= texture(u_Textures[10], v_TextureCoord * v_TilingScale); break;
		case 11: textureColor *= texture(u_Textures[11], v_TextureCoord * v_TilingScale); break;
		case 12: textureColor *= texture(u_Textures[12], v_TextureCoord * v_TilingScale); break;
		case 13: textureColor *= texture(u_Textures[13], v_TextureCoord * v_TilingScale); break;
		case 14: textureColor *= texture(u_Textures[14], v_TextureCoord * v_TilingScale); break;
		case 15: textureColor *= texture(u_Textures[15], v_TextureCoord * v_TilingScale); break;
		case 16: textureColor *= texture(u_Textures[16], v_TextureCoord * v_TilingScale); break;
		case 17: textureColor *= texture(u_Textures[17], v_TextureCoord * v_TilingScale); break;
		case 18: textureColor *= texture(u_Textures[18], v_TextureCoord * v_TilingScale); break;
		case 19: textureColor *= texture(u_Textures[19], v_TextureCoord * v_TilingScale); break;
		case 20: textureColor *= texture(u_Textures[20], v_TextureCoord * v_TilingScale); break;
		case 21: textureColor *= texture(u_Textures[21], v_TextureCoord * v_TilingScale); break;
		case 22: textureColor *= texture(u_Textures[22], v_TextureCoord * v_TilingScale); break;
		case 23: textureColor *= texture(u_Textures[23], v_TextureCoord * v_TilingScale); break;
		case 24: textureColor *= texture(u_Textures[24], v_TextureCoord * v_TilingScale); break;
		case 25: textureColor *= texture(u_Textures[25], v_TextureCoord * v_TilingScale); break;
		case 26: textureColor *= texture(u_Textures[26], v_TextureCoord * v_TilingScale); break;
		case 27: textureColor *= texture(u_Textures[27], v_TextureCoord * v_TilingScale); break;
		case 28: textureColor *= texture(u_Textures[28], v_TextureCoord * v_TilingScale); break;
		case 29: textureColor *= texture(u_Textures[29], v_TextureCoord * v_TilingScale); break;
		case 30: textureColor *= texture(u_Textures[30], v_TextureCoord * v_TilingScale); break;
		case 31: textureColor *= texture(u_Textures[31], v_TextureCoord * v_TilingScale); break;
	}
	color = textureColor;
}
