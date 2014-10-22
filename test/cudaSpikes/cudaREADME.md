cudaREADME.md

extension:
    c files that contain cuda code should be saved as file.cu

to compile:
    nvcc file.cu -o exeName
or
    nvcc file.cu -arch=sm_20 -o exeName (compiles for sm_20 architecture, removing some of the low compute warnings)