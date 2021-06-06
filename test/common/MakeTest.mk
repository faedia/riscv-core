mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
SHELL=/usr/bin/bash
current_dir := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(current_dir)/../../config/config

default: run

VERILATOR_INCLUDE = $(VERILATOR_ROOT)/include

VERILATOR_PREFIX = $(TEST_MODULE)

VERILATOR_GEN_DIR = $(ROOT)/verilator_gen/$(TEST_MODULE)

VERILATOR_MOULE_MK = $(VERILATOR_GEN_DIR)/$(VERILATOR_PREFIX).mk
VERILATOR_MODULE_LIB = $(VERILATOR_GEN_DIR)/$(VERILATOR_PREFIX)__ALL.a

gen_files: $(VERILATOR_MOULE_MK)

$(VERILATOR_MODULE_LIB): $(VERILATOR_MOULE_MK)
	$(MAKE) -C $(VERILATOR_GEN_DIR) -f $(VERILATOR_PREFIX).mk

$(VERILATOR_MOULE_MK): $(ROOT)/src/$(TEST_MODULE).sv
	mkdir -p $(VERILATOR_GEN_DIR)
	verilator --Mdir $(VERILATOR_GEN_DIR) --threads 16 --trace -sv --cc $(ROOT)/src/{Types,Decoder,RegisterFile}.sv --top-module $(TEST_MODULE) --prefix $(TEST_MODULE)

$(ROOT)/libs/googletest/Makefile: $(ROOT)/libs/googletest/CMakeLists.txt
	cmake $(ROOT)/libs/googletest/ -B $(ROOT)/libs/googletest/

$(ROOT)/libs/googletest/lib/libgtest_main.a: $(ROOT)/libs/googletest/Makefile
	$(MAKE) -C $(ROOT)/libs/googletest/

test_main: $(TEST_SOURCES) $(VERILATOR_MODULE_LIB) $(ROOT)/libs/googletest/lib/libgtest_main.a
	g++ -I$(ROOT)/libs/googletest/googletest/include -L$(ROOT)/libs/googletest/lib/ -I$(ROOT)/libs/googletest/ -DVL_THREADED -I$(ROOT)/test/common -I$(VERILATOR_INCLUDE) -I$(VERILATOR_GEN_DIR) -std=c++2a -fconcepts $(TEST_SOURCES) $(VERILATOR_INCLUDE)/verilated.cpp $(VERILATOR_INCLUDE)/verilated_threads.cpp $(VERILATOR_INCLUDE)/verilated_vcd_c.cpp -o test_main  $(VERILATOR_MODULE_LIB) -lgtest_main -lgtest -pthread

build: test_main

run: test_main
	./test_main

clean:
	rm -f test_main
	rm -f *.vcd
	rm -rf $(VERILATOR_GEN_DIR)