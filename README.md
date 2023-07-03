# OF-PointCloud-To-3D-Model

The goal of this project is to develop an efficient and robust algorithm for 3D surface reconstruction from large-scale point cloud data. Point cloud data represents a set of 3D points obtained through dataset of Semantic3D (https://www.semantic3d.net/view_dbase.php?chl=1). The resulting system will aim to handle large-scale point cloud datasets efficiently while maintaining high reconstruction quality and processing time.

This surface reconstruction process involves two main steps: point cloud preprocessing and surface generation. The point cloud preprocessing stage aims to remove noise, outliers, and redundant points while preserving important geometric features. The surface generation phase involves the construction of a continuous surface mesh that faithfully represents the underlying geometry of the object or environment. In this project we will compare the quality and time reconstructing model in 3 levels: low, medium and high poly.

## Point Cloud Preprocessing

The dataset we are using in this project has more than 20 million points for 1 model. Therefore, the program clean the unnecessary noise by rounding to 1, .5 and .1 unit (corresponding to low, medium and high poly) and avoid duplication in data. The preprocessed data is written into a new text file which could be reused again.

## Surface Reconstruction

The program read in data from a file, then process to create triangular mesh by adding indices. Since the file has been preprocessed in earlier stage with rounding to exact decimal place, the algorithm can based on that to check the distance between points to see if they form a triangular mesh.

## Optimization

Besides quality, time is also one of the main considering factor. In order to speed up the process, the program uses multi threading to reconstruct triangular mesh. Based on the condition of our hardware, we use 5 threads to process the reconstruction at the same time. 

## Analysis and Results

**Low Poly Model**

- Number of vertices: 8,471
- Reconstruction time: ~5 seconds
- PLY file size: 1,228 Kb
- Advantage: Fast reconstruction time
- Disadvanatge: Low quality poly, unable to reconstruct small details

![frontview_low_poly](https://github.com/DarlenePham/OF-PointCloud-To-3D-Model/assets/57384176/ed80edb7-3a3e-44a9-bce5-72579bd79078)

**Medium Poly Model**

- Number of vertices: 36,811
= Reconstruction time: ~30 seconds
- PLY file size: 9,755 Kb
- Advantage: Fast reconstruction time, able to reconstruct small objects like umbrellas
- Disadvanatge: Still have rough looking, messy coloring when zoom in

![frontview_medium_poly](https://github.com/DarlenePham/OF-PointCloud-To-3D-Model/assets/57384176/8b2cf8e7-f925-406f-ba37-f966b165652d)

**High Poly Model**

- Number of vertices: 345,534
- Reconstruction time: 30-40 minutes
- PLY file size: 107,331 Kb
- Advantage: Blended coloring, smooth reconstruction on curve and small details
- Disadvanatge: Slow reconstruction time, unable to reconstruct area with less density of point cloud

![pointview_high_poly](https://github.com/DarlenePham/OF-PointCloud-To-3D-Model/assets/57384176/d0329d75-24e9-4bbd-bd5d-f24b4e83eb14)
