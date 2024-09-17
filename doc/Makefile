WU_FILES := $(wildcard *.wu)
MD_FILES := $(patsubst %.wu,../docs/%.md,$(WU_FILES))

WI_FILES := $(notdir $(wildcard ./include/*.wi))
MI_FILES := $(patsubst %.wi,../docs/_includes/%.md,$(WI_FILES))

DOC_HTML_FILES := $(wildcard *.html)
DOCS_HTML_FILES := $(patsubst %.html,../docs/%.html,$(DOC_HTML_FILES))

all: $(MD_FILES) $(MI_FILES) ../docs/BUGS.html ../docs/README.html $(DOCS_HTML_FILES)

../docs/%.html: %.html
	cp $< $@

../docs/README.html: ../README
	echo "<pre>" > ../docs/README.html
	cat ../README  >> ../docs/README.html
	echo "<pre>" >> ../docs/README.html

../docs/BUGS.html: ../BUGS
	echo "<pre>" > ../docs/BUGS.html
	cat ../BUGS  >> ../docs/BUGS.html
	echo "<pre>" >> ../docs/BUGS.html

../docs/%.md: %.wu wu2markdown.tcl
	@cat $< | expand | ./wu2markdown.tcl > $@

../docs/_includes/%.md: include/%.wi wu2markdown.tcl
	@cat $< | expand | ./wu2markdown.tcl > $@

.PHONY: all

