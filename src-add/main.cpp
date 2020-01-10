//
// Created by Albert Xu on 1/8/20.
//

#include <iostream>
#include "pbrt.h"
#include "api.h"
#include "parser.h"
#include "parallel.h"

#include "memory.h"
#include "paramset.h"

template<typename X>
using u_ptr = std::unique_ptr<X[]>;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Need a filename!\n";
        exit(1);
    }

    std::string out_name("img.png");
    std::cout << "Rendering <" << argv[1] << "> to " << out_name << std::endl;

    FLAGS_stderrthreshold = 1; // Warning and above.
    pbrt::Options options;
//    options.imageFile = out_name;
    pbrt::pbrtInit(options);

    // ParseFile parses killeroo-simple.pbrt and creates a world.
//    pbrt::pbrtParseFile(argv[1]);

    // I'm going to break it down here:
    // LookAt 400 10 30   0 63 -110   0 0 1
    pbrt::pbrtLookAt(400, 10, 30, 0, 63, -110, 0, 0, 1);

    // Rotate -5 0 0 1
    pbrt::pbrtRotate(-5, 0, 0, 1);

    // Camera "perspective" "float fov" [39]
    {
        pbrt::ParamSet ps;
        ps.AddFloat("fov", u_ptr<float>(new float[1]{39}), 1);
        pbrt::pbrtCamera("perspective", ps);
    }

    // Film "image" "integer xresolution" [700] "integer yresolution" [700]
    //    "string filename" "killeroo-simple.exr"
    {
        pbrt::ParamSet ps;
        ps.AddInt("xresolution", u_ptr<int>(new int[1]{700}), 1);
        ps.AddInt("yresolution", u_ptr<int>(new int[1]{700}), 1);
        ps.AddString("filename", u_ptr<std::string>(new std::string[1]{"img.png"}), 1);
        pbrt::pbrtFilm("image", ps);
    }

    // Sampler "halton" "integer pixelsamples" [8]
    {
        pbrt::ParamSet ps;
        ps.AddInt("pixelsamples", u_ptr<int>(new int[1]{8}), 1);
        pbrt::pbrtSampler("halton", ps);
    }

    // Integrator "path"
    {
        pbrt::ParamSet ps;
        pbrt::pbrtIntegrator("path", ps);
    }

    //WorldBegin
    pbrt::pbrtWorldBegin();

    //AttributeBegin
    pbrt::pbrtAttributeBegin();

    // Material "matte" "color Kd" [0 0 0]
    {
        pbrt::ParamSet ps;
        ps.AddRGBSpectrum("Kd", u_ptr<float>(new float[3]{0, 0, 0}), 3);
        pbrt::pbrtMaterial("matte", ps);
    }

    //Translate 150 0  20
    //Translate 0 120 0
    pbrt::pbrtTranslate(150, 120, 20);

    // AreaLightSource "area"  "color L" [2000 2000 2000] "integer nsamples" [8]
    {
        pbrt::ParamSet ps;
        ps.AddRGBSpectrum("L", u_ptr<float>(new float[3]{2000, 2000, 2000}), 3);
        ps.AddInt("nsamples", u_ptr<int>(new int[1]{8}), 1);
        pbrt::pbrtAreaLightSource("area", ps);
    }

    // Shape "sphere" "float radius" [3]
    {
        pbrt::ParamSet ps;
        ps.AddFloat("radius", u_ptr<float>(new float[1]{3}), 1);
        pbrt::pbrtShape("sphere", ps);
    }

    //AttributeEnd
    pbrt::pbrtAttributeEnd();

    //AttributeBegin
    pbrt::pbrtAttributeBegin();

    // Material "matte" "color Kd" [.5 .5 .8]
    {
        pbrt::ParamSet ps;
        ps.AddRGBSpectrum("Kd", u_ptr<float>(new float[3]{.5, .5, .8}), 3);
        pbrt::pbrtMaterial("matte", ps);
    }

    //Translate 0 0 -140
    pbrt::pbrtTranslate(0, 0, -140);

    // Shape "trianglemesh" "point P" [ -1000 -1000 0 1000 -1000 0 1000 1000 0 -1000 1000 0 ]
    //      "float uv" [ 0 0 5 0 5 5 0 5 ]
    //	    "integer indices" [ 0 1 2 2 3 0]
    {
        pbrt::ParamSet ps;
        ps.AddPoint3f("P", u_ptr<pbrt::Point3f>(
                new pbrt::Point3f[4]{
                        pbrt::Point3f(-1000, -1000, 0),
                        pbrt::Point3f(1000, -1000, 0),
                        pbrt::Point3f(1000, 1000, 0),
                        pbrt::Point3f(-1000, 1000, 0)
                }), 4);
        ps.AddFloat("uv", u_ptr<float>(new float[8]{0, 0, 5, 0, 5, 5, 0, 5}), 8);
        ps.AddInt("indices", u_ptr<int>(new int[6]{0, 1, 2, 2, 3, 0}), 6);
        pbrt::pbrtShape("trianglemesh", ps);
    }

    // Shape "trianglemesh" "point P" [ -400 -1000 -1000   -400 1000 -1000   -400 1000 1000 -400 -1000 1000 ]
    //      "float uv" [ 0 0 5 0 5 5 0 5 ]
    //        "integer indices" [ 0 1 2 2 3 0]
    {
        pbrt::ParamSet ps;
        ps.AddPoint3f("P", u_ptr<pbrt::Point3f>(
                new pbrt::Point3f[4]{
                        pbrt::Point3f(-400, -1000, -1000),
                        pbrt::Point3f(-400, 1000, -1000),
                        pbrt::Point3f(-400, 1000, 1000),
                        pbrt::Point3f(-400, -1000, 1000)
                }), 4);
        ps.AddFloat("uv", u_ptr<float>(new float[8]{0, 0, 5, 0, 5, 5, 0, 5}), 8);
        ps.AddInt("indices", u_ptr<int>(new int[6]{0, 1, 2, 2, 3, 0}), 6);
        pbrt::pbrtShape("trianglemesh", ps);
    }

    //AttributeEnd
    pbrt::pbrtAttributeEnd();

    //AttributeBegin
    pbrt::pbrtAttributeBegin();

    //Scale .5 .5 .5
    pbrt::pbrtScale(.5, .5, .5);
    //Rotate -60 0 0 1
    pbrt::pbrtRotate(-60, 0, 0, 1);

    // Material "plastic" "color Kd" [.4 .2 .2] "color Ks" [.5 .5 .5]
    //        "float roughness" [.025]
    {
        pbrt::ParamSet ps;
        ps.AddRGBSpectrum("Kd", u_ptr<float>(new float[3]{.4, .2, .2}), 3);
        ps.AddRGBSpectrum("Ks", u_ptr<float>(new float[3]{.5, .5, .5}), 3);
        ps.AddFloat("roughness", u_ptr<float>(new float[1]{.025}), 1);
        pbrt::pbrtMaterial("plastic", ps);
    }

    //Translate 100 200 -140
    pbrt::pbrtTranslate(100, 200, -140);

    // Include "geometry/killeroo.pbrt"
    pbrt::pbrtParseFile("../scenes/geometry/killeroo.pbrt");

    // Material "plastic" "color Ks" [.3 .3 .3] "color Kd" [.4 .5 .4]
    //        "float roughness" [.15]
    {
        pbrt::ParamSet ps;
        ps.AddRGBSpectrum("Kd", u_ptr<float>(new float[3]{.4, .5, .4}), 3);
        ps.AddRGBSpectrum("Ks", u_ptr<float>(new float[3]{.3, .3, .3}), 3);
        ps.AddFloat("roughness", u_ptr<float>(new float[1]{.15}), 1);
        pbrt::pbrtMaterial("plastic", ps);
    }

    //Translate -200 0 0
    pbrt::pbrtTranslate(-200, 0, 0);

    // Include "geometry/killeroo.pbrt"
    pbrt::pbrtParseFile("../scenes/geometry/killeroo.pbrt");

    //AttributeEnd
    pbrt::pbrtAttributeEnd();

    //WorldEnd
    pbrt::pbrtWorldEnd();

    pbrt::pbrtCleanup();
    return 0;
}
