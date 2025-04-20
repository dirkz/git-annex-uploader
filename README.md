# Git Annex File Uploader

Simple c++ console project that uploads git annex files
in a space preserving manner.

Set up with Visual Studio, CMake, WSL 2 (Ubuntu).

Basically a `git annex copy <file> --to <backend>`,
with corresponding `git annex get` and `git annex drop` commands.