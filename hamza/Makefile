CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD -MP

NAME = ircserv
TEST = pmsg

SRCS = Server.cpp Clinet.cpp main.cpp
TEST_SRCS = Server.cpp Clinet.cpp main.cpp
HEADERS = Server.hpp Client.hpp

OBJS = $(SRCS:.cpp=.o)
TEST_OBJS = $(TEST_SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d) $(TEST_OBJS:.o=.d)


all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TEST) $^

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TEST_OBJS) $(DEPS) $(TEST)

fclean: clean
	rm -f $(NAME)

re: fclean all
.PHONY: all clean fclean re test

-include $(DEPS)