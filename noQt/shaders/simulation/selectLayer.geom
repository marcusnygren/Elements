#version 330
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_shading_language_420pack : enable

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
 
flat in int layerInstance[3];
flat out float layer;
 
void main()
{
    gl_Layer = layerInstance[0];
    layer = gl_Layer;
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive();
}