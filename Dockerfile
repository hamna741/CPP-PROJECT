FROM ubuntu
COPY requirements.txt requirements.txt
RUN apt-get update && \
    xargs -a requirements.txt apt-get install -y
  
# RUN apt-get install -y build-essential cmake g++ libssl-dev

# RUN apt-get install -y libssl1.1
WORKDIR /app
RUN mkdir -p ./files
RUN mkdir -p ./build
ENV LD_LIBRARY_PATH=./build/libexchangeInfo.so:$LD_LIBRARY_PATH


#COPY * /app
COPY ./build/libexchangeInfo.so /app/build/
COPY ./files/* /app/files/
COPY ./build/output /app/build/

#CMD  ./build/output