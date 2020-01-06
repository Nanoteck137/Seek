#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_TextureID;

uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TextureID;

void main()
{
    v_TexCoord = a_TexCoord;
    v_Color = a_Color;
    v_TextureID = a_TextureID;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TextureID;

uniform sampler2D u_Textures[32];

void main()
{
    vec4 texColor = v_Color;
    if (v_TextureID > 0.0)
    {
        int tid = int(v_TextureID - 0.5);
        texColor = v_Color * texture(u_Textures[tid], v_TexCoord);
    }

    color = texColor;
}