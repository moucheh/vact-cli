#!/bin/bash

if [ -d ~/.local/bin ]; then
  cp vact ~/.local/bin
  cp locations.txt ~/.local/bin
  echo "$(date)" > installed
else
  mkdir -p ~/.local/bin
  echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.bashrc
  echo 'Added ~/.local/bin to your PATH in bashrc, if you are using a different shell, you need to add it manually'
  . ~/.bashrc
  cp vact ~/.local/bin
  cp locations.txt ~/.local/bin
fi
