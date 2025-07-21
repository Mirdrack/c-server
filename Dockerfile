# Stage 1: Build the C project using GCC
FROM gcc:13 AS builder

WORKDIR /app

# Copy only what's needed to build
# COPY include/ include/
COPY src/ src/
COPY Makefile .

# Build the project
RUN make

# Stage 2: Minimal runtime container with only the final binary
FROM alpine:latest

# Install the minimal required runtime (e.g., libc, if needed)
RUN apk add --no-cache libgcc libstdc++  # usually not needed for pure C, but safe

# Copy the built binary from the previous stage
COPY --from=builder /app/build/main /main

# Default command
CMD ["/main"]
