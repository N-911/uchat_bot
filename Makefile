NAME_BOT = bot

SRCD = src
INCD = inc
OBJD = obj

LJSOND	= libjson
LJSONX  = libjsonc.a
LJSONA := $(addprefix $(LJSOND)/, $(LJSONX))

INCS = inc/uchat.h

SRC_BOT = main_bot.c \
	start_bot.c \
	start_bot2.c \

SRC_HELP = err_exit.c \
	functions.c \
	functions2.c \
	functions3.c \
	crypto.c \
	json_functions.c \
	json_short.c \
	json_short2.c \
	json_short3.c

# BD = src/functions/create_bd.c

SRCS_BOT = $(addprefix $(SRCD)/bot/, $(SRC_BOT))
SRCS_HELP = $(addprefix $(SRCD)/functions/, $(SRC_HELP))

OBJS_BOT = $(addprefix $(OBJD)/, $(SRC_BOT:%.c=%.o))
OBJS_HELP = $(addprefix $(OBJD)/, $(SRC_HELP:%.c=%.o))

CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic -g -fsanitize=address

LIBRESSL_A = ./libressl/tls/.libs/libtls.a \
			 ./libressl/ssl/.libs/libssl.a \
			 ./libressl/crypto/.libs/libcrypto.a

LIBRESSL_H = \
			-I ./libressl/include/tls.h \
			-I ./libressl/include/openssl \
			-I ./libressl/include/pqueue.h \
			-I ./libressl/tls \
			-I ./libressl/ssl \
			-I ./libressl/crypto

# SQLFLAGS = -lsqlite3

all: install

bot: $(NAME_S)

$(NAME_BOT): $(LMXA) $(OBJS_BOT) $(OBJS_HELP)
# 	@clang -lsqlite3 -o create $(BD) && ./create server_db.bin
# 	@rm -rf create
	@make -sC $(LJSOND)
	@clang $(CFLAGS)  $(LJSONA) $(LIBRESSL_H) $(LIBRESSL_A) \
	       $(OBJS_BOT) $(OBJS_HELP) -o $@  $(SQLFLAGS)
	@printf "\r\33[2K$@\t   \033[32;1mcreated\033[0m\n"

$(OBJD)/%.o: src/bot/%.c $(INCS)
	@clang $(CFLAGS)  -o $@ -c $< -I$(INCD) -I$(LMXI)
	@printf "\r\33[2K\033[37;1mcompile \033[0m$(<:$(SRCD)/%.c=%) "

$(OBJD)/%.o: src/functions/%.c $(INCS)
	@clang $(CFLAGS)  -o $@ -c $< -I$(INCD) -I$(LMXI)
	@printf "\r\33[2K\033[37;1mcompile \033[0m$(<:$(SRCD)/%.c=%) "

$(OBJS_BOT): | $(OBJD)
$(OBJS_HELP): | $(OBJD)

$(OBJD):
	@mkdir -pv $@

$(LJSONA):
	@make -sC $(LJSOND)

$(LJSONX): $(LJSONA)
	@make -sC $(LJSOND)


install:  bot

clean:
	@make -sC ./libjson clean
	@rm -rf $(OBJD)
	@printf "$(OBJD)\t\t   \033[31;1mdeleted\033[0m\n"

uninstall: clean
	@make -sC ./libjson uninstall
	@rm -rf $(NAME_BOT)
	@printf "$(NAME_BOT)\t   \033[31;1muninstalled\033[0m\n"

reinstall: uninstall install