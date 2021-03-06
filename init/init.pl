$cc="g++";
$gdb="gdb";
$ddd="ddd";

$default_header="#include <c-script.h>\n";

$output_opt="-o ";


$normal_opt=qq(
-O4
-DNDEBUG
-march=native
--no-warn
);


$normal_opt=qq(
-O4
-DNDEBUG
-mfpmath=sse
-mmmx
-msse2
-march=native
-ftree-vectorize
--no-warn
);
    
$normal_opt=qq(
-O4
);


$debug_opt ="-g";


$opt=qq(
-I$script_home
-I$script_home/include
-L$script_home
-I.
-L.
);

if($static){
    $opt.=' -static ';
}

$sys_src=qq(
);


$libs=qq(
-lm
);



#$libs.= getlib("/usr/lib/libboost_regex*.a");
if($vc){
    $cc="cl";
    $output_opt="/Fe";
    $normal_opt=qq(/Ox);
    $debug_opt ="/ZI";
    $opt=qq(/I$script_home /I$script_home/include /I.     );
    $libs=qq(/MT /link /libpath:. /libpath:$script_home    );
}

if($mingw32){
    if( -e "/usr/bin/i586-mingw32msvc-g++"){
	$cc="i586-mingw32msvc-g++";
    }elsif( -e "/usr/bin/i686-w64-mingw32-g++"){
	$cc="i686-w64-mingw32-g++";
    }
    # $opt.=qq(
    #   -I$script_home/mingw32/blas
    #   -I$script_home/mingw32/lapack
    #   -I$script_home/mingw32/libpng
    #   -I$script_home/mingw32/libjpeg
    #   -I$script_home/mingw32/zlib
    # );
    # $libs=qq(
    #   -L$script_home/mingw32/blas
    #   -L$script_home/mingw32/lapack
    #   -L$script_home/mingw32/libpng
    #   -L$script_home/mingw32/libjpeg
    #   -L$script_home/mingw32/zlib
    #   -llapack
    #   -lblas
    #   -lgfortran
    #   -ljpeg
    #   -lpng
    #   -lz
    #   -lm
    # );
}

%auto_package=(
    Matrix   => matrix,
    Data     => data,
    BMP      => bmp,
    BMPb     => bmp,
    FreeType => freetype,
);

