FROM ubuntu:22.04

COPY . /app

WORKDIR /app

RUN cd /app && sh setup.linux.sh

CMD ["/app/build/cms-api"]