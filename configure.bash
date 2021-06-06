ROOT=$PWD
VERILATOR_ROOT=`verilator -V | grep "\s*VERILATOR_ROOT\s*=\s*\S" | sed 's/\s*VERILATOR_ROOT\s*=\s*//g'`

mkdir -p config
echo "" >config/config
echo "ROOT=$ROOT" >> config/config
echo "VERILATOR_ROOT=$VERILATOR_ROOT" >> config/config