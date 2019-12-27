all:
ifeq ($(debug),1)
	cd ./srcs/molenet && make clean && make debug=1
	cd ./srcs/MahJong && make clean && make debug=1
	cd ./accountserver && make clean && make debug=1
	cd ./gameserver && make clean && make debug=1
	cd ./games/example && make clean && make debug=1
	cd ./games/lhd && make clean && make debug=1
	cd ./games/lunpan && make clean && make debug=1
	cd ./games/sszp && make clean && make debug=1
	cd ./games/bcbm && make clean && make debug=1
	cd ./games/brnn && make clean && make debug=1
	cd ./games/hlddz && make clean && make debug=1
	cd ./games/xzmj && make clean && make debug=1
	cd ./games/zjh && make clean && make debug=1
	cd ./games/dzpk && make clean && make debug=1
else
	cd ./srcs/molenet && make clean && make
	cd ./srcs/MahJong && make clean && make
	cd ./accountserver && make clean && make
	cd ./gameserver && make clean && make
	cd ./games/example && make clean && make
	cd ./games/lhd && make clean && make
	cd ./games/lunpan && make clean && make
	cd ./games/sszp && make clean && make
	cd ./games/bcbm && make clean && make
	cd ./games/brnn && make clean && make
	cd ./games/hlddz && make clean && make
	cd ./games/xzmj && make clean && make
	cd ./games/zjh && make clean && make
	cd ./games/dzpk && make clean && make
endif
$(info,"project compile succeed.")
