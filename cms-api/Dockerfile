FROM ubuntu:latest

# Install dependencies
RUN apt-get update && apt-get install -y \
    libuv1-dev libssl-dev zlib1g-dev \
    pkg-config build-essential linux-libc-dev \
    cmake libcurlpp-dev libpugixml-dev nlohmann-json3-dev

COPY . /app
WORKDIR /app

# Build the project
RUN cd /app && \
    mkdir -p build && \
    cd build && \
    cmake .. && \
    make

CMD ["/app/build/cms-api"]