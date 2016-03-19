.PHONY: clean All

All:
	@echo "----------Building project:[ wxOCC - Debug ]----------"
	@cd "wxOCC" && "$(MAKE)" -f  "wxOCC.mk"
clean:
	@echo "----------Cleaning project:[ wxOCC - Debug ]----------"
	@cd "wxOCC" && "$(MAKE)" -f  "wxOCC.mk" clean
