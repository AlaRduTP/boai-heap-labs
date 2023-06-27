include flags.config

all: boai-shop boai-guess

boai-%: %
	$(eval FLAG := $(FLAG_$(shell echo $< | tr '[:lower:]' '[:upper:]')))
	./build.sh $< FLAG='$(FLAG)' CTF_TIMEOUT=600