FROM ubuntu
RUN apt-get update && \
    apt-get install -y \
    cmake protobuf-compiler\
    make \
    git \
    gcc \
    g++


WORKDIR /CPP-PROJECT
RUN mkdir -p ./files
RUN mkdir -p ./src
RUN mkdir -p ./include

COPY ./files/* ./files/
COPY ./src/* ./src/
COPY ./include/* ./include/
COPY ./cmake-files ./cmake-files/
COPY CMakeLists.txt ./
RUN mkdir ./build
#RUN RUN cmake ..
#CMD  ./output