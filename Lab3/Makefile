CC := gcc

DIR_SRC := ./src
DIR_INC := ./inc
DIR_OUT := ./out
DIR_UTEST := ./unit_tests

FLAGS := -Wall -Werror -std=c99 -pedantic -Wextra -Wfloat-equal -Wfloat-conversion -lm -Wno-parentheses -Wno-sign-compare
UFLAGS := -lcheck -lpthread -lrt -lm
#-lsubunit

DEBUG := -O0 --debug -ggdb
GCOV := -O0 --coverage
RELEASE := -O2

MODE_FLAGS := $(DEBUG)

SRCS := $(wildcard $(DIR_SRC)/*.c)
OBJS := $(SRCS:$(DIR_SRC)/%.c=$(DIR_OUT)/%.o)
UNITS := $(wildcard $(DIR_UTEST)/*.c)

UOBJS := $(UNITS:$(DIR_UTEST)/%.c=$(DIR_OUT)/%.o)
UOBJS += $(filter-out $(DIR_OUT)/main.o, $(OBJS))
DEPEND := $(wildcard $(DIR_INC)/*.h)

ifeq ($(mode), gcov)
	MODE_FLAGS += $(GCOV)
endif
ifeq ($(mode), release)
	MODE_FLAGS += $(RELEASE)
endif

FLAGS += $(MODE_FLAGS)

.PHONY : clean func unit

app.exe : $(DIR_OUT) $(OBJS) $(DEPEND)
	$(CC) $(OBJS) -o $@

unit_tests.exe : $(DIR_OUT) $(UOBJS) $(DEPEND)
	$(CC) $(UOBJS) -o $@ $(UFLAGS)

$(DIR_OUT)/%.o : $(DIR_SRC)/%.c
	$(CC) $(FLAGS) -c $< -o $@

$(DIR_OUT)/%.o : $(DIR_UTEST)/%.c
	$(CC) $(FLAGS) -c $< -o $@ $(UFLAGS)

$(DIR_OUT) :
	@mkdir -p $@

clean :
	rm -rf $(DIR_OUT) *.exe ./func_tests/data/.temp *.gcda *.gcno

func : app.exe
	./func_tests/scripts/func_tests.sh

unit : unit_tests.exe
	./unit_tests.exe

