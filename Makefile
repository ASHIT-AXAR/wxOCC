.PHONY: clean All

All:
	@echo "----------Building project:[ wxOCETest - Debug ]----------"
	@cd "wxOCE" && $(MAKE) -f  "wxOCETest.mk"
clean:
	@echo "----------Cleaning project:[ wxOCETest - Debug ]----------"
	@cd "wxOCE" && $(MAKE) -f  "wxOCETest.mk" clean
