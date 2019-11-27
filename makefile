all:
ifeq ($(debug),1)
	cd ./srcs/molenet && make clean && make debug=1
	cd ./accountserver && make clean && make debug=1
	cd ./gameserver && make clean && make debug=1
	cd ./games/example && make clean && make debug=1
	cd ./games/lhd && make clean && make debug=1
	cd ./games/lunpan && make clean && make debug=1
else
	cd ./srcs/molenet && make clean && make
	cd ./accountserver && make clean && make
	cd ./gameserver && make clean && make
	cd ./games/example && make clean && make
	cd ./games/lhd && make clean && make
	cd ./games/lunpan && make clean && make
endif
$(info,"project compile succeed.")
