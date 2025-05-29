# Use Ubuntu as a base image
FROM ubuntu:22.04

# Install basic shell tools
RUN apt-get update && \
    apt-get install -y bash coreutils && \
    apt-get clean

# Default shell
CMD ["/bin/bash"]
