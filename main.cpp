#include "helper/scene.h"
#include "helper/scenerunner.h"
#include "scenebasic_uniform.h"

int main(int argc, char* argv[])
{
	printf("Basic Controls:\nWASD ZX - Movement\nMouse - Look\nT - Toggle Toon\nT + (Shift/Ctrl) - Change Toon Bands\nALT + _ - Toggle\n	A - Ambience\n	S - Specular\n	D - Diffuse\n	G - Global\n	E - Sobel Edge\n	B - Gaussian Blur\n	I - Disintergration\nEscape - Quit\n\nPress Enter To Continue");

	cin.get();

	SceneRunner runner("Shader_Basics");

	std::unique_ptr<Scene> scene;

	scene = std::unique_ptr<Scene>(new SceneBasic_Uniform());

	return runner.run(*scene);
}