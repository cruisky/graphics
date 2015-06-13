#pragma warning(disable: 4244)
#pragma warning(disable: 4305)

#include "Matrix.h"
#include "Vector.h"
#include "stdafx.h"
#include "Core/transform.h"
#include "Color.h"
#include "Image.h"
#include "RNG.h"
#include "Tools.h"
#include "Core/Sampler.h"
#include "Core/Sample.h"
#include "Core/Config.h"
#include "Core/BSDF.h"
#include "Core/Film.h"
#include "Core/Renderer.h"
#include "Core/Tracer.h"
#include "Core/Scene.h"
#include "Core/Camera.h"
#include "Core/Primitive.h"
#include "Core/Light.h"
#include "Lights/DirectionalLight.h"
#include "Lights/PointLight.h"

#include "Samplers/RandomSampler.h"
#include "Application/GUIViewer.h"

#include <iostream>
#include <string>

using namespace std;
using namespace Cruisky;
using namespace Cruisky::RayTracer;

// NOTE: Rotate -> Translate -> Scale
void GUI(){
	shared_ptr<Film> film(new Film);
	shared_ptr<Scene> scene(new Scene);

	/////////////////////////////////////////////
	// Materials
	shared_ptr<BSDF> diffuse_blue(new Diffuse(Color(0.2, 0.5, 1)));
	shared_ptr<BSDF> diffuse_red(new Diffuse(Color(1, 0.3, 0.3)));
	shared_ptr<BSDF> diffuse_green(new Diffuse(Color(0.2, 0.7, 0.2)));
	shared_ptr<BSDF> diffuse_yellow(new Diffuse(Color(0.9, 0.8, 0.3)));
	shared_ptr<BSDF> diffuse_orange(new Diffuse(Color(0.7, 0.4, 0.1)));
	shared_ptr<BSDF> diffuse(new Diffuse);
	//shared_ptr<BSDF> yellow(new Specular(Color(0.1, 0.1, 0), Color(0.8, 0.7, 0.2), Color(1), 100));
	//shared_ptr<BSDF> red(new Specular(Color(0.1, 0, 0), Color(0.9, 0.3, 0.3), Color(1), 100));
	//shared_ptr<BSDF> green(new Specular(Color(0, 0.1, 0), Color(0, 0.9, 0.4), Color(1), 100));
	//shared_ptr<BSDF> blue(new Specular(Color(0, 0, 0.1), Color(0, 0.4, 0.9), Color(1), 100));
	shared_ptr<BSDF> mirror(new Mirror);
	shared_ptr<BSDF> glass(new Dielectric(Color::WHITE));
	shared_ptr<BSDF> greenglass(new Dielectric(Color::GREEN));
	shared_ptr<BSDF> redglass(new Dielectric(Color(0.7, 0.2, 0.2)));

	///////////////////////////////////////////
	// Shapes & Primitives
	shared_ptr<UnitSphere> sphere(new UnitSphere);
	shared_ptr<UnitPlane> plane(new UnitPlane);
		
	int wall_size = 15;
	shared_ptr<Primitive> w_bottom(new Primitive(plane, diffuse));
	w_bottom->transform.Scale(wall_size, wall_size, 1);
	shared_ptr<Primitive> w_forward(new Primitive(plane, diffuse));
	w_forward->transform.Rotate(90, Vector3::X);
	w_forward->transform.Translate(0, 0, -wall_size/2.f);
	w_forward->transform.Scale(wall_size, wall_size, 1);
	shared_ptr<Primitive> w_back(new Primitive(plane, diffuse));
	w_back->transform.Rotate(-90, Vector3::X);
	w_back->transform.Translate(0, 0, -wall_size/2.f);
	w_back->transform.Scale(wall_size, wall_size, 1);
	shared_ptr<Primitive> w_left(new Primitive(plane, diffuse_yellow));
	w_left->transform.Rotate(90, Vector3::Y);
	w_left->transform.Translate(0, 0, -wall_size/2.f);
	w_left->transform.Scale(wall_size, wall_size, 1);
	shared_ptr<Primitive> w_right(new Primitive(plane, diffuse_green));
	w_right->transform.Rotate(-90, Vector3::Y);
	w_right->transform.Translate(0, 0, -wall_size/2.f);
	w_right->transform.Scale(wall_size, wall_size, 1);
	shared_ptr<Primitive> w_top(new Primitive(plane, diffuse));
	w_top->transform.Rotate(180, Vector3::Y);
	w_top->transform.Translate(0, 0, -wall_size / 2.f);
	w_top->transform.Scale(wall_size, wall_size, 1);

	shared_ptr<Primitive> lamp(new Primitive(plane, diffuse_orange));
	lamp->transform.Rotate(180, Vector3::Y);
	lamp->transform.Translate(0, 0, -wall_size / 2.f);
	lamp->transform.Scale(3, 3, 1);

	shared_ptr<Primitive> ball1(new Primitive(sphere, diffuse_blue));
	ball1->transform.Translate(-3, 0, 1);
	shared_ptr<Primitive> ball2(new Primitive(sphere, diffuse_red));
	ball2->transform.Translate(3, 0, 1);
	shared_ptr<Primitive> ball3(new Primitive(sphere, mirror));
	ball3->transform.Translate(0, 0, 1);

	scene->AddPrimitive(w_bottom);
	scene->AddPrimitive(w_top);
	scene->AddPrimitive(w_forward);
	scene->AddPrimitive(w_back);
	scene->AddPrimitive(w_left);
	scene->AddPrimitive(w_right);
	
	scene->AddPrimitive(lamp);

	scene->AddPrimitive(ball1);
	scene->AddPrimitive(ball2);
	scene->AddPrimitive(ball3);

	//////////////////////////////////////////////////
	// Lights
	shared_ptr<Light> light_main(new PointLight(Color(1), 200, Vector3(0, 0, 5)));
	shared_ptr<Light> light_lamp(new AreaLight(Color(150), lamp.get()));
	
	//scene->AddLight(light_main);
	scene->AddLight(light_lamp);

	int width = 800;
	int height = 600;
	shared_ptr<Camera> camera(new Camera(width, height));
	camera->transform.Rotate(90, 0, -10);
	camera->transform.Translate(0, 3, 5);

	scene->Construct();
	GUIViewer gui(scene, camera, film);
	gui.ConfigRenderer(RendererConfig(TracerType::PathTracing, 2));
	gui.Run();
}

int main(){
	try{
		GUI();
	}
	catch (const std::exception& e){
		fprintf(stderr, "Uncaught Exception: \n\t%s\n", e.what());
	}
}
