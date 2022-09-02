#
# Projekt: proj3.tex
# Autor: Dias Assatulla
# Datum: 01.04.2022
# Email: xassat00@fit.vutbr.cz
#
SHELL = /usr/bin/env bash
NAME = proj3
LOGIN = xassat00

all:
	latex $(NAME).tex
	latex $(NAME).tex
	dvips -t a4 $(NAME).dvi
	ps2pdf -sPAPERSIZE=a4 $(NAME).ps

zip:
	zip $(LOGIN)-fit.zip *.tex Makefile  

clean:
	rm -f $(NAME).{aux,dvi,log,pdf,ps} $(LOGIN)-fit.zip
