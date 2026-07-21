# Over boring & useless stuff - Project is in progress

Over is a handmade engine using OpenGL with some examples.
It's a mix of the learnopengl tutorial & other courses.
Also it is a playground for me to test architecture of render engine & algorithms.

# Examples

## Basics

- model-view-projection matrix
- texture loading
- vao/vbo/ibo
- raw main function

<img width="1940" height="1156" alt="image" src="https://github.com/user-attachments/assets/afbf35c5-28e5-4f97-9cbc-d67b9f11121d" />

## Lighting

- Mutiple textures with different lighting rules
- Directional light
- Point light
- Spot light
- Normal map

### Earlier version

<img width="3839" height="2085" alt="image" src="https://github.com/user-attachments/assets/2452d15e-5522-40c5-bf2f-dd5c7cd43843" />

### Latest version

<img width="3839" height="2088" alt="image" src="https://github.com/user-attachments/assets/7a660b54-2578-40c7-b2cf-c8330f44137b" />

## Fractal

- Maldebrot fractal
- ImGUI interface: iterations, color, power

<img width="2138" height="1365" alt="image" src="https://github.com/user-attachments/assets/8efef666-3c49-4ec0-a919-1b4a19d037f8" />

## Models

- Model loading using assimp
- Outline (light-green)

<img width="2134" height="1663" alt="image" src="https://github.com/user-attachments/assets/c488b98e-f7c2-4237-861e-1db14fd967c8" />

## Morphing

- It's model, actually
- Morphing function (are you surprised?)
- Using custom App singleton
- ImGUI interface (you can see it on picture)
- Free cursor using `T` key.

<img width="1922" height="1075" alt="image" src="https://github.com/user-attachments/assets/840a465b-84e5-4ce8-a163-e7f7ab462d31" />

## Features

- Using engine package (module, library? Does it matter?)
- FrameBuffer + RenderBuffer + Texture2D = Post-processing
- => Kernel matrix in shader
- MSAA x4 is used (not for post-processing)
- Hold `Left-Shift` to change view mode
- Hold `Left-Control` to slow down camera
- Hold `Q` to change refraction to reflection
- Hold `Tab` to see normal vectors
- Hold `E` to blow up model (avg normal vectors of primitives are taken). Enter `R` to restore.
- Use `Y`, `H`, `G`, `J`, `B` and `N` to rotate model

<img width="1123" height="954" alt="image" src="https://github.com/user-attachments/assets/4bddcc44-4c68-4e57-9b0a-29db2b45c7f4" />

### With skybox (and "kernel view" on)
<img width="1978" height="954" alt="image" src="https://github.com/user-attachments/assets/6519b67e-6b66-4054-ac90-877d51759671" />

### Environment mapping

#### Refraction
<img width="2034" height="1915" alt="image" src="https://github.com/user-attachments/assets/bb05ee20-c200-41d3-9f7c-8569d7a92d1f" />

#### Reflection
<img width="3336" height="1738" alt="image" src="https://github.com/user-attachments/assets/71111d44-48b5-4929-b46b-2ebc401d9f53" />
