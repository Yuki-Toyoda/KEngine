struct Particle
{
    float32_t3  translate;
    float32_t3  scale;
    float32_t3  velocity;
    float32_t   currentTime;
    float32_t   lifeTime;
    float32_t4  color;
};

struct EmitterSphere
{
    float32_t3  translate;
    float32_t   radius;
    int32_t     count;
    float32_t   frequency;
    float32_t   frequencyTime;
    int32_t     emit;
};