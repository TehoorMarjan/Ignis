# Makefile for exporting multi-page SVGs to PNGs using Inkscape CLI

# Variables
SVG_SRC_DIR := src
PNG_OUT_DIR := png

# Define SVG->directory mappings  
GRAY_SVG := TheoToys_Gray
MONO_SVG := TheoToys_Mono
COLOR_SVG := TheoToys

GRAY_DIR := $(PNG_OUT_DIR)/gray
MONO_DIR := $(PNG_OUT_DIR)/mono
COLOR_DIR := $(PNG_OUT_DIR)/color

PAGE_NAMES := Under Side
DPI := 300
LOGO_SIZES := 32 48 64 128 256

# Generate all targets
GRAY_PAGES := $(foreach page,$(PAGE_NAMES),$(GRAY_DIR)/$(GRAY_SVG)_$(page).png)
MONO_PAGES := $(foreach page,$(PAGE_NAMES),$(MONO_DIR)/$(MONO_SVG)_$(page).png)
COLOR_PAGES := $(foreach page,$(PAGE_NAMES),$(COLOR_DIR)/$(COLOR_SVG)_$(page).png)

GRAY_LOGOS := $(foreach size,$(LOGO_SIZES),$(GRAY_DIR)/$(GRAY_SVG)_logo_$(size).png)
MONO_LOGOS := $(foreach size,$(LOGO_SIZES),$(MONO_DIR)/$(MONO_SVG)_logo_$(size).png)
COLOR_LOGOS := $(foreach size,$(LOGO_SIZES),$(COLOR_DIR)/$(COLOR_SVG)_logo_$(size).png)

# Default target
.PHONY: all
all: $(GRAY_PAGES) $(MONO_PAGES) $(COLOR_PAGES) $(GRAY_LOGOS) $(MONO_LOGOS) $(COLOR_LOGOS)

# Generic page export rules using functions
define page_rule
$(1)/$(2)_Under.png: $(SVG_SRC_DIR)/$(2).svg | $(1)
	inkscape --export-type=png --export-filename=$$@ --export-dpi=$(DPI) --export-page=1 $$<

$(1)/$(2)_Side.png: $(SVG_SRC_DIR)/$(2).svg | $(1)
	inkscape --export-type=png --export-filename=$$@ --export-dpi=$(DPI) --export-page=3 $$<

$(1)/$(2)_logo_%.png: $(SVG_SRC_DIR)/$(2).svg | $(1)
	inkscape --export-type=png --export-filename=$$@ --export-width=$$* --export-height=$$* --export-page=2 $$<
endef

# Generate rules for each SVG/directory combination
$(eval $(call page_rule,$(GRAY_DIR),$(GRAY_SVG)))
$(eval $(call page_rule,$(MONO_DIR),$(MONO_SVG)))
$(eval $(call page_rule,$(COLOR_DIR),$(COLOR_SVG)))

# Create output directories if not exist
$(GRAY_DIR) $(MONO_DIR) $(COLOR_DIR):
	mkdir -p $@

# Clean all generated PNGs
.PHONY: clean
clean:
	rm -rf $(PNG_OUT_DIR)/*/
