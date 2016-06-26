#include <iostream>
#include <vector>
// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
typedef OpenMesh::PolyMesh_ArrayKernelT<>  MyMesh;
int main(int argc, char **argv) {
    MyMesh mesh;
    int mode(atoi(argv[1]));
    // read mesh from stdin
    if (!OpenMesh::IO::read_mesh(mesh, argv[3])) {
        std::cerr << "Error: Cannot read mesh from " << argv[3] << std::endl;
        return 1;
    }

    // original
    MyMesh::FaceIter f_it, f_end(mesh.faces_end());
    MyMesh::FaceFaceIter ff_it;
    MyMesh::Normal aon, d0, d1;
    std::vector<MyMesh::Scalar> originals;
    std::vector<MyMesh::Scalar>::iterator original_it;
    originals.reserve(mesh.n_faces());
    MyMesh::Scalar original, lb(-1.0), ub(1.0), count;
    for (f_it = mesh.faces_begin(); f_it != f_end; ++f_it) {
        aon[0] = aon[1] = aon[2] = count = 0.0;
        for (ff_it = mesh.ff_iter(*f_it); ff_it.is_valid(); ++ff_it) {
            aon += mesh.calc_face_normal(*ff_it).normalize();
            ++count;
        }
        d0 = mesh.calc_face_normal(*f_it);
        d0.normalize();
        d1 = aon / count;
        d1.normalize();
        original = std::abs(std::max(lb, std::min(ub, dot(d0, d1))));
        originals.push_back(original);
    }

    // this vector stores the computed centers of gravity
    std::vector<MyMesh::Point> cogs;
    std::vector<MyMesh::Point>::iterator cog_it;
    cogs.reserve(mesh.n_vertices());

    // smoothing mesh argv[2] times
    MyMesh::VertexIter v_it, v_end(mesh.vertices_end());
    MyMesh::VertexVertexIter vv_it;
    MyMesh::Point cog, tmp;
    MyMesh::Scalar valence;
    unsigned int i, N(atoi(argv[2]));
    float lamda = 0.25; // Laplacian smoothing
    float mu = -0.33; // Taubin smoothing
    for (i = 0; i < N; ++i) {
        cogs.clear();
        for (v_it = mesh.vertices_begin(); v_it != v_end; ++v_it) {
            cog[0] = cog[1] = cog[2] = valence = 0.0;

            // iterate over all neighboring vertices
            for (vv_it = mesh.vv_iter(*v_it); vv_it.is_valid(); ++vv_it) {
                cog += mesh.point(*vv_it);
                ++valence;
            }
            tmp = (cog / valence - mesh.point(*v_it)) * lamda + mesh.point(*v_it);
            if (mode == 2)
                tmp = (cog / valence - tmp) * mu + tmp;
            cogs.push_back(tmp);
        }
        for (v_it = mesh.vertices_begin(), cog_it = cogs.begin(); v_it != v_end; ++v_it, ++cog_it)
            if (!mesh.is_boundary(*v_it))
                mesh.set_point(*v_it, *cog_it);
    }

    // set face color
    MyMesh::Scalar weight, weight0;
    int r = 0, o = 0, y = 0, g = 0;
    int r0 = 0, o0 = 0, y0 = 0, g0 = 0;

    for (f_it = mesh.faces_begin(), original_it = originals.begin(); f_it != f_end; ++f_it, ++original_it) {
        aon[0] = aon[1] = aon[2] = valence = 0.0;
        for (ff_it = mesh.ff_iter(*f_it); ff_it.is_valid(); ++ff_it) {
            tmp = mesh.calc_face_normal(*ff_it);
            tmp.normalize();
            aon += tmp;
            ++valence;
        }
        d0 = mesh.calc_face_normal(*f_it);
        d0.normalize();
        d1 = aon / valence;
        d1.normalize();
        weight = std::abs(std::max(lb, std::min(ub, dot(d0, d1))));
        weight0 = *original_it;
        if (weight < 0.4) {
            r++;
            //if (mesh.is_valid_handle(*f_it))
            //    mesh.set_color(*f_it, MyMesh::Color(255, 0, 0));
        }
        if ((weight >= 0.4) && (weight < 0.7)) {
            o++;
            //if (mesh.is_valid_handle(*f_it))
            //    mesh.set_color(*f_it, MyMesh::Color(255, 165, 0));
        }
        if ((weight >= 0.7) && (weight < 0.9)) {
            y++;
            //if (mesh.is_valid_handle(*f_it))
            //    mesh.set_color(*f_it, MyMesh::Color(255, 255, 0));
        }
        if ((weight >= 0.9) && (weight <= 1)) {
            g++;
            //if (mesh.is_valid_handle(*f_it))
            //    mesh.set_color(*f_it, MyMesh::Color(0, 255, 0));
        }
        if (weight0 < 0.4)
            r0++;
        if ((weight0 >= 0.4) && (weight0 < 0.7))
            o0++;
        if ((weight0 >= 0.7) && (weight0 < 0.9))
            y0++;
        if ((weight0 >= 0.9) && (weight0 <= 1))
            g0++;
    }

    // write mesh to stdout
    if (!OpenMesh::IO::write_mesh(mesh, argv[4])) {
        std::cerr << "Error: cannot write mesh to " << argv[4] << std::endl;
        return 1;
    }
    std::cout << "Before smoothing:" << std::endl;
    std::cout << "    Red: " << r0 << std::endl;
    std::cout << "    Orange: " << o0 << std::endl;
    std::cout << "    Yellow: " << y0 << std::endl;
    std::cout << "    Green: " << g0 << std::endl;
    if (mode == 1)
        std::cout << "After Laplacian smoothing:" << std::endl;
    if (mode == 2)
        std::cout << "After Taubin smoothing:" << std::endl;
    std::cout << "    Red: " << r << std::endl;
    std::cout << "    Orange: " << o << std::endl;
    std::cout << "    Yellow: " << y << std::endl;
    std::cout << "    Green: " << g << std::endl;
    return 0;
}