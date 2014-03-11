#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
 
flat in int layerInstance[3];
out float layer;
 
void main()
{
    gl_Layer = layerInstance[0];
    layer = float(gl_Layer) + 0.5; // Fixning drift in z

    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive();
}