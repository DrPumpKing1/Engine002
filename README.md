# Engine002
OpenGL Graphics Engine

## Build Container
### Setup
#### Build Image
docker build -t engine002-builder .

#### Create Container
docker create --name engine002-build \
    -v $(pwd):/workspace \
    -v ~/engine002-builds:/output \
    engine002-builder
    
### How to use
#### Start
docker start engine002-build

#### Build Release
docker exec -e BUILD_TYPE=release engine002-build build.sh

#### Build Debug
docker exec -e BUILD_TYPE=debug engine002-build build.sh

#### Stop
docker stop engine002-build
