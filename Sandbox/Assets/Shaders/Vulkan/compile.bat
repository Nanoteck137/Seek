@echo off

glslc -fshader-stage=vertex triangle.vert.glsl -o triangle.vert.spv
glslc -fshader-stage=fragment triangle.frag.glsl -o triangle.frag.spv