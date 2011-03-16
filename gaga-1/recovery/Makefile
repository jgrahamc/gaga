# Makefile to control building of Python code and upload to GM862
#
# Copyright (c) 2010 John Graham-Cumming
#
# all: Builds all out of date pyo files
# upload: Uploads to GM862

SRC := gaga-1.py at.py sms.py gps2.py phone.py logger.py util.py
OBJ := $(SRC:.py=.pyo)

%.pyo: %.py
	@echo Making $@
	@python -O -m py_compile $<

.PHONY: all clean
all: $(OBJ)

clean:
	@rm $(OBJ)
	@rm $(UPL)

TO_UPL := $(OBJ)
UPL := $(addprefix .upload-,$(TO_UPL))

.PHONY: upload
upload: $(UPL)

.upload-%: %
	@echo Uploading $<
	@./upload.pl $< $(if $(subst $(firstword $(TO_UPL)),,$<),,1)
	@touch $@

.PHONY: test
test:
	@python gaga-1.py
