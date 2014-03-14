#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
 
flat in int layerInstance[3];
out float layer;
 
void main()
{
    gl_Layer = layerInstance[0];
    layer = float(gl_Layer) + 0.5;

    for (int i = 0; i < gl_in.length(); i++)
    {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}