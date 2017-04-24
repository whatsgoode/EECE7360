tex: *.tex
	pdflatex $< 

bib: *.aux
	bibtex $<

ps: *.dvi
	dvips $<

pdf: *.ps
	ps2pdf $<

clean:
	rm -f *bbl *dvi *blg *pdf *log *ps *aux


