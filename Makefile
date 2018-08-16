# Makefile
# id150325d		

CC = gcc
SRCDIR = src
HDIR = h
OBJDIR = obj
ARCHFLAG = -m32
CFLAGS = -c -MMD -Wall -std=c11
DEBUGFLAG = -g
PROJECTROOT = ..
CLIBS = 
BINDIR = $(PROJECTROOT)/bin

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

BIN = prog


$(BIN):	$(BINDIR) $(OBJDIR) $(OBJ)
	$(CC) -o $(BINDIR)/$(BIN) $(OBJ) $(CLIBS) $(ARCHFLAG)
	
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(DEBUGFLAG) $(ARCHFLAG) -I $(HDIR) -o $@ $< 

$(BINDIR):      
	mkdir -p $(BINDIR)
$(OBJDIR):	
	mkdir -p $(OBJDIR)
	

-include $(OBJDIR)/*.d

.PHONY: clean
clean:
	rm -rf $(OBJDIR)
