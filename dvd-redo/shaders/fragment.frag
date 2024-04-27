#version 330 core
out vec4 FragColor;

in vec3 our_color;
in vec2 image_tex_coords;

uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1, image_tex_coords) * vec4(our_color, 1.0);
}
