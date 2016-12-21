out vec4 outColor;
uniform int type;

void main(void)
{
    if (type == 0) {
        outColor = vec4(1.0, 0.0, 0.0, 1.0);
    } else if (type == 1) {
        outColor = vec4(0.0, 1.0, 0.0, 1.0);
    } else if (type == 2) {
        outColor = vec4(1.0, 0.5, 0.75, 1.0);
    } else {
        outColor = vec4(0.0, 1.0, 1.0, 1.0);
    }
}
