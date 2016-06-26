#include <iostream>

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

using namespace OpenMesh;
using namespace std;
typedef PolyMesh_ArrayKernelT<>  MyMesh;

int main() {
    MyMesh original, mesh;

    // read original model
    original.request_vertex_normals();
    if (!original.has_vertex_normals()) {
        cerr << "Error: Standard vertex property 'Normals' not available!\n";
        return 1;
    }
    IO::Options opt;
    opt += IO::Options::VertexColor;
    const char* filename = "off/23.off";
    if (!OpenMesh::IO::read_mesh(original, filename, opt)) {
        cerr << "Error: Cannot read mesh from " << filename << endl;
        return 1;
    }
    if (!opt.check(IO::Options::VertexNormal)) {
        original.request_face_normals();
        original.update_normals();
        original.release_face_normals();
    }

    // copy original to mesh
    // copy vertex
    for (MyMesh::VertexIter vit = original.vertices_begin(); vit != original.vertices_end(); ++vit)
        mesh.add_vertex(original.point(*vit));
    // copy face
    vector<MyMesh::VertexHandle> fvh;
    for (MyMesh::FaceIter fit = original.faces_begin(); fit != original.faces_end(); ++fit) {
        fvh.clear();
        for (MyMesh::FaceVertexIter fvit = original.fv_begin(*fit); fvit != original.fv_end(*fit); ++fvit)
            fvh.push_back(*fvit);
        mesh.add_face(fvh);
    }
    if (!IO::write_mesh(mesh, "output.off")) {
        cerr << "Cannot write mesh to file 'output.off'\n";
        return 1;
    }
    return 0;
}