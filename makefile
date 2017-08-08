BUILD_DIR = build
TUP = tup

.PHONY : all iso run

all : iso

run-amd64 : all
	sh ./script/qemu-amd64.sh

run-i386 : all
	sh ./script/qemu-i386.sh

iso : $(BUILD_DIR)
	$(TUP)

$(BUILD_DIR) :
	sh ./script/setup.sh
	$(TUP) init
