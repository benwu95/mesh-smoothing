Mesh Smoothing with OpenMesh
============================

Smoothing Algorithm
-------------------

* Laplacian Smoothing
    * lamda = 0.25
* Taubin Smoothing
    * lamda = 0.5
    * mu = -0.67

Prepare
-------

> [Compiling OpenMesh (OpenMesh Documentation)](http://www.openmesh.org/media/Documentations/OpenMesh-Doc-Latest/a00030.html)

1. Download OpenMesh via website or git
    * static version: [OpenMesh Download Page](http://www.openmesh.org/download/)
    * development version: `git clone https://www.graphics.rwth-aachen.de:9000/OpenMesh/OpenMesh.git`

2. Unzip and build
    1. Unzip
    2. `cd OpenMesh-6.1`
    3. `mkdir build`
    4. `cd build`
    5. debug version: `cmake .. -DCMAKE_BUILD_TYPE=Debug`  
       release version: `cmake .. -DCMAKE_BUILD_TYPE=Release`
    6. `sudo make install`

Compile
-------

just `make`  
> if error while loading shared libraries  
> add "/usr/local/lib" in /etc/ld.so.conf  
> `sudo ldconfig`

Run
---

Laplacian Smoothing = 1  
Taubin Smoothing = 2  
* `./smoothEx <algorithm> <times> <input obj file> <output obj file>`
* example of Laplacian Smoothing: `./smoothEx 1 50 input.obj output.obj`
* example of Taubin Smoothing: `./smoothEx 2 50 input.obj output.obj`

Output
------

* filename.obj: only vertex and face
* filename_n.obj: add vertex normal