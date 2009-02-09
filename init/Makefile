DIR = init
INIT_OBJECTS = main.o 
TARGET = build_in.o

$(TARGET): $(INIT_OBJECTS)
	@$(AR) -r -o $@ $(INIT_OBJECTS) >/dev/null 2>&1
	@echo "AR $@"

.S.o:
	@$(CC) $(CCAFLAGS) -c $<
	@echo "CC $@"
.c.o:
	@$(CC) $(CFLAGS) -c $<
	@echo "CC $@"

clean:
	@$(RM) -rf $(INIT_OBJECTS) $(TARGET) *~
