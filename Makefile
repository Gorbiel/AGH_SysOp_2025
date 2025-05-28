CC = gcc
CFLAGS = -Wall -Wextra -g -fPIC -I./src
MAKEFLAGS += --no-print-directory

# Directory for source files and test scripts
SRC_DIR = ./src
TEST_DIR = ./test
RES_DIR = ./resources

# Targets to build
TARGETS = client_static client_shared client_dynamic countdown flipper proc_1 proc_2 sig_1 catcher sender pipe_1 pipe_2_IO pipe_2_calculator chat_client chat_server print_system integral doctor socket_chat_client socket_chat_server socket_chat_client_2 socket_chat_server_2
LAB_TARGETS = lab01 lab02 lab03 lab04 lab05 lab06 lab07 lab08 lab09 lab10 lab11 lab12

# Source and object files
LIBSRC = $(SRC_DIR)/collatz.c
LIBOBJ = $(LIBSRC:.c=.o)
LIBHEADER = $(SRC_DIR)/collatz.h

STATIC_LIB = libcollatz.a
SHARED_LIB = libcollatz.so.0.0.1

.PHONY: all collatz clean cleaner labs $(LAB_TARGETS)

all: $(TARGETS)

labs: cleaner $(LAB_TARGETS)
	@echo "-------------------------------------------------------"
	@echo "|                                                     |"
	@echo "|   All labs completed. Check log.txt for details.    |"
	@echo "|   Run \033[0;36mmake cleaner\033[0m to remove all leftover files.    |"
	@echo "|                                                     |"
	@echo "-------------------------------------------------------"

client_static: $(LIBOBJ)
	ar rcs $(STATIC_LIB) $(LIBOBJ)
	$(CC) $(CFLAGS) -o client_static $(SRC_DIR)/client.c -L. -lcollatz -static

client_shared: $(LIBOBJ)
	$(CC) -shared -o $(SHARED_LIB) $(LIBOBJ)
	$(CC) $(CFLAGS) -o client_shared $(SRC_DIR)/client.c -L. -lcollatz -Wl,-rpath,.

client_dynamic: $(LIBOBJ)
	$(CC) -shared -o $(SHARED_LIB) $(LIBOBJ)
	$(CC) $(CFLAGS) -o client_dynamic $(SRC_DIR)/client.c -ldl -DDYNAMIC_LOAD -Wl,-rpath,.

collatz: client_static client_shared client_dynamic

countdown: $(SRC_DIR)/countdown.c
	$(CC) $(CFLAGS) -o countdown $(SRC_DIR)/countdown.c

flipper: $(SRC_DIR)/flipper.c
	$(CC) $(CFLAGS) -o flipper $(SRC_DIR)/flipper.c

proc_1: $(SRC_DIR)/proc_1.c
	$(CC) $(CFLAGS) -o proc_1 $(SRC_DIR)/proc_1.c

proc_2: $(SRC_DIR)/proc_2.c
	$(CC) $(CFLAGS) -o proc_2 $(SRC_DIR)/proc_2.c

sig_1: $(SRC_DIR)/sig_1.c
	$(CC) $(CFLAGS) -o sig_1 $(SRC_DIR)/sig_1.c

catcher: $(SRC_DIR)/catcher.c
	$(CC) $(CFLAGS) -o catcher $(SRC_DIR)/catcher.c

sender: $(SRC_DIR)/sender.c
	$(CC) $(CFLAGS) -o sender $(SRC_DIR)/sender.c

pipe_1: $(SRC_DIR)/pipe_1.c
	$(CC) $(CFLAGS) -o pipe_1 $(SRC_DIR)/pipe_1.c

pipe_2_IO: $(SRC_DIR)/pipe_2_IO.c
	$(CC) $(CFLAGS) -o pipe_2_IO $(SRC_DIR)/pipe_2_IO.c

pipe_2_calculator: $(SRC_DIR)/pipe_2_calculator.c
	$(CC) $(CFLAGS) -o pipe_2_calculator $(SRC_DIR)/pipe_2_calculator.c

chat_client: $(SRC_DIR)/chat_client.c
	$(CC) $(CFLAGS) -o chat_client $(SRC_DIR)/chat_client.c

chat_server: $(SRC_DIR)/chat_server.c
	$(CC) $(CFLAGS) -o chat_server $(SRC_DIR)/chat_server.c

print_system: $(SRC_DIR)/print_system.c
	$(CC) $(CFLAGS) -o print_system $(SRC_DIR)/print_system.c

integral: $(SRC_DIR)/integral.c
	$(CC) $(CFLAGS) -o integral $(SRC_DIR)/integral.c -lm

doctor: $(SRC_DIR)/doctor.c
	$(CC) $(CFLAGS) -pthread -o doctor $(SRC_DIR)/doctor.c

socket_chat_client: $(SRC_DIR)/socket_chat_client.c
	$(CC) $(CFLAGS) -o socket_chat_client $(SRC_DIR)/socket_chat_client.c

socket_chat_server: $(SRC_DIR)/socket_chat_server.c
	$(CC) $(CFLAGS) -o socket_chat_server $(SRC_DIR)/socket_chat_server.c

socket_chat_client_2: $(SRC_DIR)/socket_chat_client_2.c
	$(CC) $(CFLAGS) -o socket_chat_client_2 $(SRC_DIR)/socket_chat_client_2.c

socket_chat_server_2: $(SRC_DIR)/socket_chat_server_2.c
	$(CC) $(CFLAGS) -o socket_chat_server_2 $(SRC_DIR)/socket_chat_server_2.c


# Lab targets
lab01: countdown
	stdbuf -oL $(TEST_DIR)/countdown_test.sh 2>&1 | tee -a log.txt
	make clean 2>&1 | tee -a log.txt

lab02: client_static client_shared client_dynamic
	stdbuf -oL $(TEST_DIR)/collatz_test.sh 2>&1 | tee -a log.txt
	make clean 2>&1 | tee -a log.txt

lab03: flipper
	stdbuf -oL $(TEST_DIR)/flipper_test.sh 2>&1 | tee -a log.txt
	make clean 2>&1 | tee -a log.txt

lab04: proc_1 proc_2
	stdbuf -oL $(TEST_DIR)/proc_test.sh 2>&1 | tee -a log.txt
	make clean 2>&1 | tee -a log.txt

lab05: sig_1 catcher sender
	stdbuf -oL $(TEST_DIR)/sig_1_test.sh 2>&1 | tee -a log.txt
	stdbuf -oL $(TEST_DIR)/catcher_test.sh 2>&1 | tee -a log.txt
	make clean 2>&1 | tee -a log.txt

lab06: pipe_1 pipe_2_IO pipe_2_calculator
	stdbuf -oL $(TEST_DIR)/pipe_1_test.sh 2>&1 | tee -a log.txt
	stdbuf -oL $(TEST_DIR)/pipe_2_test.sh 2>&1 | tee -a log.txt
	make clean 2>&1 | tee -a log.txt

lab07: chat_client chat_server
	stdbuf -oL $(TEST_DIR)/chat_test.sh 2>&1 | tee -a log.txt
	make clean 2>&1 | tee -a log.txt

lab08: print_system
	stdbuf -oL $(TEST_DIR)/print_system_test.sh 2>&1 | tee -a log.txt
	make clean 2>&1 | tee -a log.txt

lab09: integral
	stdbuf -oL $(TEST_DIR)/integral_test.sh 2>&1 | tee -a log.txt
	make clean 2>&1 | tee -a log.txt

lab10: doctor
	stdbuf -oL $(TEST_DIR)/doctor_test.sh 2>&1 | tee -a log.txt
	make clean 2>&1 | tee -a log.txt

lab11: socket_chat_client socket_chat_server
	stdbuf -oL $(TEST_DIR)/socket_chat_test.sh 2>&1 | tee -a log.txt
	make clean 2>&1 | tee -a log.txt

lab12: socket_chat_client_2 socket_chat_server_2
	stdbuf -oL $(TEST_DIR)/socket_chat_2_test.sh 2>&1 | tee -a log.txt
	make clean 2>&1 | tee -a log.txt

%.o: $(SRC_DIR)/%.c $(LIBHEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(TARGETS) $(LIBOBJ) $(STATIC_LIB) $(SHARED_LIB)
	@rm -rf ./*.log

cleaner: clean
	@rm -rf reversed restored
	@rm -f log.txt