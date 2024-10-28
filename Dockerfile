# Use a base Ubuntu image
FROM ubuntu:22.04

# Update the package lists
RUN apt-get update -y

# Install required packages
RUN export DEBIAN_FRONTEND=noninteractive && apt-get install -y build-essential xorg libx11-dev libxext-dev libbsd-dev

# Create a working directory
WORKDIR /best_rt

# Copy your C source code and MinilibX source code to the working directory
COPY . .

# Compile MinilibX
WORKDIR /best_rt/minilibx-linux
RUN make

# Compile your C program
WORKDIR /best_rt
RUN make

# Expose the port (if necessary, adjust the port number)
EXPOSE 8080

# Define the command to run when the container starts
CMD ["./miniRT", "scenes/snowman.rt"]
