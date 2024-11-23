FROM ubuntu:latest
RUN apt-get -o Acquire::Max-FutureTime=86400 update -y
RUN export DEBIAN_FRONTEND=noninteractive && apt-get install -y build-essential xorg libx11-dev libxext-dev libbsd-dev
RUN apt-get install -y libpthread-stubs0-dev

WORKDIR /best_rt
COPY . .
RUN make

# run the executable without arguments, or with the path to a scene file as argument e.g."scenes/snowman.rt"
CMD ["./miniRT", "scenes/multilight.rt"]
