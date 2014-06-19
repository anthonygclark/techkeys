# Make sure BINARY is set to the absolute path of the output hex file
# Make sure MISC_PATH is the absolute or relative path of the misc dir
flash:
	@bash $$(readlink -m $(MISC_PATH))/flash.sh $$(readlink -m $(HEX)) 
