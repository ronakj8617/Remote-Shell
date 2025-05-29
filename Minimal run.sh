#!/bin/bash

set -e  # Exit immediately on error

echo "🧹 Clear the terminal"
clear

if ! docker build -t remote-shell-image .; then
      echo "❌ Doccker build failed!"]
      exit 1
fi

echo "📁 Moving to build directory..."
cd build

echo "⚙️  Running CMake..."
cmake ..

echo "🔨 Building project..."
if ! make; then
    echo "❌ Build failed!"
    exit 1
fi

#if ! ctest; then
#      echo "❌ Test cases failed!"]
#      exit 1
#fi

if [ ! -f ./remoteshell ]; then
  echo "❌ Build succeeded but remote shell binary not found!"
  exit 1
fi

echo "🚀 Running the remote shell..."
./remoteshell

#docker system prune -f
