#include <cinolib/export_hexahedra.h>
#include <cinolib/find_intersections.h>
#include <cinolib/gl/glcanvas.h>
#include <cinolib/gl/surface_mesh_controls.h>
#include <cinolib/gl/volume_mesh_controls.h>
#include <cinolib/meshes/meshes.h>
#include <cinolib/profiler.h>

int main(int argc, char **argv)
{
  using namespace cinolib;

  std::string path = (argc == 2) ? std::string(argv[1]) : "/home/luca/Documenti/cinolib/examples/data/rockerarm.mesh";

  Hexmesh<> hm(path.c_str());

  mat3d R = mat3d::ROT_3D(vec3d(0, 1, 0), to_rad(90.0));

  auto N = hm.num_verts();
  for (uint vid = 0, n = hm.num_verts(); vid < n; ++vid)
  {
    hm.vert_add(R * hm.vert(vid));
  }

  for (uint pid = 0, n = hm.num_polys(); pid < n; ++pid)  
  {
    auto verts = hm.poly_verts_id(pid);   

    for (auto &v : verts)
    {
      v += N;      
    }

    auto t = polys_from_serialized_vids(verts, 8);   
    for (auto hex : t)
    {
      uint id = hm.poly_add(hex);                  
    }
  }
  hm.save("/home/luca/Documenti/cinolib/examples/data/mesh3.mesh");
  return 0;
}
