# install dependencies
apt-get update && apt-get install -y \
    libuv1-dev libssl-dev zlib1g-dev \
    pkg-config build-essential linux-libc-dev \
    cmake git wget unzip curl zip tar

ROOT_DIR=$(pwd)

# ScyllaDB C++ driver
git clone https://github.com/scylladb/cpp-driver.git $ROOT_DIR/lib/cpp-driver && \
    cd $ROOT_DIR/lib/cpp-driver && \
    mkdir -p build && \
    cd build && \
    cmake .. && \
    make

# Oat++ web framework
git clone -b 1.3.0-latest https://github.com/oatpp/oatpp.git $ROOT_DIR/lib/oatpp && \
    cd $ROOT_DIR/lib/oatpp && \
    mkdir -p build && \
    cd build && \
    cmake .. && \
    make install

# Minio C++ SDK
git clone https://github.com/minio/minio-cpp $ROOT_DIR/lib/minio-cpp && \
    cd $ROOT_DIR/lib/minio-cpp && \
    wget --quiet -O vcpkg-master.zip https://github.com/microsoft/vcpkg/archive/refs/heads/master.zip && \
    unzip -qq vcpkg-master.zip && \
    ./vcpkg-master/bootstrap-vcpkg.sh && \
    ./vcpkg-master/vcpkg integrate install && \
    cmake -B ./build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=./vcpkg-master/scripts/buildsystems/vcpkg.cmake && \
    cmake --build ./build --config Debug

# ONNX Runtime
wget https://github.com/microsoft/onnxruntime/releases/download/v1.19.2/onnxruntime-linux-x64-1.19.2.tgz && \
    tar -xvzf onnxruntime-linux-x64-1.19.2.tgz && \
    mv onnxruntime-linux-x64-1.19.2 $ROOT_DIR/lib/onnxruntime && \
    rm -rf onnxruntime-linux-x64-1.19.2.tgz

# curlpp
wget https://github.com/jpbarrette/curlpp/archive/refs/tags/v0.8.1.tar.gz && \
    tar -xvf v0.8.1.tar.gz && rm v0.8.1.tar.gz && \
    mv curlpp-0.8.1 $ROOT_DIR/lib/curlpp && \
    cd $ROOT_DIR/lib/curlpp && \
    mkdir build && cd build && \
    cmake .. && \
    make

# inih
git clone https://github.com/benhoyt/inih.git $ROOT_DIR/lib/inih

# ONNX Model assets
git clone https://huggingface.co/sentence-transformers/all-MiniLM-L6-v2 $ROOT_DIR/lib/all-MiniLM-L6-v2 && \
    mkdir -p $ROOT_DIR/assets && \
    cp $ROOT_DIR/lib/all-MiniLM-L6-v2/tokenizer.json $ROOT_DIR/assets/tokenizer.json && \
    cp $ROOT_DIR/lib/all-MiniLM-L6-v2/onnx/model.onnx $ROOT_DIR/assets/model.onnx && \
    rm -rf $ROOT_DIR/lib/all-MiniLM-L6-v2