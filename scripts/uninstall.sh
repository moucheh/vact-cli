#!/bin/bash

if [ -f installed ]; then
  rm ~/.local/bin/vact
  rm ~/.local/bin/locations.txt
  rm installed
else
  echo "You haven't installed vact cli"
fi
