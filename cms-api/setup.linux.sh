# install dependencies
apt-get update && apt-get install -y \
    libuv1-dev libssl-dev zlib1g-dev \
    pkg-config build-essential ninja-build cmake \
    git git-lfs wget unzip curl zip tar python3 \
    libcurlpp-dev libpugixml-dev linux-libc-dev nlohmann-json3-dev 

# install rust
curl https://sh.rustup.rs -sSf | sh -s -- -y
. $HOME/.cargo/env

# Set VCPKG_FORCE_SYSTEM_BINARIES to 1 for architectures arm, s390x, ppc64le and riscv (vcpkg settings)
ARCH=$(uname -m)

case "$ARCH" in
  arm* | aarch64 | s390x | ppc64le | riscv*)
    export VCPKG_FORCE_SYSTEM_BINARIES=1
    echo "VCPKG_FORCE_SYSTEM_BINARIES is set to 1 for architecture: $ARCH"
    ;;
  *)
    echo "No changes for architecture: $ARCH"
    ;;
esac

# ONNX Runtime version
ONNX_VERSION="1.19.2"
# Set ONNX_FILE based on architecture
case "$ARCH" in
  x86_64)
    ONNX_FILE="onnxruntime-linux-x64-${ONNX_VERSION}"
    ;;
  aarch64 | arm*)
    ONNX_FILE="onnxruntime-linux-aarch64-${ONNX_VERSION}"
    ;;
  *)
    echo "Unsupported architecture: $ARCH"
    exit 1
    ;;
esac

ROOT_DIR=$(pwd)

mkdir -p $ROOT_DIR/lib && mkdir -p $ROOT_DIR/assets && mkdir -p $ROOT_DIR/build

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
wget "https://github.com/microsoft/onnxruntime/releases/download/v${ONNX_VERSION}/${ONNX_FILE}.tgz" && \
    tar -xvzf ${ONNX_FILE}.tgz && \
    mv ${ONNX_FILE} $ROOT_DIR/lib/onnxruntime && \
    rm -rf ${ONNX_FILE}.tgz

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
    cp $ROOT_DIR/lib/all-MiniLM-L6-v2/tokenizer.json $ROOT_DIR/assets/tokenizer.json && \
    cp $ROOT_DIR/lib/all-MiniLM-L6-v2/onnx/model.onnx $ROOT_DIR/assets/model.onnx && \
    rm -rf $ROOT_DIR/lib/all-MiniLM-L6-v2

# Tokenizers C++ bindings
git clone https://github.com/AtlasYang/tokenizers-cpp.git $ROOT_DIR/tokenizers-cpp && \
    cd $ROOT_DIR/tokenizers-cpp/tokenizers && \
    cargo build --release && \
    mkdir -p $ROOT_DIR/lib/tokenizers && \
    cp $ROOT_DIR/tokenizers-cpp/tokenizers/target/release/libtokenizers.so $ROOT_DIR/lib/tokenizers/libtokenizers.so && \
    rm -rf $ROOT_DIR/tokenizers-cpp

# build the project
cd $ROOT_DIR/build && cmake .. && make