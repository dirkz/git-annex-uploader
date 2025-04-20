# Git Annex File Uploader

Simple c++ console project that uploads git annex files
in a space preserving manner.

Set up with Visual Studio, CMake, WSL 2 (Ubuntu).

Basically a load of`git annex copy <file> --to <remote>`
for every file encountered,
with corresponding `git annex get` and `git annex drop` commands.