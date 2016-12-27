#!/bin/sh
cp ./README.markdown ./README.md
judge --help >> README.md
echo "\`\`\`" >> README.md
