#
# Projekt: proj4.tex
# Autor: Dias Assatulla
# Datum: 15.04.2022
# Email: xassat00@fit.vutbr.cz
#
SHELL = /usr/bin/env bash
NAME = proj4
LOGIN = xassat00
INCLUDE = czechiso.bst

all: bibtex or biblatex

biblatex:
	pdflatex $(NAME).tex
	biber $(NAME).bcf
	pdflatex $(NAME).tex

bibtex:
	pdflatex $(NAME).tex
	bibtex $(NAME).aux
	pdflatex $(NAME).tex
	pdflatex $(NAME).tex

