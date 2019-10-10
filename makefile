all:
ifeq ($(debug),1)
	cd ./srcs/molenet && make clean && make debug=1
	cd ./loginserver && make clean && make debug=1
	cd ./gameserver && make clean && make debug=1
	cd ./games/example && make clean && make debug=1
else
	cd ./srcs/molenet && make clean && make
	cd ./loginserver && make clean && make
	cd ./gameserver && make clean && make
	cd ./games/example && make clean && make
endif
$(info "project compile succeed.")