mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(current_dir)/../../config/config

default: $(TEST_OUTPUT)

VERILATOR_INCLUDE = $(VERILATOR_ROOT)/include

VERILATOR_PREFIX = V$(TEST_MODULE)

VERILATOR_GEN_DIR = $(ROOT)/verilator_gen/$(TEST_MODULE)

VERILATOR_MOULE_MK = $(VERILATOR_GEN_DIR)/$(VERILATOR_PREFIX).mk
VERILATOR_MODULE_LIB = $(VERILATOR_GEN_DIR)/$(VERILATOR_PREFIX)__ALL.a

gen_files: $(VERILATOR_MOULE_MK)

$(VERILATOR_GEN_DIR):
	mkdir -p $(VERILATOR_GEN_DIR)

$(VERILATOR_MODULE_LIB): $(VERILATOR_MOULE_MK)
	$(MAKE) -C $(VERILATOR_GEN_DIR) -f $(VERILATOR_PREFIX).mk

$(VERILATOR_MOULE_MK): $(ROOT)/src/$(TEST_MODULE).sv $(VERILATOR_GEN_DIR)
	verilator --Mdir $(VERILATOR_GEN_DIR) -Wall --threads 16 --trace -sv --cc -CFLAGS "-std=c++2a -fconcepts" $(ROOT)/src/$(TEST_MODULE).sv


$(TEST_OUTPUT): $(TEST_SOURCES) $(VERILATOR_MODULE_LIB)
	g++ -DVL_THREADED -I$(ROOT)/test/common -I$(VERILATOR_INCLUDE) -I$(VERILATOR_GEN_DIR) -std=c++2a -fconcepts $(TEST_SOURCES) $(VERILATOR_INCLUDE)/verilated.cpp $(VERILATOR_INCLUDE)/verilated_threads.cpp $(VERILATOR_INCLUDE)/verilated_vcd_c.cpp -o $(TEST_OUTPUT)  $(VERILATOR_MODULE_LIB) -pthread

clean:
	rm -f $(TEST_OUTPUT)
	rm -rf $(VERILATOR_GEN_DIR)