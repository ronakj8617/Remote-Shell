FROM ubuntu:22.04
LABEL authors="ronak"

# Install bash and common tools
RUN apt-get update && apt-get install -y bash iproute2 net-tools

# Set default shell
CMD ["bash"]

ENTRYPOINT ["top", "-b"]