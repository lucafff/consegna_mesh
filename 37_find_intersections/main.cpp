#include <cinolib/find_intersections.h>
#include <cinolib/gl/glcanvas.h>
#include <cinolib/export_hexahedra.h>
#include <cinolib/gl/surface_mesh_controls.h>
#include <cinolib/gl/volume_mesh_controls.h>
#include <cinolib/meshes/meshes.h>
#include <cinolib/profiler.h>

#define DATA_PATH "/home/luca/Documenti/cinolib/examples/data"

int main(int argc, char **argv) {

// richiamo della mesh esaedrica che si desidera usare (si consigliano solo rockerarm e ellipsoid in quanto uniche esaedriche)
  std::string path = std::string(DATA_PATH) + "/rockerarm.mesh";

  cinolib::DrawableHexmesh<> hm(path.c_str());
  // definizione della matrice di rotazione
  cinolib::mat3d R = cinolib::mat3d::ROT_3D(cinolib::vec3d(0, 1, 0), cinolib::to_rad(90.0));

// tutti i vertici della mesh vengono ruotati e aggiunti alla stessa cosi iniziando il processo di duplicazione
  auto N = hm.num_verts();
  for (uint vid = 0, n = hm.num_verts(); vid < n; ++vid)
  {
    hm.vert_add(R * hm.vert(vid));
  }
  
// per tutti gli esaedri
  for (uint pid = 0, n = hm.num_polys(); pid < n; ++pid)
  {
  // salva i vertici dell'esaedro pid in verts
    auto verts = hm.poly_verts_id(pid);

// corregge l'id sommando il numero dei vertici totali (della singola mesh)
    for (auto &v : verts)
    {
      v += N;
    }
// prende 8 vertici da verts per formare un esaedro e aggiungerlo alla nuova mesh, il for itera il processo per tutti gli esaedri
    auto t = cinolib::polys_from_serialized_vids(verts, 8);
    for (auto hex : t)
    {
      hm.poly_add(hex);
    }
  }
  
  cinolib::Profiler p;
  p.push("Find intersections");
  // dichiarazione di tre variabili dove verranno salvati gli id dei vertici, le coppie di triangoli che si intersecano e il numero totale di esaedri della mesh
  std::vector<uint> tris;
  std::set<cinolib::ipair> intersections;
  auto num_hexahedra = hm.num_polys();
 // stesura del pattern per la scomposizione dei vertici di un esaedro in un insieme di triangoli
  constexpr const uint tris_ids[12][3] = {
    {0, 1, 2},
    {0, 2, 3},
    {0, 1, 5},
    {0, 5, 4},
    {1, 2, 6},
    {1, 6, 5},
    {4, 5, 6},
    {4, 6, 7},
    {0, 3, 7},
    {0, 7, 4},
    {2, 3, 7},
    {2, 7, 6}
  };
  // sequela di for per la scomposizione degli esaedri, vengono presi uno per uno scomposti in 36 vertici (12 triangoli * 3 vertici) e salvati nella variabile tris
  for (uint pid = 0; pid < num_hexahedra; ++pid) {
    auto verts = hm.poly_verts_id(pid);

    for(const auto& tid: tris_ids)
    {
      for(const auto vid: tid)
      {
        tris.push_back(verts[vid]);
      }
    }
  }
// viene richiamata la funzione find_intersections passando il vettore ricavato sopra (tris) e il vettore dei vertici in forma vec3d
  find_intersections(hm.vector_verts(), tris, intersections);
  p.pop();

//infine in intersections vengono salvate tutti i triangoli che hanno delle intersezioni, quindi si risale al esaedro di provenienza tramite la divisione per 12 (cosa che ci è permessa grazie
// alla scomposizione fatta poco fa, un esaedro equivale a 12 triangoli quindi supponendo di aver identificato tra i tanti il triangolo numero 14, dividendolo per 12 sappiamo che l'esaedro di 
// origine è il numero  1), quindi gli esaedri trovati vengono marchiati e colorati di rosso.
  std::cout << "\n"
            << intersections.size() << " pairs of intersecting triangles were found\n"
            << std::endl;
  for (const auto &i : intersections) {
    hm.poly_data(i.first / 12).color = cinolib::Color::RED();
    hm.poly_data(i.second / 12).color = cinolib::Color::RED();

    hm.poly_data(i.first / 12).flags[cinolib::MARKED] = true;
    hm.poly_data(i.second / 12).flags[cinolib::MARKED] = true;
  }
  hm.updateGL();

  cinolib::GLcanvas gui;
  cinolib::VolumeMeshControls<cinolib::DrawableHexmesh<>> menu(&hm, &gui);
  gui.push(&hm);
  gui.push(&menu);
  return gui.launch();
}
