BUILD_DIR = build
TUP = tup

.PHONY : all iso run

all : iso

run : all
	sh ./script/qemu-amd64.sh

iso : $(BUILD_DIR)
	$(TUP)

$(BUILD_DIR) :
	sh ./script/setup.sh
	$(TUP) init
