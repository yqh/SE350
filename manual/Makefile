.SILENT:

.PHONY: all clean

LATEX = pdflatex
PAPER = main_book
OUTPUT = se350_manual.pdf

main:
	$(LATEX) $(PAPER)
	bibtex $(PAPER)
	$(LATEX) $(PAPER)
#	mv $(PAPER).pdf $(OUTPUT)
	cp $(PAPER).pdf $(OUTPUT)

clean:
	rm -f *~ $(PAPER).dvi $(PAPER).log $(PAPER).pdf $(PAPER).ind $(OUTPUT)
	rm -f $(PAPER).bbl $(PAPER).blg $(PAPER).ps $(PAPER).aux
	rm -f $(PAPER).out $(PAPER).lot $(PAPER).lof $(PAPER).toc
