@echo off

pushd Releases
md new_release
pushd new_release

copy ..\..\assetFile assetFile
copy ..\..\Compendium Compendium

md msvc
copy ..\..\out\release.exe msvc\release.exe

md gcc
copy ..\..\gccout\release.exe gcc\release.exe

md clang
copy ..\..\clangout\release.exe clang\release.exe

popd

md new-Claudio
copy new_release\assetFile new-Claudio\assetFile
copy new_release\Compendium new-Claudio\Compendium
copy new_release\clang\release.exe new-Claudio\release.exe

popd