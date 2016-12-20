varying float alpha;
uniform float time;
attribute vec3 vertex;
attribute vec3 random;
attribute vec3 velocity;
uniform mat4 mvp;

vec3 calc_pos(vec3 initial, vec3 vel, float time)
{
    vec3 pos = initial + vel * time;
    if (pos.y < -0.99) {
        pos.y = -0.99;
    }
    return pos;
}

vec3 calc_velocity()
{
    float t = fract((time + vertex.x) / (500.0));

    float x = 2.0 * velocity.x + 0.1 * sin(t * t) * random.z;
    float y = 2.0 * velocity.y + 2.0 * abs(random.y) + t;
    float z = 2.0 * velocity.z + 0.1 * cos(t * t) * random.z;

    return vec3(x, y, z);
}

void main(void)
{
    float t = fract((time + vertex.x) / (500.0));

    vec3 pos         = vec3(0.01 * random.x, -0.99, 0.01 * random.z);
    vec3 vel         = calc_velocity();
    vec3 current_pos = calc_pos(pos, vel, t);

    gl_Position = mvp * vec4(current_pos, 1.0);
    alpha = 1.0 - t * t;
}

