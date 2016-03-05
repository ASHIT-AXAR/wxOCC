.PHONY: clean All

All:
	@echo "----------Building project:[ wxOCE - Debug ]----------"
	@cd "wxOCE" && $(MAKE) -f  "wxOCE.mk"
clean:
	@echo "----------Cleaning project:[ wxOCE - Debug ]----------"
	@cd "wxOCE" && $(MAKE) -f  "wxOCE.mk" clean
