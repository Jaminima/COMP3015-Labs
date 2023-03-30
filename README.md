# COMP3015 CW1

***Please ensure you restore packages through NuGet beforee attempting to run the code through VS!***

## System Spec

- Windows 11

- Visual Studio 2022

- C++ 17

## Demo Video

[COMP3015 CW1 - YouTube](https://youtu.be/t2YZadDNy1w)

## Code

My first step when working with this project was to transform the rendering code into OOP. This enabled me to more easily load in, render and configure individual assets. It also resulted in a far more elegant looking code.

On startup the code loads in the assets, on first load assets are dumped out in a more efficiently loadable format. From here the assets data is loaded into the suitable gl objects and then the main loop starts for rendering the scene on repeat.

Inputs are processed in a way such that there is a known time passed from last run. This allows movement steps to be time related.

Textures are loaded into a stack like array where the bottom most texture has the following ones placed on top.

Lights follow a similar array structure, but their results are summed as opposed to overlayed.

## Navigating Code

Main pieces of code can all be found from **<u>scenebasic_uniform.cpp</u>**. This file manages all aspects of the scene. The functions within here will give a good guide for exploring the spin-off procedures and their workings.

Historical shaders are provided in <u>**/shaders**</u> and give a look into the development process.

The asset loading code is mainly located withing <u>**Asset.cpp**</u> and attempts to parse suitable .obj files into memory. In here code related to the loading and exporting of dumps is located.

The dump system takes the data arrays constructed from the parsing of .obj files and puts them into formatted files. Such that the mesh data can be read in a far more rapid time. From tests it has taken loading times from 20~ seconds to <2 seconds, a significant improvement!

## Self Assessed Grade

I believe I have acheived the **<u>50-60</u>** range. As i have satisified the majority of elements in this band, excluding the Skybox which I did not tackle. I do satisfy parts of <u>**60-70**</u> (>1 model & light) however I believe this is only enough to strengthen my mark not push me up a band.
