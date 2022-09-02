#
# Projekt: proj1.tex
# Autor: Dias Assatulla
# Datum: 02.03.2022
# Email: xassat00@fit.vutbr.cz
#
SHELL = /usr/bin/env bash
NAME = proj1
LOGIN = xassat00

all:
	latex $(NAME).tex
	dvips -t at $(NAME).dvi
	ps2pdf -sPAPERSIZE=a4 $(NAME).ps

zip:
	zip $(LOGIN)-fit.zip *.tex Makefile

clean:
	rm -f $(NAME).{aux,dvi,log,pdf,ps} $(LOGIN)-fit.zip
