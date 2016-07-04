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

1. Download OpenMesh via website or git
    * static version: [OpenMesh Download Page](http://www.openmesh.org/download/)
    * development version: git clone https://www.graphics.rwth-aachen.de:9000/OpenMesh/OpenMesh.git

2. Unzip and build
    1. Unzip
    2. cd OpenMesh-6.1
    3. mkdir build
    4. cd build
    5. debug version: cmake .. -DCMAKE_BUILD_TYPE=Debug  
       release version: cmake .. -DCMAKE_BUILD_TYPE=Release
    6. sudo make install

Compile
-------

make
    if error while loading shared libraries  
    add "/usr/local/lib" in /etc/ld.so.conf  
    sudo ldconfig