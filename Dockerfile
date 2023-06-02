FROM ubuntu
RUN apt-get update && \
    apt-get install -y \
    cmake protobuf-compiler\
    make \
    git \
    gcc \
    g++


RUN mkdir /CPP-PROJECT && cd /CPP-PROJECT
RUN mkdir -p ./files
RUN mkdir -p ./src
RUN mkdir -p ./include
WORKDIR /cpp-PROJECT
COPY ./files/* ./files/
COPY ./src/* ./src/
COPY ./include/* ./include/
COPY ./cmake-files ./cmake-files/
COPY CMakeLists.txt ./
RUN mkdir ./build && cd build ; cmake .. && make
#RUN RUN cmake ..
#CMD  ./output