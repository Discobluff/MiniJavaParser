FROM ubuntu
RUN apt-get update -y
RUN apt-get install --yes cmake
RUN apt-get install --yes clang
RUN apt-get install --yes git
RUN git clone https://gitlab.com/cunity/cunit.git/
WORKDIR "/cunit"
RUN cmake . 
RUN cmake --build .