# Stage 1: Build the C project
FROM gcc:13 AS builder

# Set working directory
WORKDIR /app

# Install dependencies
RUN apt-get update && \
    apt-get install -y --no-install-recommends check && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

# Copy source files
COPY include/ include/
COPY src/ src/
COPY Makefile .

# Build the project
RUN make

# Stage 2: Minimal runtime container (still Debian-based)
FROM debian:bookworm-slim

# Install tini and any minimal runtime requirements
RUN apt-get update && \
    apt-get install -y --no-install-recommends tini && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

# Copy the binary from the builder
COPY --from=builder /app/build/main /app/main

# Use tini to avoid PID 1 issues
ENTRYPOINT ["/usr/bin/tini", "--"]

# Default command
CMD ["/app/main"]
