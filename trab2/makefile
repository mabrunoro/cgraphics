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
VER = -std=c++14
FIL = trab2.cc tinyxml2.cpp
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


all: trab2.cc tinyxml2.cpp
	@printf "\n\e[1;3mMakefile\e[0m\n"
	@printf "\t\e[1mCompilando:\e[0m \e[90mtrabalhocg\e[0m\n"
	@printf "\t\e[1mSistema Operacional:\e[0m \e[90m${OSTYPE}\e[0m\n"
	@printf "\t\e[1mArquivos:\e[0m \e[90m${FIL}\e[0m\n"
	@printf "\t\e[1mWarnings:\e[0m \e[90m${WAR}\e[0m\n"
	@printf "\t\e[1mLinks:\e[0m \e[90m${FWK} ${BBM}\e[0m\n"
	@printf "\t\e[1mVersão:\e[0m \e[90mC++ 14\e[0m\n\n"
	@g++ ${FIL} ${WAR} ${FWK} ${ML} ${BBM} ${VER} ${OUT}

debug: trab2.cc tinyxml2.cpp
	@printf "\n\e[1;3mMakefile\e[0m\n"
	@printf "\t\e[1mCompilando:\e[0m \e[90mtrabalhocg\e[0m\n"
	@printf "\t\e[1mSistema Operacional:\e[0m \e[90m${OSTYPE}\e[0m\n"
	@printf "\t\e[1mArquivos:\e[0m \e[90m${FIL}\e[0m\n"
	@printf "\t\e[1mWarnings:\e[0m \e[90m${WAR}\e[0m\n"
	@printf "\t\e[1mLinks:\e[0m \e[90m${FWK} ${BBM}\e[0m\n"
	@printf "\t\e[1mVersão:\e[0m \e[90mC++ 14\e[0m\n\n"
	@printf "\t\e[1;5;91mModo debug ativado\e[0m\n\n"
	@g++ ${FIL} ${WAR} ${FWK} ${ML} ${BBM} ${VER} ${OUT} -D DEBUG=true

 run:
 	./trabalhocg ./2\ -\ ArquivosGraficos/

clean:
	rm trabalhocg