DIR = lib 
LIB_OBJECTS = string.o printk.o 
TARGET = build_in.o

$(TARGET): $(LIB_OBJECTS)
	@$(AR) -r -o $@ $(LIB_OBJECTS) >/dev/null 2>&1
	@echo "AR $@"

.S.o:
	@$(CC) $(CCAFLAGS) -c $<
	@echo "CC $@"
.c.o:
	@$(CC) $(CFLAGS) -c $<
	@echo "CC $@"

clean:
	@$(RM) -rf $(LIB_OBJECTS) $(TARGET)
