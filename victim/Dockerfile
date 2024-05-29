FROM gcc:latest

RUN apt-get update && apt-get install -y make

# Source code respository
WORKDIR /usr/src/app

# Copy the source code
COPY * .

# Compile the source code
RUN make all

ENTRYPOINT ["/bin/bash"]
