FROM ubuntu
RUN apt-get update && \
    apt-get install -y \
    cmake \
    make \
    git \
    gcc \
    g++ 

RUN apt-get update -y && apt-get install -y g++ make wget libboost-all-dev git \
libcpprest-dev \
libspdlog-dev \
rapidjson-dev

RUN  mkdir /CPP-PROJECT
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