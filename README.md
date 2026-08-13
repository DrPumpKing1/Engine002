# Engine002
OpenGL Graphics Engine

## Build Container
### Setup
#### Build Image
docker build -t engine002-builder .

#### Create Container
docker create --name engine002-build \
    -v $(pwd):/local-src \
    -v ~/Builds:/output \
    engine002-builder
    
### How to use
#### Start
docker start engine002-build

#### Build Release
docker exec -e BUILD_TYPE=Release engine002-build build.sh

#### Build Debug
docker exec -e BUILD_TYPE=Debug engine002-build build.sh

#### Stop
docker stop engine002-build
