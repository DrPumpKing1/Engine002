#version 460 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
} fs_in;

uniform sampler2D image;

void main() {
    FragColor = texture(image, fs_in.TexCoords);
}

