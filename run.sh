#!/bin/bash

set -e  # Exit immediately on error

echo "🧹 Clear the terminal"
clear

echo "🧹 Cleaning previous CMake cache and build files..."
rm -rf CMakeCache.txt CMakeFiles Makefile cmake_install.cmake build

echo " 🔨 Building docker..."
if ! docker build -t remote-shell-image .; then
      echo "❌ Doccker build failed!"]
      exit 1
fi

echo "📁 Creating build directory..."
mkdir -p build
cd build

echo "⚙️  Running CMake..."
cmake ..

echo "🔨 Building project..."
if ! make; then
    echo "❌ Build failed!"
    exit 1
fi

if ! ctest; then
      echo "❌ Test cases failed!"
      exit 1
fi

if [ ! -f ./remoteshell ]; then
  echo "❌ Build succeeded but remote shell binary not found!"
  exit 1
fi

echo "🚀 Running the remote shell (with default ports)..."
./remoteshell # REST_PORT=9090 SHELL_PORT=9091 ./remoteshell

#docker system prune -f
