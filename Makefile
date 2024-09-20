# Compiler settings and get all binaries
CXX = g++
CXXFLAGS = -std=c++20 -Wall
SRC = $(wildcard *.cc)
BIN = $(SRC:.cc=)

# Rules
default: assemble_teams_using_latch bank_account_using_mutex_locks cake_baking_using_barriers

assemble_teams_using_latch: assemble_teams_using_latch.cc
	$(CXX) $(CXXFLAGS) -o assemble_teams_using_latch assemble_teams_using_latch.cc

bank_account_using_mutex_locks: bank_account_using_mutex_locks.cc
	$(CXX) $(CXXFLAGS) -o bank_account_using_mutex_locks bank_account_using_mutex_locks.cc

cake_baking_using_barriers: cake_baking_using_barriers.cc
	$(CXX) $(CXXFLAGS) -o cake_baking_using_barriers cake_baking_using_barriers.cc

clean:
	@$(RM) $(BIN)


# Declare that the targets default and clean are not actual files
.PHONY: default clean
