@echo off

glslc -fshader-stage=vertex -finvert-y triangle.vert.glsl -o triangle.vert.spv
glslc -fshader-stage=fragment -finvert-y triangle.frag.glsl -o triangle.frag.spv