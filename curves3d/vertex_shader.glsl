attribute vec3 point;
uniform mat4 mvp;
//attribute vec4 color;
//out vec3 fragColor;

void main(void)
{
    gl_Position = mvp * vec4(point, 1.0);
//    fragColor   = color;
} 
