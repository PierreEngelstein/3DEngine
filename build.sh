build_dir="build/"
source_dir="$(pwd)"

for i in "$@"
do
case $i in
    -c|--clean)
    rm -rf $build_dir/
    rm -rf bin/
    shift
    ;;
    -b=*|--builddir=*)
    build_dir="${i#*=}"
    shift
    ;;
    -s=*|--sourcedir=*)
    sourcedir="${i#*=}"
    shift
    ;;
    -h|--help)
    echo "Usage"
    echo "./build.sh [options]"
    echo "Options"
    echo "  -c cleanup build and bin directories"
    echo "  -b=<path-to-build>   = Explicitly specify the cmake build directory"
    echo "  -s=<path-to-source>  = Explicitly specify the source folder"
    exit 0
esac
done

#rm -rf $source_dir/bin/resources
#rm $source_dir/bin/ProjectGame.dll
#rm $source_dir/bin/Scripting.Core.dll

echo $source_dir/bin/

rm -rf $source_dir/bin/

mkdir -p $build_dir && cd $build_dir/
echo "cmake $source_dir"
cmake $source_dir
make -j10

cd $source_dir/resources/Scripting/ProjectGame/
dotnet build
cat myscript.cs

cd $source_dir/bin
cp $source_dir/resources/Scripting/ProjectGame/bin/Debug/netcoreapp3.1/ProjectGame.dll ProjectGame.dll
cp $source_dir/resources/Scripting/ProjectGame/bin/Debug/netcoreapp3.1/Scripting.Core.dll Scripting.Core.dll
rm -rf $source_dir/bin/resources/Scripting
./game
cd $source_dir