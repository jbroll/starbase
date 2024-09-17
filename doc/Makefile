WU_FILES := $(wildcard *.wu)
MD_FILES := $(patsubst %.wu,../docs/%.md,$(WU_FILES))

WI_FILES := $(notdir $(wildcard ./include/*.wi))
MI_FILES := $(patsubst %.wi,../docs/_includes/%.md,$(WI_FILES))

all: $(MD_FILES) $(MI_FILES)

../docs/%.md: %.wu wu2markdown.tcl
	@cat $< | expand | ./wu2markdown.tcl > $@

../docs/_includes/%.md: include/%.wi wu2markdown.tcl
	@cat $< | expand | ./wu2markdown.tcl > $@

.PHONY: all

