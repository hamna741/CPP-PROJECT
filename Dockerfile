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
RUN  apt-get -y install openssl




WORKDIR /CPP-PROJECT

RUN git clone https://github.com/google/googletest.git

# Build and install Google Test
RUN cd googletest && cmake -E make_directory "build"


RUN git clone https://github.com/google/benchmark.git
RUN cd benchmark && cmake -E make_directory "build"


# COPY ./files/* ./files/
# COPY ./src/* ./src/
# COPY ./include/* ./include/
# COPY ./BENCHMARKING/* ./BENCHMARK/
# COPY ./unittest/* ./unittest/
# COPY CMakeLists.txt ./
# RUN mkdir ./build
#RUN RUN cmake ..
#CMD  ./output