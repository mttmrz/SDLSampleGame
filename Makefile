NAME = ELPEPE
FLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -g
INCLUDES = -I./SDL2

all: fclean_spec $(NAME)

$(NAME):
	@echo "Compiling..."
	@g++ *.cpp $(INCLUDES) $(FLAGS) -o $(NAME)
	@echo "Done!"

fclean_spec:
	@rm -rf $(NAME)

clean:
	@rm -rf $(NAME)

fclean: clean

re: fclean all

.PHONY: all clean fclean re fclean_spec
