# 3DEngine
A small project, testing nvidia physx & ECS

You can find the doxygen documentation inside Project.Engine/Documentation/html and Project.Game/Documentation/html

## Linux
### Requirements
#### Debian-based systems:
* Install needed tools and libraries:
    ```
    sudo apt install cmake libglfw3 libglfw3-dev libglew-dev libglew2.1 libboost-all-dev
    ```
* Install cuda from [here](https://developer.nvidia.com/cuda-downloads)

#### RedHat-based systems:
* Install needed tools and libraries:
    ```
    sudo dnf install cmake glfw glfw-devel glew glew-devel boost-devel
    ```
* Install cuda from [here](https://developer.nvidia.com/cuda-downloads)

### Compiling
```
sudo cp libraries/libPhysXGpu_64.so /usr/lib64/
chmod +x build.sh
./build.sh -c -b=<build-folder>
```

### Running
```
cd bin/
./game
```