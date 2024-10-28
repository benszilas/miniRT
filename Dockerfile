FROM ubuntu:latest
RUN apt-get update -y
RUN export DEBIAN_FRONTEND=noninteractive && apt-get install -y build-essential xorg libx11-dev libxext-dev libbsd-dev

WORKDIR /best_rt
COPY . .
RUN make

# Define the command to run when the container starts
CMD ["./miniRT", "scenes/snowman.rt"]
