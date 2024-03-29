current_dir := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(current_dir)/config/config

MODULES=Decoder RegisterFile ALU
MODULES_A=$(foreach m,$(MODULES), $(LIB_DIR)/lib$(m).a)

VERILATED_SRCS=$(VERILATOR_INCLUDE)/verilated.cpp $(VERILATOR_INCLUDE)/verilated_vcd_c.cpp $(VERILATOR_INCLUDE)/verilated_cov.cpp
CP_VERITALTED_SRCS=$(patsubst $(VERILATOR_INCLUDE)/%,$(ROOT)/libs/verilated_src/%,$(VERILATED_SRCS))
VERILATED_OBJS=$(patsubst $(ROOT)/libs/verilated_src/%.cpp,$(ROOT)/libs/verilated_src/obj/%.o,$(CP_VERITALTED_SRCS))

NOT_DIR_SRCS=Types.sv Decoder.sv RegisterFile.sv ALU.sv
SRCS=$(foreach s,$(NOT_DIR_SRCS), $(ROOT)/src/$(s))

TEST_SRCS=$(foreach m,$(MODULES),$(wildcard $(ROOT)/test/$(m)/*.cpp))
TEST_OBJS=$(patsubst %.cpp,%.o,$(TEST_SRCS))

REQUIRED_DIRS=$(ROOT)/verilator_gen $(ROOT)/libs/verilated_src/ $(ROOT)/libs/verilated_src/obj/

CPPFLAGS=-I$(ROOT)/libs/googletest/googletest/include -I$(VERILATOR_INCLUDE)\
	-I$(ROOT)/test/common -I$(ROOT)/verilator_gen -std=c++2a -fconcepts -g -DVM_TRACE=1 -DVM_COVERAGE=1

.PHONY:all clean check

LIBS=$(foreach m,$(MODULES),-l$(m)) -lverilator -lgtest -lgtest_main -pthread

all: $(REQUIRED_DIRS) $(LIB_DIR)/libverilator.a $(MODULES_A) $(LIB_DIR)/libgtest.a

$(LIB_DIR)/libverilator.a: $(VERILATED_OBJS)
	mkdir -p $(LIB_DIR)
	ar rcs $@ $^

check: $(ROOT)/bin/run_tests
	$<

$(ROOT)/bin/run_tests: $(TEST_OBJS)
	mkdir -p $(ROOT)/bin
	g++ $(TEST_OBJS) -o $@ -L$(LIB_DIR) $(LIBS)

$(LIB_DIR)/libgtest_main.a: $(ROOT)/libs/googletest/lib/libgtest_main.a
	cp $< $@

$(ROOT)/libs/googletest/lib/libgtest.a: $(ROOT)/libs/googletest/lib/libgtest_main.a

$(LIB_DIR)/libgtest.a: $(ROOT)/libs/googletest/lib/libgtest.a $(LIB_DIR)/libgtest_main.a
	cp $< $@

$(ROOT)/libs/googletest/lib/libgtest_main.a:
	cmake $(ROOT)/libs/googletest/ -B $(ROOT)/libs/googletest/
	$(MAKE) -C $(ROOT)/libs/googletest/

$(ROOT)/verilator_gen:
	mkdir -p $@

$(ROOT)/verilator_gen/%__ALL.a: $(ROOT)/src/%.sv
	verilator --Mdir $(dir $@) --trace --coverage -sv --cc $(SRCS) --top-module $(patsubst $(ROOT)/src/%.sv,%,$<) --prefix $(basename $(notdir $<))
	$(MAKE) -C $(dir $@) -f $(basename $(notdir $<)).mk

$(ROOT)/lib/lib%.a: $(ROOT)/verilator_gen/%__ALL.a
	cp $< $@

$(ROOT)/libs/verilated_src/obj/%.o: $(ROOT)/libs/verilated_src/%.cpp
	g++ $(CPPFLAGS) -c -o $@ $<

$(ROOT)/libs/verilated_src/obj/:
	mkdir -p $@

$(ROOT)/libs/verilated_src/:
	mkdir -p $@

.PRECIOUS:$(ROOT)/libs/verilated_src/%.cpp
$(ROOT)/libs/verilated_src/%.cpp: $(VERILATOR_INCLUDE)/%.cpp
	cp $< $@

$(ROOT)/test/%/obj:
	mkdir -p $@

$(ROOT)/test/%.o: $(ROOT)/test/%.cpp
	g++ $(CPPFLAGS) -c -o $@ $< 

clean:
	rm -rf verilator_gen lib/* $(TEST_OBJS) libs/verilated_src
	$(MAKE) -C $(ROOT)/libs/googletest/ clean
