
.DEFAULT_GOAL := run

server: 
	@echo "Running DNA analyzer server"
	gcc -fopenmp  server.c -lm  -o server.out && ./server.out
	@echo "Cleaning"
	rm server.out


client: 
	@echo "Running DNA analyzer client"
	gcc client.c -o client.out && ./client.out
	@echo "Cleaning"
	rm client.out
