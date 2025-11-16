FROM ubuntu
RUN apt-get update -y
RUN apt-get install --yes cmake
RUN apt-get install --yes clang
RUN apt-get install --yes git
RUN apt-get install --yes libsdl2-dev
RUN apt-get install --yes libsdl2-ttf-dev