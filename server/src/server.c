#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<signal.h>
#include<fcntl.h>

// Include GPIO control
#include "gpiorasp2.h"

#define CONNMAX 1000
#define BYTES 1024

// Define the pin number for each output
// the L at the begining means light and the D door
// B stads for button
#define LLIVINGROOM 5
#define LDINNIGROOM 6
#define LKITCHEN 13
#define LMASTERBEDROOM 19
#define LBEDROOM 26
#define BLIVINGROOM 25
#define BDINIGROOM 8
#define BKITCHEN 7
#define BMASTERBEDROOM 9
#define BBEDROOM 11
#define DFRONT 2
#define DBACK 3
#define DBEDROOM 4
#define DMASTERBEDROOM 14

// State variables
int llrstate = 0;
int ldrstate = 0;
int lkstate = 0;
int lmbrstate = 0;
int lbrstate = 0;
int dbstate = 0;
int dfstate = 0;
int dbrstate = 0;
int dmbrstate = 0;


char *ROOT;
int listenfd, clients[CONNMAX];
void error(char *);
void startServer(char *);
void respond(int);
void execute(int);

int main(int argc, char* argv[])
{
	struct sockaddr_in clientaddr;
	socklen_t addrlen;
	char c;    
	
	//Default Values PATH = ~/ and PORT=10000
	char PORT[6];
	ROOT = getenv("PWD");
	strcpy(PORT,"10000");

	int slot=0;

	// Enable pins
	GPIOExport(LLIVINGROOM);
	GPIOExport(LDINNIGROOM);
	GPIOExport(LKITCHEN);
	GPIOExport(LMASTERBEDROOM);
	GPIOExport(LBEDROOM);
	GPIOExport(BLIVINGROOM);
	GPIOExport(BDINIGROOM);
	GPIOExport(BKITCHEN);
	GPIOExport(BMASTERBEDROOM);
	GPIOExport(BBEDROOM);
	GPIOExport(DFRONT);
	GPIOExport(DBEDROOM);
	GPIOExport(DMASTERBEDROOM);
	GPIOExport(DBACK);

	// Set GPIO mode
	pinMode(LLIVINGROOM, OUT);
	pinMode(LDINNIGROOM, OUT);
	pinMode(LKITCHEN, OUT);
	pinMode(LMASTERBEDROOM, OUT);
	pinMode(LBEDROOM, OUT);
	pinMode(BLIVINGROOM, IN);
	pinMode(BDINIGROOM, IN);
	pinMode(BKITCHEN, IN);
	pinMode(BMASTERBEDROOM, IN);
	pinMode(BBEDROOM, IN);
	pinMode(DFRONT, IN);
	pinMode(DBEDROOM, IN);
	pinMode(DMASTERBEDROOM, IN);
	pinMode(DBACK, IN);
	



	//Parsing the command line arguments
	while ((c = getopt (argc, argv, "p:r:")) != -1)
		switch (c)
		{
			case 'r':
				ROOT = malloc(strlen(optarg));
				strcpy(ROOT,optarg);
				break;
			case 'p':
				strcpy(PORT,optarg);
				break;
			case '?':
				fprintf(stderr,"Wrong arguments given!!!\n");
				exit(1);
			default:
				exit(1);
		}
	
	printf("Server started at port no. %s%s%s with root directory as %s%s%s\n","\033[92m",PORT,"\033[0m","\033[92m",ROOT,"\033[0m");
	// Setting all elements to -1: signifies there is no client connected
	int i;
	for (i=0; i<CONNMAX; i++)
		clients[i]=-1;
	startServer(PORT);

	// ACCEPT connections
	while (1)
	{
		addrlen = sizeof(clientaddr);
		clients[slot] = accept (listenfd, (struct sockaddr *) &clientaddr, &addrlen);

		if (clients[slot]<0)
			error ("accept() error");
		else
		{
			if ( fork()==0 )
			{
				respond(slot);
				exit(0);
			}
		}

		while (clients[slot]!=-1) slot = (slot+1)%CONNMAX;

		// Read signals
		int readfd = digitalRead(DFRONT);
		int readb = digitalRead(DBACK);
		int readmbrd = digitalRead(DMASTERBEDROOM);
		int readbd = digitalRead(DBEDROOM);
		int readldr = digitalRead(LDINNIGROOM);
		int readllr = digitalRead(LLIVINGROOM);
		int readlk = digitalRead(LKITCHEN);
		int readlmbr = digitalRead(LMASTERBEDROOM);
		int readlbr = digitalRead(LBEDROOM);

		// Change the dinning room light
		if (readldr != 0){
			ldrstate = !ldrstate;
			digitalWrite(LDINNIGROOM, ldrstate);
		}
		// Change the living room light
		if (readllr != 0){
			llrstate = !llrstate;
			digitalWrite(LLIVINGROOM, llrstate);
		}
		// Change the kitchen light
		if (readlk != 0){
			lkstate = !lkstate;
			digitalWrite(LKITCHEN, lkstate);
		}
		// Change the bedroom light
		if (readlbr != 0){
			lbrstate = !lbrstate;
			digitalWrite(LBEDROOM, lbrstate);
		}
		// Change the master bedroom light
		if (readlmbr != 0){
			lmbrstate = !lmbrstate;
			digitalWrite(LMASTERBEDROOM, lmbrstate);
		}

		

	}

	return 0;
}

//start server
void startServer(char *port)
{
	struct addrinfo hints, *res, *p;

	// getaddrinfo for host
	memset (&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo( NULL, port, &hints, &res) != 0)
	{
		perror ("getaddrinfo() error");
		exit(1);
	}
	// socket and bind
	for (p = res; p!=NULL; p=p->ai_next)
	{
		listenfd = socket (p->ai_family, p->ai_socktype, 0);
		if (listenfd == -1) continue;
		if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) break;
	}
	if (p==NULL)
	{
		perror ("socket() or bind()");
		exit(1);
	}

	freeaddrinfo(res);

	// listen for incoming connections
	if ( listen (listenfd, 1000000) != 0 )
	{
		perror("listen() error");
		exit(1);
	}
}

//client connection
void respond(int n)
{
	char mesg[99999], *reqline[3], data_to_send[BYTES], path[99999];
	int rcvd, fd, bytes_read;

	memset( (void*)mesg, (int)'\0', 99999 );

	rcvd=recv(clients[n], mesg, 99999, 0);

	if (rcvd<0)    // receive error
		fprintf(stderr,("recv() error\n"));
	else if (rcvd==0)    // receive socket closed
		fprintf(stderr,"Client disconnected upexpectedly.\n");
	else    // message received
	{
		printf("%s", mesg);
		execute((int) mesg);
		reqline[0] = strtok (mesg, " \t\n");
		if ( strncmp(reqline[0], "GET\0", 4)==0 )
		{
			reqline[1] = strtok (NULL, " \t");
			reqline[2] = strtok (NULL, " \t\n");
			if ( strncmp( reqline[2], "HTTP/1.0", 8)!=0 && strncmp( reqline[2], "HTTP/1.1", 8)!=0 )
			{
				write(clients[n], "HTTP/1.0 400 Bad Request\n", 25);
			}
			else
			{
				if ( strncmp(reqline[1], "/\0", 2)==0 )
					reqline[1] = "/index.html";        //Because if no file is specified, index.html will be opened by default (like it happens in APACHE...

				strcpy(path, ROOT);
				strcpy(&path[strlen(ROOT)], reqline[1]);
				printf("file: %s\n", path);

				if ( (fd=open(path, O_RDONLY))!=-1 )    //FILE FOUND
				{
					send(clients[n], "HTTP/1.0 200 OK\n\n", 17, 0);
					while ( (bytes_read=read(fd, data_to_send, BYTES))>0 )
						write (clients[n], data_to_send, bytes_read);
				}
				else    write(clients[n], "HTTP/1.0 404 Not Found\n", 23); //FILE NOT FOUND
			}
		}
	}

	//Closing SOCKET
	shutdown (clients[n], SHUT_RDWR);         //All further send and recieve operations are DISABLED...
	close(clients[n]);
	clients[n]=-1;
}






/**
 * Recive the msg from the server and execute the desired action
 * command: Action to execute
 */
void execute(int command){
	
	switch (command)
	{
	case 0:
		// Write on bedroom light
		lbrstate = !lbrstate;
		digitalWrite(LBEDROOM, lbrstate);
		break;
	case 1:
		// Write on master bedroom light
		lmbrstate = !lmbrstate;
		digitalWrite(LMASTERBEDROOM, lmbrstate);
		break;
	case 2:
		// Write on kitchen light
		lkstate = !lkstate;
		digitalWrite(LKITCHEN, lkstate);
		break;
	case 3:
		// Write on dining room light
		ldrstate = !ldrstate;
		digitalWrite(LDINNIGROOM, ldrstate);
		break;
	case 4:
		// Write on living room light
		llrstate = !llrstate;
		digitalWrite(LLIVINGROOM, llrstate);
		break;
	case 5:
		// Negate the state of the back door
		dbstate = !dbstate;
		break;
	case 6:
		// Negate the state of the front door
		dfstate = !dfstate;
		break;
	case 7:
		// Negate the state of the bedroom door
		dbrstate = !dbrstate;
		break;
	case 8:
		// Negate the state of the door
		dmbrstate = !dmbrstate;
		break;
	
	default:
		break;
	}

}


void readImages(){

	
	FILE *fp;
	char path[1035];
	
	/* Open the command for reading. */
	fp = popen("ls images/", "r");
	if (fp == NULL) {
		printf("Failed to run command\n" );
		exit(1);
	}

	// If there are no images well shit
	if (fgets(path, sizeof(path)-1, fp) == NULL){
		
	}
	else{

		// Numerate imgs
		int i = 1;
		char is[5];

		// Build starter part
		system("echo '<!doctype html><htmllang=\"en\"><head><style>.row>.column{padding:08px;}.row:after{content:\"\";display:table;clear:both;}/*Createfourequalcolumnsthatfloatsnexttoeachother*/.column{float:left;width:25%;}/*TheModal(background)*/.modal{display:none;position:fixed;z-index:1;padding-top:100px;left:0;top:0;width:100%;height:100%;overflow:auto;background-color:black;}/*ModalContent*/.modal-content{position:relative;background-color:#fefefe;margin:auto;padding:0;width:90%;max-width:1200px;}/*TheCloseButton*/.close{color:white;position:absolute;top:10px;right:25px;font-size:35px;font-weight:bold;}.close:hover,.close:focus{color:#999;text-decoration:none;cursor:pointer;}/*Hidetheslidesbydefault*/.mySlides{display:none;}/*Next&previousbuttons*/.prev,.next{cursor:pointer;position:absolute;top:50%;width:auto;padding:16px;margin-top:-50px;color:white;font-weight:bold;font-size:20px;transition:0.6sease;border-radius:03px3px0;user-select:none;-webkit-user-select:none;}/*Positionthe\"nextbutton\"totheright*/.next{right:0;border-radius:3px003px;}/*Onhover,addablackbackgroundcolorwithalittlebitsee-through*/.prev:hover,.next:hover{background-color:rgba(0,0,0,0.8);}/*Numbertext(1/3etc)*/.numbertext{color:#f2f2f2;font-size:12px;padding:8px12px;position:absolute;top:0;}/*Captiontext*/.caption-container{text-align:center;background-color:black;padding:2px16px;color:white;}img.demo{opacity:0.6;}.active,.demo:hover{opacity:1;}img.hover-shadow{transition:0.3s;}.hover-shadow:hover{box-shadow:04px8px0rgba(0,0,0,0.2),06px20px0rgba(0,0,0,0.19);}</style><metacharset=\"utf-8\"><title>Backyardcamera</title><metaname=\"description\"content=\"Backyardcamera\"><metaname=\"author\"content=\"SitePoint\"><linkrel=\"stylesheet\"href=\"css/styles.css?v=1.0\"></head><body>' > images.html");

		// Start division
		system("echo '<div class=\"row\">' >> images.html");

		// Add images to the viewer
		while (fgets(path, sizeof(path)-1, fp) != NULL) {	

			// Fill the html
			char html[1000] = "echo '<div class=\"column\"><img src=\"images/";
			// Convert int to char
			snprintf(is, sizeof(is), "%d", i);
			strcat(html, path);
			strcat(html, "\" onclick=\"openModal();currentSlide(");
			strcat(html, is);
			strcat(html , ")\" class=\"hover-shadow\"></div>' >> images.html");
			system(html);

			++i;

		}

		// Close the div
		system("echo '</div>' >> images.html");

		// Start the div
		system("echo '<div id=\"myModal\" class=\"modal\"><span class=\"close cursor\" onclick=\"closeModal()\">&times;</span><div class=\"modal-content\">' >> images.html");

		i = 1;
		// Re read all
		fp = popen("ls images/", "r");
		int total;
  		FILE *ptotal = popen("ls images/ | wc -l", "r");
		fscanf(ptotal, "%d", &total);
		// Add images to the viewer
		while (fgets(path, sizeof(path)-1, fp) != NULL) {

			// Fill the html
			char html[1000] = "echo '<div class=\"mySlides\"><div class=\"numbertext\">";
			// Convert int to char
			snprintf(is, sizeof(is), "%d", i);
			strcat(html, is);
			strcat(html, "/ ");
			// Convert int to char
			snprintf(is, sizeof(is), "%d", total);
			strcat(html, is);
			strcat(html, "</div><img src=\"images/");
			strcat(html, path);
			strcat(html , "\" style=\"width:100%\"></div>' >> images.html");

			// Call the command
			system(html);

			++i;
		}

		// Sides
		system("echo '<a class=\"prev\" onclick=\"plusSlides(-1)\">&#10094;</a><a class=\"next\" onclick=\"plusSlides(1)\">&#10095;</a>' >> images.html");
		// Caption
		system("echo '<div class=\"caption-container\"><p id=\"caption\"></p></div>' >> images.html");

		i = 1;
		// Re read all
		fp = popen("ls images/", "r");
		// Add images to the viewer
		while (fgets(path, sizeof(path)-1, fp) != NULL) {

			// Fill the html
			char html[1000] = "echo '<div class=\"column\"><img class=\"demo\" src=\"images/";
			strcat(html, path);
			strcat(html , "\" onclick=\"currentSlide(");
			// Convert int to char
			snprintf(is, sizeof(is), "%d", i);
			strcat(html, is);
			strcat(html, ")\" alt=\"");
			strcat(html, path);
			strcat(html, "\"></div>' >> images.html");

			// Call the command
			system(html);

			++i;
		}
		// Send 9 to go back
		system("echo '</div></div><button type=\"button\" class=\"btnBack\" onclick=httpPOST(9)>Back to main menu</button> </body></html>' >> images.html");
		// OCUPA EL IP AQUIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
		system("echo '<script> function openModal() {document.getElementById(\"myModal\").style.display = \"block\";}function closeModal() {document.getElementById(\"myModal\").style.display = \"none\";}var slideIndex = 1;showSlides(slideIndex);function plusSlides(n) {showSlides(slideIndex += n);}function currentSlide(n) {showSlides(slideIndex = n);}function showSlides(n) {var i;var slides = document.getElementsByClassName(\"mySlides\");var dots = document.getElementsByClassName(\"demo\");var captionText = document.getElementById(\"caption\");if (n > slides.length) {slideIndex = 1}if (n < 1) {slideIndex = slides.length}for (i = 0; i < slides.length; i++) {slides[i].style.display = \"none\";}for (i = 0; i < dots.length; i++) {dots[i].className = dots[i].className.replace(\" active\", "");}slides[slideIndex-1].style.display = \"block\";dots[slideIndex-1].className += \" active\";captionText.innerHTML = dots[slideIndex-1].alt;}function httpPOST(data){var xhr = new XMLHttpRequest();xhr.withCredentials = true;xhr.addEventListener(\"readystatechange\", function () {if (this.readyState === 4) {console.log(this.responseText);}});xhr.open(\"POST\", \"http://192.168.100.92:3000\");xhr.setRequestHeader(\"cache-control\", \"no-cache\");xhr.setRequestHeader(\"Postman-Token\", \"bb2d356c-4f5a-4baa-91eb-a6a9bf0a238d\"); xhr.send(data);console.log(\"Printed!!!\")}</script>' >> images.html");

	}

	/* close */
	pclose(fp);

}
