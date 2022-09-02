/*
	 Assatulla Dias
		xassat00
		  1BIT
	 xassat00@stud.fit.vutbr.cz
	 2 Projekt IOS
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define ERROR_DETECTED 1
#define UNLOCKED 1
#define LOCKED 0
#define SEM_NAME1 "/xassat00_sem1"
#define SEM_NAME2 "/xassat00_sem2"
#define SEM_NAME3 "/xassat00_sem3"
#define SEM_NAME4 "/xassat00_sem4"

// Structure for arguments
typedef struct {
	int NO;
	int NH;
	int TI;
	int TB;
} args;

// Pointer to a FILE for writing/saving output
FILE *out_file;

// Initializing semaphores
sem_t *sem_writing = NULL;
sem_t *sem_kyslikQueue = NULL;
sem_t *sem_vodikQueue = NULL;
sem_t *sem_mutex = NULL;

// Initializing shared variables
int *action = NULL;
int *kyslik_cnt = NULL;
int *vodik_cnt = NULL;
int *error = NULL;
int *molecula_cnt = NULL;
pid_t *pids_kyslik = NULL;
pid_t *pids_vodik = NULL;

// Initializing ID for shared vars
int shm_action = 0;
int shm_kyslik = 0;
int shm_vodik = 0;
int shm_error = 0;
int shm_molecula_cnt = 0;
pid_t shm_pids_kyslik = 0;
pid_t shm_pids_vodik = 0;

// Function to check input of the main program
args arg_check(int argc, char **argv) {
	args argums;
	char *errNO;
	char *errNH;
	char *errTI;
	char *errTB;

	if (argc != 5) {
		fprintf(stderr, "ERROR: arguments count must be only 4!\n");
		exit(1);
	}
	int NO = strtol(argv[1], &errNO, 10);
	int NH = strtol(argv[2], &errNH, 10);
	int TI = strtol(argv[3], &errTI, 10);
	int TB = strtol(argv[4], &errTB, 10);

	if ( NO <= 0 || NH <= 0 || TI < 0 || TI > 1000 || TB < 0 || TB > 1000 || 
			*errNO != '\0' || *errNH != '\0' || *errTI != '\0' || *errTB != '\0') 
	{
		fprintf(stderr, "ERROR: Wrong arguments value!\n");
		exit(1);
	}

	argums.NO = NO;
	argums.NH = NH;
	argums.TI = TI;
	argums.TB = TB;
	return argums;

}
// Functions to clear all semaphores, FILE and shared memory
void unlink_semaphores(void) {
	sem_close(sem_writing);
	sem_close(sem_kyslikQueue);
	sem_close(sem_vodikQueue);
	sem_close(sem_mutex);

	sem_unlink(SEM_NAME1);
	sem_unlink(SEM_NAME2);
	sem_unlink(SEM_NAME3);
	sem_unlink(SEM_NAME4);
}

void clean_all(void) {
	unlink_semaphores();
	fclose(out_file);
	shmctl(shm_action,IPC_RMID,NULL);
	shmctl(shm_kyslik,IPC_RMID,NULL);
	shmctl(shm_vodik,IPC_RMID,NULL);
	shmctl(shm_error,IPC_RMID,NULL);
	shmctl(shm_pids_kyslik, IPC_RMID, NULL);
	shmctl(shm_pids_vodik, IPC_RMID, NULL);
	shmctl(shm_molecula_cnt, IPC_RMID, NULL);
}

// Initializing all shared mem and semaphores
void init(args argums) {
	// Initialize semaphores and check them
	sem_writing = sem_open(SEM_NAME1, O_CREAT | O_EXCL, 0666, UNLOCKED);
	sem_vodikQueue = sem_open(SEM_NAME2, O_CREAT | O_EXCL, 0666, LOCKED); // H - only 2 needed
	sem_kyslikQueue = sem_open(SEM_NAME3, O_CREAT | O_EXCL, 0666, LOCKED); // O - only 1 needed
	sem_mutex = sem_open(SEM_NAME4, O_CREAT | O_EXCL, 0666, UNLOCKED);


	if (sem_writing == SEM_FAILED || sem_kyslikQueue == SEM_FAILED || sem_kyslikQueue == SEM_FAILED || sem_mutex == SEM_FAILED ) {
		fclose(out_file);
		unlink_semaphores();
		fprintf(stderr, "ERROR: can't make semaphore.\n");
		exit(1);
	}


	shm_action = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
	shm_error = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
	shm_vodik = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
	shm_kyslik = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
	shm_molecula_cnt = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
	shm_pids_vodik = shmget(IPC_PRIVATE, argums.NH * sizeof(pid_t), IPC_CREAT | 0666);
	shm_pids_kyslik = shmget(IPC_PRIVATE, argums.NO * sizeof(pid_t), IPC_CREAT | 0666);

	if (shm_action == -1 || shm_error == -1 || shm_kyslik == -1 || shm_vodik == -1 || shm_pids_vodik == -1 || shm_pids_kyslik == -1 || shm_molecula_cnt == -1) {
		clean_all();
		fprintf(stderr, "ERROR: Can't initialize shared memory.\n");
		exit(1);
	}

	// Mapping shared memory
	action = shmat(shm_action,NULL,0);
	error = shmat(shm_error,NULL,0);
	vodik_cnt = shmat(shm_vodik,NULL,0);
	kyslik_cnt = shmat(shm_kyslik,NULL,0);
	pids_kyslik = shmat(shm_pids_kyslik, NULL, 0);
	pids_vodik = shmat(shm_pids_vodik, NULL, 0);
	molecula_cnt = shmat(shm_molecula_cnt, NULL, 0);

	if(action == NULL || error == NULL || vodik_cnt == NULL || kyslik_cnt == NULL || pids_kyslik == NULL || pids_vodik == NULL || molecula_cnt == NULL) {
		clean_all();
		fprintf(stderr, "ERROR: Can't map shared memory.\n");
		exit(1);
	}
}

void kyslik_function(args argums, int id) {
	sem_wait(sem_writing);
	(*action)++;
	fprintf(out_file, "%d: O %d: started\n", *action, id);
	sem_post(sem_writing);
	if (argums.TI != 0) 
	     usleep((rand() % argums.TI) * 1000);

	sem_wait(sem_writing);
	(*action)++;
	fprintf(out_file, "%d: O %d: going to queue\n", *action, id);
	sem_post(sem_writing);

	sem_wait(sem_mutex);
	if((*vodik_cnt) >= 2) {
		sem_post(sem_vodikQueue);
		sem_post(sem_vodikQueue);
		(*vodik_cnt) -= 2;		
		(*kyslik_cnt) -= 1;
		sem_post(sem_kyslikQueue);
	}
	else
		sem_post(sem_mutex);
	
	sem_wait(sem_writing);
	(*molecula_cnt)++;
	(*action)++;
	fprintf(out_file, "%d: O %d: creating molecule %d\n", *action, id, *molecula_cnt);
	sem_post(sem_writing);
	if (argums.TB != 0) 
		 usleep((rand() % argums.TB) * 1000);
	
	sem_wait(sem_writing);
	(*action)++;
	fprintf(out_file, "%d: O %d: molecule %d created\n", *action, id, *molecula_cnt);
	sem_post(sem_writing);
	sem_post(sem_mutex);
	exit(0);
}

void vodik_function(args argums, int id) {
	sem_wait(sem_writing);
	(*action)++;
	fprintf(out_file, "%d: H %d: started\n", *action, id);
	sem_post(sem_writing);
	if (argums.TI != 0) 
	     usleep((rand() % argums.TI) * 1000);
	
	sem_wait(sem_writing);
	(*action)++;
	fprintf(out_file, "%d: H %d: going to queue\n", *action, id);
	sem_post(sem_writing);

	sem_wait(sem_mutex);
	if((*vodik_cnt) >= 2 && (*kyslik_cnt) >= 1) {	
			
		sem_post(sem_vodikQueue);
		sem_post(sem_vodikQueue);
		(*vodik_cnt) -= 2;		
		(*kyslik_cnt) -= 1;
		sem_post(sem_kyslikQueue);
	}
	else
		sem_post(sem_mutex);
	sem_wait(sem_writing);
	(*action)++;
	fprintf(out_file, "%d: H %d: creating molecule %d\n", *action, id, *molecula_cnt);
	sem_post(sem_writing);
	if (argums.TB != 0) 
		 usleep((rand() % argums.TB) * 1000);
	
	sem_wait(sem_writing);
	(*action)++;
	fprintf(out_file, "%d: H %d: molecule %d created\n", *action, id, *molecula_cnt);
	sem_post(sem_writing);
	sem_post(sem_mutex);
	exit(0);
}



int main(int argc, char **argv) {
	args argums = arg_check(argc,argv);
	out_file = fopen("proj2.out", "w");
	if (out_file == NULL) {
		fprintf(stderr, "ERROR: Can not open the file.\n");
		exit(1);
	}
	// code for init semafor and sharem mem
	init(argums);
	setbuf(out_file, NULL);
	srand(time(NULL));
	// end of code init
	
	pid_t main_proces = fork();

	if(main_proces == 0) {
		pid_t component_process = fork();
		if(component_process == 0) { // Kyslik gen
			pid_t process_kyslik;
			for(int id = 1; id <= argums.NO; id++) {
				process_kyslik = fork();
				srand(getpid());
				if(process_kyslik == 0) {
					(*kyslik_cnt)++;
					kyslik_function(argums,id);
				}
				else if (process_kyslik < 0) {
					*error = ERROR_DETECTED;
					fprintf(stderr, "ERROR: unable to create process vodik.\n");
					exit(1);
				}
				pids_kyslik[id - 1] = process_kyslik;
			}
			for (int i = 0; i < argums.NO; i++){
            	waitpid(pids_kyslik[i], NULL, 0);
        	}
			exit(0);
		}
		else { // Vodik gen
			pid_t process_vodik;
			for (int id = 1; id <= argums.NH; id++) {
				process_vodik = fork();
				srand(getpid());
				if(process_vodik == 0) {
					(*vodik_cnt)++;
					vodik_function(argums,id);
				}
				else if (process_vodik < 0) {
					*error = ERROR_DETECTED;
					fprintf(stderr, "ERROR: unable to create process vodik.\n");
					exit(1);
				}

				pids_vodik[id - 1] = process_vodik;
			}
			for (int i = 0; i < argums.NH; i++){
            	waitpid(pids_vodik[i], NULL, 0);
        	}
			exit(0);
		}
	}
	else if (main_proces < 0) {
		clean_all();
		fprintf(stderr, "ERROR: bad process generation.\n");
		exit(1);
	}

	waitpid(main_proces, NULL, 0);
	clean_all();
	if (*error == ERROR_DETECTED)
		exit(1);
	else
		exit(0);
}
