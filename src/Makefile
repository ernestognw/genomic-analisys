
.DEFAULT_GOAL := run

run: 
	@echo "Running DNA analyzer"
	gcc main.c -o main.out && ./main.out
	@echo "Cleaning"
	rm main.out

server: 
	@echo "Running DNA analyzer server"
	gcc server.c -o server.out && ./server.out
	@echo "Cleaning"
	rm server.out

client: 
	@echo "Running DNA analyzer client"
	gcc client.c -o client.out && ./client.out
	@echo "Cleaning"
	rm client.out
