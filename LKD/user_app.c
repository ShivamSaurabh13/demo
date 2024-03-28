#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>

#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)

int main(){
	int ch;
	int fd = open("/dev/real_device",O_RDWR);
	int8_t write_buff[1024];
	int8_t read_buff[1024];
	
	int32_t number,value;
	if(fd < 0){
		printf("Cannot open the device file\n");
		return 0;
	}
	while(1){
		printf("Enter your choice\n1.Write\n2.Read\n3.IOCTL\n4.Exit\n");
		scanf("%d",&ch);
		switch(ch){
			case 1: printf("Enter the string to send\n");
			//fgets(write_buff,stdin,1024);
			scanf(" %[^\n\t]s",write_buff);
			printf("Data writing\n");
			write(fd,write_buff,strlen(write_buff)+1);
			printf("Done");
			break;
			case 2: printf("Data Reading\n");
				read(fd,read_buff,1024);
				printf("Done\nData is :\n");
				printf("%s \n",read_buff);
				break;
			case 3:	printf("Enter the value to send\n");
				scanf("%d",&number);
				printf("Writing value to driver\n");
				ioctl(fd,WR_VALUE,(int32_t*)&number);
				printf("Reading value from driver\n");
				ioctl(fd,RD_VALUE,(int32_t*)&value);
				printf("Value = %d\n",value);
				printf("Done\n");
				break;
				
			case 4: close(fd);
				exit(1);
				break;
			default:printf("Please enter the valid data\n");
			break;
		}
	}
	close(fd);
}


