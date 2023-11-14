#include "find_intersections.h"
#include <cinolib/gl/glcanvas.h>
#include <cinolib/gl/volume_mesh_controls.h>
#include <cinolib/meshes/meshes.h>
#include <cinolib/profiler.h>

#define DATA_PATH "/home/luca/Documenti/cinolib/examples/data"

int main(int argc, char **argv) {
  using namespace cinolib;

  std::string path = std::string(DATA_PATH) + "/mesh2.mesh";

  DrawableHexmesh<> hm(path.c_str());

  Profiler p;
  p.push("Find intersections");
  std::set<ipair> inters;
  find_intersections(hm, inters);
  p.pop();

  std::cout << "\n"
            << inters.size() << " pairs of intersecting triangles were found\n"
            << std::endl;
  for (const auto &i : inters) {
    hm.poly_data(i.first / 12).color = Color::RED();
    hm.poly_data(i.second / 12).color = Color::RED();

    hm.poly_data(i.first / 12).flags[MARKED] = true;
    hm.poly_data(i.second / 12).flags[MARKED] = true;
  }
  hm.updateGL();

  GLcanvas gui;
  VolumeMeshControls<DrawableHexmesh<>> menu(&hm, &gui);
  gui.push(&hm);
  gui.push(&menu);
  return gui.launch();
}
