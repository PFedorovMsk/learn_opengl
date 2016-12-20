in vec3 fragColor;
out vec4 outColor;

void main(void)
{
    outColor = vec4(fragColor.x, fragColor.y, fragColor.z, 1.0);
}
