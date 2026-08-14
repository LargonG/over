# OVER project

Over is...
- Engine (core/engine)
- Catalog of rendering examples
- CV
- "Anthology" of rendering projects (mine, all of them)
- [OpenGL](https://learnopengl.com) tutorial realization
- Render test environment

## Over core

First implementation of over "core" module. `engine` is the addition for `core` part.

Using Wrapper-Target-View pattern for OpenGL objects.
- Wrapper - who owns?
- Target - to what target?
- View - what to do?

### Used in

#### Atmosphere

[Atmosphere scattering algorithm from gpu gems 2](https://developer.nvidia.com/gpugems/gpugems2/part-ii-shading-lighting-and-shadows/chapter-16-accurate-atmospheric-scattering)

_Current state:_
- [bug] Light on opposite site of planet
- [bug] Non-optimal constants

<img width="963" height="866" alt="Снимок экрана 2026-08-14 212615" src="https://github.com/user-attachments/assets/1287ef0c-d2c1-424a-8564-0b418dae8753" />

_For more information see [`atmosphere`](atmosphere)_

#### Features

_Tested use of (or implemented)_
- FrameBuffer: Texture, RenderBuffer
- Draw quad on full screen (off-screen shading)
- MSAA x4
- Gamma correction

_Controls_:
- Hold `Left-Shift` to change view mode
- Hold `Left-Control` to slow down camera
- Hold `Q` to change refraction to reflection
- Hold `Tab` to see normal vectors
- Hold `E` to blow up model (avg normal vectors of primitives are taken). Enter `R` to restore.
- Use `Y`, `H`, `G`, `J`, `B` and `N` to rotate model

<img width="2411" height="1412" alt="image" src="https://github.com/user-attachments/assets/87c86b73-4e26-40af-a30f-1ea8f29971bc" />

_For more information see [`features`](features)_

#### Other

* Basics - first try of [OpenGL](learnopengl.com) tutorial
* Lighting - lighting chapter
* Fractal - first task on CS courses
* Models - model loading chapter
* Morphing - second task on CS courses

_Using_:
* ImGUI interface - `fractal` & `morphing`
* Phong light model - `morphing` & `lighting`
* mvp - all of them, actually, except `fractal`
* Interpolation function in `morphing`
* Model loading in `models` & `morphing`

<img width="2138" height="1365" alt="image" src="https://github.com/user-attachments/assets/8efef666-3c49-4ec0-a919-1b4a19d037f8" />

