/**
 * @author: Marcel Feiler
 * @file hinfosvc.c
 * @name Info Server
 * @date 10.02.2022
*/

//////////Libraries//////////
#include <stdio.h>
#include <stdlib.h>
//#include <arpa/inet.h>
#include <netinet/in.h>
//#include <sys/types.h>
#include <sys/socket.h>
//#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
/////////////////////////////

//////////Functions//////////
char* host_name(char *result);
char* cpu_info(char *result);
char* cpu_usage(char *result);
/////////////////////////////


/**
 * Calling functions host_name, cpu_info or cpu_usage in case of request
 * @brief Main function calling its sub-functions
 * param argc[in] Represents number of arguments
 * param argv[in] An array of given arguments
 * @return Returns 0 in case of successful run, otherwise returns 1
 */ 

int main(int argc, char const *argv[]) {
	int server_fd, new_socket; // valread;
	struct sockaddr_in address;
	
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};

	if (argc > 2) {
		printf("Wrong amount of parametres.");
		return 1;
	}

	//Create socket file descr
	if ((server_fd  = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0) {
		perror("Socket Failed");
		exit(EXIT_FAILURE);
		//return 1;
	}

	//Attaching socket to the port
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("Setsockopt Failed");
		exit(EXIT_FAILURE);
	}

	int your_port = atoi(argv[1]);
	if (your_port == 0) {
		perror("Atoi conversion failure");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(your_port);
	address.sin_addr.s_addr = INADDR_ANY;

	//Binding
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 10) < 0) {
		perror("listen failed");
		exit(EXIT_FAILURE);
	}

	while (true) {
		if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
			perror("Cannot Accept");
			exit(EXIT_FAILURE);
		}

		read(new_socket, buffer, 1024); //int valread = 		
		printf("%s\n", buffer);

		if (strncmp(buffer,"GET /hostname ", 14) == 0) {
			char hostname[200];
			char headname[200] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n";

			host_name(hostname);
			sprintf(headname, "%s", hostname);
			send(new_socket, headname, strlen(headname), 0);
		}

		else if (strncmp(buffer,"GET /cpu-name ", 14) == 0) {
			char cpu_name[200];
			char head2_name[200] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n";

			cpu_info(cpu_name);
			sprintf(head2_name, "%s", cpu_name);
			send(new_socket, head2_name, strlen(head2_name), 0);
		}

		else if (strncmp(buffer,"GET /load ", 10) == 0) {
			char load[200];
			char head3_name[200] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n";

			cpu_usage(load);
			sprintf(head3_name, "%s", load);
			send(new_socket, head3_name, strlen(head3_name), 0);
		}

		else {
			char msg[200] = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain;\r\n\r\nBad Request\n";
			send(new_socket, msg, strlen(msg), 0);
		}

		close(new_socket);
	}


	return 0;
}



//////////Sub-functions//////////

/**
 * Parsing host name from cat output to temporary variable
 * @brief Storing host name from cat output
 * @param result[in,out] in-out 'string' parameter containing host name
 * @return Return result which represents host name  
 */

char* host_name(char *result) {
	FILE *file;
	file = popen("cat /proc/sys/kernel/hostname", "r");
	fgets(result, 500, file);
	pclose(file);
	return result;
}

/**
 * Parsing cpu name from cat output to temporary variable
 * @brief Storing cpu name from cat output
 * @param result[in,out] in-out 'string' parameter containing cpu name
 * @return Return result which represents a CPU name  
 */

char* cpu_info(char *result) {
	FILE *file;
	file = popen("cat /proc/cpuinfo | grep 'model name' | head -n 1 | awk -F \": \" '{print $2}'", "r");
	fgets(result, 500, file);
	pclose(file);
	return result;	
}

/**
 * Counting a CPU usage using many different values and finally converting it to 'string' variable
 * @brief Counting a CPU usage and storing value of it
 * @param result[in,out] in-out 'string' parameter containing final value of CPU usage in %
 * @return Return result which represents a CPU usage 
 */

char* cpu_usage(char *result) {
	int k = 0;
	int j = 0;
	int i = 0;
	FILE *prev_file;
	FILE *current_file;
	char row[300], row2[300];
	char values[200][200], values2[200][200];
	char *eptr;
	long long int usage[200], usage2[200];
	
	prev_file = popen("cat /proc/stat | grep 'cpu' | head -n 1", "r");
	fgets(row, 500, prev_file);
	
	for (unsigned i = 0; i <= strlen(row); i++) {
		if (row[i] != ' ' && row[i] != '\n' && row[i] != '\0') {
			values[k][j] = row[i];
			j++;
		}

		else {
			j = 0;
			k++;
		}
	}

	for (i = 2; i <= k-1; i++) {
		usage[i] = strtoll(values[i], &eptr, 10);
	}

	pclose(prev_file);

	sleep(1);

	current_file = popen("cat /proc/stat", "r");
	fgets(row2, 500, current_file);
	
	k = 0;
	j = 0;

	for (unsigned i = 0; i <= strlen(row2); i++) {
		if (row2[i] != ' ' && row2[i] != '\n' && row[i] != '\0') {
			values2[k][j] = row2[i];
			j++;
		}

		else {
			j = 0;
			k++;
		}
	}

	for (i = 2; i <= k-1; i++) {
		usage2[i] = strtoll(values2[i], &eptr, 10);
	}

	pclose(current_file);

	///////Counting a CPU usage/////////
	long long int prev_idle = usage[5] + usage[6];
	long long int idle = usage2[5] + usage2[6];

	long long int prev_non_idle = usage[2] + usage[3] + usage[4] + usage[7] + usage[8] + usage[9];
	long long int non_idle = usage2[2] + usage2[3] + usage2[4] + usage2[7] + usage2[8] + usage2[9];

	long long int prev_total = prev_idle + prev_non_idle;
	long long int total = idle + non_idle;

	long long int totald = total - prev_total;
	long long int idled = idle - prev_idle;

	double cpu_perc = (totald - idled) / (double)totald;
	cpu_perc *= 100;
	snprintf(result, 500, "%.2lf%%\n", cpu_perc);

	return result;
}