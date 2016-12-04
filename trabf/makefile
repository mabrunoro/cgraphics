# Makefile para compilar Trabalho 2 de CG
# Autor: Marco Thomé

# Warnings para funções deprecated está desativada.
#	Para ativar, altere WARFLAG para false

# O código verifica apenas se o SO é OS X ou não.
#	Caso não seja OS X, assume que é Linux.

# Modo debug imprime os parâmetros de todos objetos.
#	Ex.: posição, tipo, cor

TP := $(shell echo `uname`)
WARFLAG := true

BBM = -lm
VER = -std=c++11
FIL =	trabf.cc tinyxml2.cpp\
		cor.cc circulo.cc\
		elipse.cc retangulo.cc\
		janela.cc municao.cc carro.cc
MAC =	-D RANSPEED=0.05 \
		-D RAIOP=20.0 \
		-D RTIRO=2.0 \
		-D TGAME=45
TEST = teste.cc
OUT = -o trabalhocg

ifeq (${TP},Darwin)
	OSTYPE = OS X
	FWK = -framework GLUT -framework OpenGL -framework Cocoa
	ML = -D MACLINUX=true
# endif
# ifeq (${TP},Linux)
else # Caso comando 'uname' não retorne Linux, assuma-o mesmo assim
	OSTYPE = Linux
	FWK = -lglut -lGLU -lGL
	ML = -D MACLINUX=false
endif
ifeq (${WARFLAG},true)
	WAR = -Wno-deprecated
endif


all: ${FIL}
	@printf "\nMakefile\n"
	@printf "Compilando: trabalhocg\n"
	@printf "Sistema Operacional: ${OSTYPE}\n"
	@printf "Arquivos: ${FIL}\n"
	@printf "Warnings: ${WAR}\n"
	@printf "Links: ${FWK} ${BBM}\n"
	@printf "Versão: C++ 11\e\n"
	@g++ ${FIL} ${WAR} ${FWK} ${ML} ${BBM} ${VER} ${OUT} ${MAC}

debug: ${FIL}
	@printf "\nMakefile\n"
	@printf "Compilando: trabalhocg\n"
	@printf "Sistema Operacional: ${OSTYPE}\n"
	@printf "Arquivos: ${FIL}\n"
	@printf "Warnings: ${WAR}\n"
	@printf "Links: ${FWK} ${BBM}\n"
	@printf "Versão: C++ 11\n\n"
	@printf "\tModo debug ativado\n\n"
	@g++ ${FIL} ${WAR} ${FWK} ${ML} ${BBM} ${VER} ${OUT} ${MAC} -D DEBUG=true

# teste: ${TEST}
# 	@g++ ${TEST} ${WAR} ${FWK} ${ML} ${BBM} ${VER} ${MAC}
# 	@./a.out

run:
	./trabalhocg ./4\ -\ JogoCanhao/

clean:
	rm trabalhocg
